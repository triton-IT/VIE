#pragma once

#include <pluginterfaces/vst/vsttypes.h>

namespace live::tritone::vie {
	class i_parameter_listener {
	public:
		i_parameter_listener() = default;
		i_parameter_listener(const i_parameter_listener&) = default;
		i_parameter_listener(i_parameter_listener&&) = default;

		virtual ~i_parameter_listener() = default;

		i_parameter_listener& operator=(const i_parameter_listener&) = default;
		i_parameter_listener& operator=(i_parameter_listener&&) = default;

		virtual void parameter_value_changed(Steinberg::Vst::ParamID parameter_id, Steinberg::Vst::ParamValue normalized_value) = 0;
	};
} // namespace

