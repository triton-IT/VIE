#pragma once

#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/base/ibstream.h>

#include <RenderDevice.h>
#include <DeviceContext.h>
#include <RefCntAutoPtr.hpp>

#include <thread>

#include <vector>

#include "NkDiligent.h"
#include "UserInterface.h"
#include "NodeEditor.h"
#include "UserInterfaceStyle.h"

#include "Parameter.h"

#include <EngineFactoryVk.h>
#include "Parameter.h"

#ifdef _WIN32
	#include "windows/EditorView.h"
#endif

namespace live::tritone::vie {
	class VieView : public Steinberg::IPlugView
	{
	public:
		VieView(const std::vector<Parameter>& parameters);
		~VieView();

		Steinberg::tresult PLUGIN_API queryInterface(const Steinberg::TUID iid, void** obj) SMTG_OVERRIDE;
		virtual Steinberg::uint32 PLUGIN_API addRef() SMTG_OVERRIDE;
		virtual Steinberg::uint32 PLUGIN_API release() SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API isPlatformTypeSupported(Steinberg::FIDString type) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API attached(void* parent, Steinberg::FIDString type) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API removed() SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onWheel(float distance) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onKeyDown(Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onKeyUp(Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API getSize(Steinberg::ViewRect* size) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onSize(Steinberg::ViewRect* newSize) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onFocus(Steinberg::TBool state) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API setFrame(Steinberg::IPlugFrame* frame) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API canResize() SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API checkSizeConstraint(Steinberg::ViewRect* rect) SMTG_OVERRIDE;

		Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state);
		Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state);

		void InitializeNuklear();

		void render();

	private:
		static Diligent::IEngineFactoryVk* pFactoryVk;
		static Diligent::RefCntAutoPtr<Diligent::IRenderDevice>  m_pDevice;
		static Diligent::RefCntAutoPtr<Diligent::IDeviceContext> m_pImmediateContext;

		EditorView* m_pEditorView;

		Steinberg::uint32 nbRef_;
		Steinberg::IPlugFrame* frame_;

		//FrequencyParameter* frequencyParameter_;

		int width_;
		int height_;

		Diligent::RefCntAutoPtr<Diligent::IPipelineState> m_pPSO;
		
		Diligent::RefCntAutoPtr<Diligent::ISwapChain> m_pSwapChain;

		nk_diligent_context* m_pNkDlgCtx;
		nk_context* m_pNkCtx;

		UserInterface m_ui;
		NodeEditor m_nodeEditor;
		UserInterfaceStyle m_uiStyle;

		std::unique_ptr<std::thread> rendererThread;
		bool isRendererRunning;

		bool InitializeDiligentEngine();
		void CreateResources();
		void Render();
		void Present();
	};
}
