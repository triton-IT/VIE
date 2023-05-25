#include "../application.hpp"
#include "../logger.hpp"

#include "../boolean_parameter.hpp"
#include "../float_parameter.hpp"

#include <pluginterfaces/base/ustring.h>

extern "C" {
	bool InitDll() {
		return true;
	}

	bool ExitDll() {
		return true;
	}

	std::wstring content_path;

#ifdef VIE_DEBUG
	live::tritone::vie::debug::logger debugLogger;
#endif //VIE_DEBUG
} // extern "C"

parameter application::add_parameter(uint_fast8_t id, parameter parameter)
{
	auto parameter_ptr = new live::tritone::vie::parameter(parameter);

	parameters_[nb_parameters_] = parameter_ptr;
	nb_parameters_++;

	return *parameter_ptr;
}

parameter application::add_parameter(uint_fast8_t id, Steinberg::char16 name[128], Steinberg::char16 short_name[128], Steinberg::char16 type[128], Steinberg::char16 unit[128], float value) {
    Steinberg::UString boolean_type(nullptr, sizeof("booblean"));
    boolean_type.fromAscii("boolean");
    std::u16string str;
	if (type == boolean_type) {
		//FIXME: use wstring in parameters and fund a way to parse wstring with json.
		boolean_parameter parameter(id,
			name,
			short_name,
			unit,
			value);
		return add_parameter(id, parameter);
	}
	else {
		float_parameter parameter(id,
			name,
			short_name,
			unit,
			value);
		return add_parameter(id, parameter);
	}
}

parameter application::get_parameter(uint_fast8_t id)
{
	return *parameters_[id];
}

uint_fast8_t application::get_parameters_count()
{
	return nb_parameters_;
}

parameter* application::parameters_[255] = { nullptr };
uint_fast8_t application::nb_parameters_ = 0;
