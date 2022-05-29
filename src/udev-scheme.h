#include <tll/scheme/binder.h>

namespace udev_scheme {

static constexpr std::string_view scheme_string = R"(yamls+gz://eJyNkd1qwkAQhe99irkbkATUWpDcpe0DlEIfYHVHXWpml+wmJci+uzNGLUiLvdpzdj7mtwQ2DVWA760P1KYBJwBbRwcbK1EAJRwvhD5YQBqCuphaxzvMd1BvDt1vVHmt80a925BWcbaC+UwEcddcqmG9Sc4zVnAcMzhOq+JMyBfW1kpoXgC+OFa5EPm6N7wjMU9iPqjxvZpnMZ+8HrHl2Xyx/9bUs5z/HNKM9W8D1Ffvg4qoXWhI0qB2hfl+A7FbxyEmah7uSrB/7dRSH0zaP+TCeEJH8Qed3u6aJyfOTZQG)";

template <typename Buf>
struct Property : public tll::scheme::Binder<Buf>
{
	using tll::scheme::Binder<Buf>::Binder;

	static constexpr size_t meta_size() { return 16; }
	static constexpr std::string_view meta_name() { return "Property"; }

	std::string_view get_name() const { return this->template _get_string<tll_scheme_offset_ptr_t>(0); }
	void set_name(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(0, v); }

	std::string_view get_value() const { return this->template _get_string<tll_scheme_offset_ptr_t>(8); }
	void set_value(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(8, v); }
};

template <typename Buf>
struct Device : public tll::scheme::Binder<Buf>
{
	using tll::scheme::Binder<Buf>::Binder;

	static constexpr size_t meta_size() { return 33; }
	static constexpr std::string_view meta_name() { return "Device"; }
	static constexpr int meta_id() { return 10; }

	enum class Action: int8_t
	{
		Unknown = 0,
		Add = 1,
		Bind = 2,
		Change = 3,
		Unbind = 4,
		Remove = 5,
	};

	using type_action = Action;
	type_action get_action() const { return this->template _get_scalar<type_action>(0); }
	void set_action(type_action v) { return this->template _set_scalar<type_action>(0, v); }

	std::string_view get_subsystem() const { return this->template _get_string<tll_scheme_offset_ptr_t>(1); }
	void set_subsystem(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(1, v); }

	std::string_view get_sysname() const { return this->template _get_string<tll_scheme_offset_ptr_t>(9); }
	void set_sysname(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(9, v); }

	std::string_view get_devpath() const { return this->template _get_string<tll_scheme_offset_ptr_t>(17); }
	void set_devpath(std::string_view v) { return this->template _set_string<tll_scheme_offset_ptr_t>(17, v); }

	using type_properties = tll::scheme::binder::List<Buf, Property<Buf>, tll_scheme_offset_ptr_t>;
	const type_properties get_properties() const { return this->template _get_binder<type_properties>(25); }
	type_properties get_properties() { return this->template _get_binder<type_properties>(25); }
};

} // namespace udev_scheme
