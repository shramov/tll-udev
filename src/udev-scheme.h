#pragma once

#include <tll/scheme/binder.h>
#include <tll/util/conv.h>

namespace udev_scheme {

static constexpr std::string_view scheme_string =
    R"(yamls+gz://eJyNkd0KgkAQhe99irlbCIV+IbyzeoAIeoC1nXJJR3FXQ8J3b1atIAq72nN2P+bszARAMsMQxL7MCyxtIzyAs8ZUmZAVQAD3gXCH8ME2hXPGlpouov2AaplW36jgmbPDWp/QpWgVwmzKAqnKhjQRnazOSYRw7ytosmu/I/hKRErx08wHsdHk5JzlNpF0QTYLNgfM8tqZFZsjxT227MyV8psrPW3bn03KPv/VwPCfzzZNFZvGWMxGB8LYX4NTWBfSJqNc0e9Jo3mjk9fyWu8BOvWK7w==)";

struct Property
{
	static constexpr size_t meta_size() { return 16; }
	static constexpr std::string_view meta_name() { return "Property"; }

	template <typename Buf>
	struct binder_type : public tll::scheme::Binder<Buf>
	{
		using tll::scheme::Binder<Buf>::Binder;

		static constexpr auto meta_size() { return Property::meta_size(); }
		static constexpr auto meta_name() { return Property::meta_name(); }
		void view_resize() { this->_view_resize(meta_size()); }

		std::string_view get_name() const { return this->template _get_string<tll_scheme_offset_ptr_t>(0); }
		void set_name(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(0, v); }

		std::string_view get_value() const { return this->template _get_string<tll_scheme_offset_ptr_t>(8); }
		void set_value(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(8, v); }
	};

	template <typename Buf>
	static binder_type<Buf> bind(Buf &buf, size_t offset = 0)
	{
		return binder_type<Buf>(tll::make_view(buf).view(offset));
	}
};

struct Device
{
	static constexpr size_t meta_size() { return 33; }
	static constexpr std::string_view meta_name() { return "Device"; }
	static constexpr int meta_id() { return 10; }

	enum class Action : int8_t
	{
		Unknown = 0,
		Add = 1,
		Bind = 2,
		Change = 3,
		Unbind = 4,
		Remove = 5,
	};

	template <typename Buf>
	struct binder_type : public tll::scheme::Binder<Buf>
	{
		using tll::scheme::Binder<Buf>::Binder;

		static constexpr auto meta_size() { return Device::meta_size(); }
		static constexpr auto meta_name() { return Device::meta_name(); }
		static constexpr auto meta_id() { return Device::meta_id(); }
		void view_resize() { this->_view_resize(meta_size()); }

		using type_action = Action;
		type_action get_action() const { return this->template _get_scalar<type_action>(0); }
		void set_action(type_action v) { return this->template _set_scalar<type_action>(0, v); }

		std::string_view get_subsystem() const { return this->template _get_string<tll_scheme_offset_ptr_t>(1); }
		void set_subsystem(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(1, v); }

		std::string_view get_sysname() const { return this->template _get_string<tll_scheme_offset_ptr_t>(9); }
		void set_sysname(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(9, v); }

		std::string_view get_devpath() const { return this->template _get_string<tll_scheme_offset_ptr_t>(17); }
		void set_devpath(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(17, v); }

		using type_properties = tll::scheme::binder::List<Buf, Property::binder_type<Buf>, tll_scheme_offset_ptr_t>;
		const type_properties get_properties() const { return this->template _get_binder<type_properties>(25); }
		type_properties get_properties() { return this->template _get_binder<type_properties>(25); }
	};

	template <typename Buf>
	static binder_type<Buf> bind(Buf &buf, size_t offset = 0)
	{
		return binder_type<Buf>(tll::make_view(buf).view(offset));
	}
};

} // namespace udev_scheme

template <>
struct tll::conv::dump<udev_scheme::Device::Action> : public to_string_from_string_buf<udev_scheme::Device::Action>
{
	template <typename Buf>
	static inline std::string_view to_string_buf(const udev_scheme::Device::Action &v, Buf &buf)
	{
		switch (v) {
		case udev_scheme::Device::Action::Add:
			return "Add";
		case udev_scheme::Device::Action::Bind:
			return "Bind";
		case udev_scheme::Device::Action::Change:
			return "Change";
		case udev_scheme::Device::Action::Remove:
			return "Remove";
		case udev_scheme::Device::Action::Unbind:
			return "Unbind";
		case udev_scheme::Device::Action::Unknown:
			return "Unknown";
		default:
			break;
		}
		return tll::conv::to_string_buf<int8_t, Buf>((int8_t) v, buf);
	}
};
