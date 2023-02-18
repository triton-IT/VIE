#pragma once

#include "../host_callback.hpp"

#include <pluginterfaces/vst/ivsteditcontroller.h>

namespace live::tritone::vie {
	class vst_host_callback : public host_callback {
	public:
		vst_host_callback(Steinberg::Vst::IComponentHandler* component_handler_);
		~vst_host_callback();
		
		void beginEdit(uint32_t id);
		void performEdit(uint32_t id, double valueNormalized);
		void endEdit(uint32_t id);
		void restartComponent(int32_t flags);

		Steinberg::Vst::IComponentHandler* get_vst_handler();

	private:
		Steinberg::Vst::IComponentHandler* vst_component_handler_;
	};
}
