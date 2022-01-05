#pragma once

#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/base/ibstream.h>

#include "NkDiligent.h"
#include <RenderDevice.h>
#include <DeviceContext.h>
#include <RefCntAutoPtr.hpp>

#include <thread>

#include "UserInterface.h"
#include "NodeEditor.h"
#include "UserInterfaceStyle.h"

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

#include "FrequencyParameter.h"

namespace live::tritone::vie {
	class VieView : public Steinberg::IPlugView
	{
	public:
		VieView(FrequencyParameter* frequencyParameter);
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
		Steinberg::uint32 nbRef_;
		Steinberg::IPlugFrame* frame_;
		void* parent_;

		FrequencyParameter* frequencyParameter_;

		int width_;
		int height_;

		Diligent::RefCntAutoPtr<Diligent::IRenderDevice>  m_pDevice;
		Diligent::RefCntAutoPtr<Diligent::IDeviceContext> m_pImmediateContext;
		Diligent::RefCntAutoPtr<Diligent::IPipelineState> m_pPSO;
		struct WindowInfo
		{
			Diligent::RefCntAutoPtr<Diligent::ISwapChain> pSwapChain;
			HWND                      hWnd;
		};
		WindowInfo m_Window;

		nk_diligent_context* m_pNkDlgCtx = nullptr;
		nk_context* m_pNkCtx = nullptr;

		UserInterface m_ui;
		NodeEditor m_nodeEditor;
		UserInterfaceStyle m_uiStyle;

		std::thread* rendererThread;

		bool InitializeDiligentEngine(HWND hWnd);
		void CreateResources();
		void Render();
		void Present();

		static LONG_PTR WINAPI MessageProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		LONG_PTR WINAPI HandleWin32Message(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}
