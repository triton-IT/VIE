#pragma once

#include <pluginterfaces/base/ipluginbase.h>

#include "Constants.h"

namespace live::tritone::vie {
	class VstPluginFactory : public Steinberg::IPluginFactory3
	{
	public:
		VstPluginFactory();

		//Inherit from IPluginFactory3
		Steinberg::tresult PLUGIN_API getClassInfoUnicode(Steinberg::int32 index, Steinberg::PClassInfoW* info);
		Steinberg::tresult PLUGIN_API setHostContext(Steinberg::FUnknown* context);
		//Inherit from IPluginFactory2
		Steinberg::tresult PLUGIN_API getClassInfo2(Steinberg::int32 index, Steinberg::PClassInfo2* info);
		//Inherit from IPluginFactory
		Steinberg::tresult PLUGIN_API getFactoryInfo(Steinberg::PFactoryInfo* info);
		Steinberg::int32 PLUGIN_API countClasses();
		Steinberg::tresult PLUGIN_API getClassInfo(Steinberg::int32 index, Steinberg::PClassInfo* info);
		Steinberg::tresult PLUGIN_API createInstance(Steinberg::FIDString cid, Steinberg::FIDString _iid, void** obj);
		//Inherit from FUnknown
		Steinberg::tresult PLUGIN_API queryInterface(const Steinberg::TUID _iid, void** obj);
		Steinberg::uint32 PLUGIN_API addRef();
		Steinberg::uint32 PLUGIN_API release();

	private:
		Steinberg::PFactoryInfo factoryInfo_;
		Steinberg::IPtr<Steinberg::FUnknown> hostContext_;
		Steinberg::uint32 nbRef_;

		Steinberg::PClassInfo2 controllerClassInfo_;
		Steinberg::PClassInfo2 processorClassInfo_;

		Steinberg::PClassInfoW controllerClassInfoW_;
		Steinberg::PClassInfoW processorClassInfoW_;
	};
}