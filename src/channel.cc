/*
 * Copyright (c) 2021 Pavel Shramov <shramov@mexmat.net>
 *
 * tll is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <libudev.h>
#include <unistd.h>

#include "tll/channel/base.h"
#include "tll/channel/module.h"

#include "udev-scheme.h"

using namespace tll;

class UDev : public tll::channel::Base<UDev>
{
	udev * _udev = nullptr;
	udev_monitor * _monitor = nullptr;

	std::string _subsystem;
	std::string _devtype;

	std::vector<char> _buf;

 public:
	static constexpr std::string_view channel_protocol() { return "udev"; }
	static constexpr auto scheme_policy() { return Base::SchemePolicy::Manual; }

	int _init(const tll::Channel::Url &, tll::Channel *master);
	int _open(const tll::ConstConfig &);
	int _close();
	void _destroy();

	int _process(long timeout, int flags);

 private:
};

int UDev::_init(const Channel::Url &url, Channel * master)
{
	if (_scheme_url)
		return _log.fail(EINVAL, "UDev channel has it's own scheme, conflicts with init parameter");
	_scheme.reset(context().scheme_load(udev_scheme::scheme_string));
	if (!_scheme.get())
		return _log.fail(EINVAL, "Failed to load udev scheme");

	auto reader = channel_props_reader(url);
	_subsystem = reader.getT("subsystem", std::string());
	_devtype = reader.getT("devtype", std::string());
	if (!reader)
		return _log.fail(EINVAL, "Invalid url: {}", reader.error());

	return 0;
}

int UDev::_open(const tll::ConstConfig &s)
{
	_udev = udev_new();
	if (!_udev)
		return _log.fail(EINVAL, "Failed to create udev");

	_monitor = udev_monitor_new_from_netlink(_udev, "udev");
	if (!_monitor)
		return _log.fail(EINVAL, "Failed to create monitor for {}", "udev");

	if (_subsystem.size()) {
		_log.info("Filter by subsystem: {}, devtype: {}", _subsystem, _devtype);
		if (udev_monitor_filter_add_match_subsystem_devtype(_monitor, _subsystem.c_str(), _devtype.size()?_devtype.c_str():nullptr) < 0)
			return _log.fail(EINVAL, "Failed to add subsystem filter for {}/{}", _subsystem, _devtype);
	}
	if (udev_monitor_enable_receiving(_monitor) < 0)
		return _log.fail(EINVAL, "Failed to enable udev monitor");
	auto fd = udev_monitor_get_fd(_monitor);
	if (fd != -1) {
		_log.debug("Register monitor fd {}", fd);
		_update_fd(fd);
		_update_dcaps(dcaps::CPOLLIN);
	}

	return 0;
}

int UDev::_close()
{
	_update_fd(-1);
	if (_monitor)
		udev_monitor_unref(_monitor);
	_monitor = nullptr;
	if (_udev)
		udev_unref(_udev);
	_udev = nullptr;
	return 0;
}

int UDev::_process(long timeout, int flags)
{
	auto dev = udev_monitor_receive_device(_monitor);
	if (!dev)
		return _log.fail(EAGAIN, "Failed to get udev device");

	_buf.resize(0);
	auto data = tll::scheme::make_binder<udev_scheme::Device>(_buf);
	_buf.resize(data.meta_size());
	std::string_view action = udev_device_get_action(dev);
	if (action == "add") data.set_action(data.Action::Add);
	else if (action == "bind") data.set_action(data.Action::Bind);
	else if (action == "change") data.set_action(data.Action::Change);
	else if (action == "unbind") data.set_action(data.Action::Unbind);
	else if (action == "remove") data.set_action(data.Action::Remove);
	else
		data.set_action(data.Action::Unknown);
	data.set_subsystem(udev_device_get_subsystem(dev));
	data.set_sysname(udev_device_get_sysname(dev));
	data.set_devpath(udev_device_get_devpath(dev));

	_log.info("{}: {}", udev_device_get_action(dev), udev_device_get_sysname(dev));

	size_t idx = 0;
	auto list = udev_device_get_properties_list_entry(dev);
	for (; list; list = udev_list_entry_get_next(list))
		idx++;

	auto props = data.get_properties();
	props.resize(idx);

	list = udev_device_get_properties_list_entry(dev);
	for (idx = 0; list; list = udev_list_entry_get_next(list)) {
		auto p = props[idx++];
		p.set_name(udev_list_entry_get_name(list));
		p.set_value(udev_list_entry_get_value(list));
	}

	udev_device_unref(dev);

	tll_msg_t msg = {};
	msg.msgid = data.meta_id();
	msg.data = _buf.data();
	msg.size = _buf.size();
	_callback_data(&msg);

	return 0;
}

TLL_DEFINE_IMPL(UDev);

TLL_DEFINE_MODULE(UDev);
