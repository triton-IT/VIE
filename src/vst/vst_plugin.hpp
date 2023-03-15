#pragma once

#include <pluginterfaces/base/ipluginbase.h>

namespace live::tritone::vie::vst {
	class vst_plugin {
	public:
		static const Steinberg::FUID processor_uid;
		static const Steinberg::FUID controller_uid;
	};
}