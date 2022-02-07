#pragma once

#include <pluginterfaces/base/ipluginbase.h>

#include "constants.hpp"

namespace live::tritone::vie {
	class vst_plugin_factory : public Steinberg::IPluginFactory3 {
	public:
		vst_plugin_factory();
		vst_plugin_factory(vst_plugin_factory&) = default;
		vst_plugin_factory(vst_plugin_factory&&) = default;

		virtual ~vst_plugin_factory() = default;

		vst_plugin_factory& operator=(const vst_plugin_factory&) = default;
		vst_plugin_factory& operator=(vst_plugin_factory&&) = default;

		//Inherit from IPluginFactory3
		Steinberg::tresult PLUGIN_API
			getClassInfoUnicode(Steinberg::int32
				index,
				Steinberg::PClassInfoW* info
			) override;
		Steinberg::tresult PLUGIN_API
			setHostContext(Steinberg::FUnknown
				* context) override;
		//Inherit from IPluginFactory2
		Steinberg::tresult PLUGIN_API
			getClassInfo2(Steinberg::int32
				index,
				Steinberg::PClassInfo2* info
			) override;
		//Inherit from IPluginFactory
		Steinberg::tresult PLUGIN_API
			getFactoryInfo(Steinberg::PFactoryInfo
				* info) override;
		Steinberg::int32 PLUGIN_API

			countClasses() override;

		Steinberg::tresult PLUGIN_API
			getClassInfo(Steinberg::int32
				index,
				Steinberg::PClassInfo* info
			) override;
		Steinberg::tresult PLUGIN_API
			createInstance(Steinberg::FIDString
				cid,
				Steinberg::FIDString _iid,
				void** obj
			) override;
		//Inherit from FUnknown
		Steinberg::tresult PLUGIN_API

			queryInterface(const Steinberg::TUID _iid, void** obj) override;

		Steinberg::uint32 PLUGIN_API

			addRef() override;

		Steinberg::uint32 PLUGIN_API

			release() override;

	private:
		Steinberg::PFactoryInfo factory_info_;
		Steinberg::IPtr <Steinberg::FUnknown> host_context_;
		Steinberg::uint32 nb_ref_;

		Steinberg::PClassInfo2 controller_class_info_;
		Steinberg::PClassInfo2 processor_class_info_;

		Steinberg::PClassInfoW controller_class_info_w_;
		Steinberg::PClassInfoW processor_class_info_w_;
	};
}