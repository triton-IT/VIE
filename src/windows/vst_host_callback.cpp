#include "vst_host_callback.hpp"

using namespace live::tritone::vie;

vst_host_callback::vst_host_callback(Steinberg::Vst::IComponentHandler* component_handler) : vst_component_handler_(component_handler)
{
	
}

vst_host_callback::~vst_host_callback()
{
	
}

void vst_host_callback::beginEdit(uint32_t id)
{
	vst_component_handler_->beginEdit(id);
}

void vst_host_callback::performEdit(uint32_t id, double valueNormalized)
{
	vst_component_handler_->performEdit(id, valueNormalized);
}

void vst_host_callback::endEdit(uint32_t id)
{
	vst_component_handler_->endEdit(id);
}

void vst_host_callback::restartComponent(int32_t flags)
{
	vst_component_handler_->restartComponent(flags);
}

Steinberg::Vst::IComponentHandler* vst_host_callback::get_vst_handler()
{
	return vst_component_handler_;
}