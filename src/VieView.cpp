#include "VieView.h"

#include "Application.h"

#include <fstream>

#ifndef NOMINMAX
#    define NOMINMAX
#endif

#include "EngineFactoryVk.h"

#include "RenderDevice.h"
#include "DeviceContext.h"
#include "SwapChain.h"

#include "RefCntAutoPtr.hpp"


using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace Diligent;

static const char* VSSource = R"(
struct PSInput 
{ 
    float4 Pos   : SV_POSITION; 
    float3 Color : COLOR; 
};

void main(in  uint    VertId : SV_VertexID,
          out PSInput PSIn) 
{
    float4 Pos[3];
    Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
    Pos[1] = float4( 0.0, +0.5, 0.0, 1.0);
    Pos[2] = float4(+0.5, -0.5, 0.0, 1.0);

    float3 Col[3];
    Col[0] = float3(1.0, 0.0, 0.0); // red
    Col[1] = float3(0.0, 1.0, 0.0); // green
    Col[2] = float3(0.0, 0.0, 1.0); // blue

    PSIn.Pos   = Pos[VertId];
    PSIn.Color = Col[VertId];
}
)";

// Pixel shader simply outputs interpolated vertex color
static const char* PSSource = R"(
struct PSInput 
{ 
    float4 Pos   : SV_POSITION; 
    float3 Color : COLOR; 
};

struct PSOutput
{ 
    float4 Color : SV_TARGET; 
};

void main(in  PSInput  PSIn,
          out PSOutput PSOut)
{
    PSOut.Color = float4(PSIn.Color.rgb, 1.0);
}
)";


class Tutorial00App
{
public:
	~Tutorial00App()
	{
		if (m_pImmediateContext)
			m_pImmediateContext->Flush();
	}

	bool InitializeDiligentEngine(HWND hWnd)
	{
		m_Window.hWnd = hWnd;

		SwapChainDesc SCDesc;
#if EXPLICITLY_LOAD_ENGINE_VK_DLL
		// Load the dll and import GetEngineFactoryVk() function
		auto GetEngineFactoryVk = LoadGraphicsEngineVk();
#endif
		EngineVkCreateInfo EngineCI;

		auto* pFactoryVk = GetEngineFactoryVk();
		pFactoryVk->CreateDeviceAndContextsVk(EngineCI, &m_pDevice, &m_pImmediateContext);

		Win32NativeWindow Window{ m_Window.hWnd };
		pFactoryVk->CreateSwapChainVk(m_pDevice, m_pImmediateContext, SCDesc, Window, &m_Window.pSwapChain);
		SCDesc.IsPrimary = false;

		return true;
	}

	void CreateResources()
	{
		// Pipeline state object encompasses configuration of all GPU stages

		GraphicsPipelineStateCreateInfo PSOCreateInfo;

		// Pipeline state name is used by the engine to report issues.
		// It is always a good idea to give objects descriptive names.
		PSOCreateInfo.PSODesc.Name = "Simple triangle PSO";

		// This is a graphics pipeline
		PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

		// clang-format off
		// This tutorial will render to a single render target
		PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
		// Set render target format which is the format of the swap chain's color buffer
		ISwapChain* pSwapChain = m_Window.pSwapChain;
		PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = pSwapChain->GetDesc().ColorBufferFormat;
		// Use the depth buffer format from the swap chain
		PSOCreateInfo.GraphicsPipeline.DSVFormat = pSwapChain->GetDesc().DepthBufferFormat;
		// Primitive topology defines what kind of primitives will be rendered by this pipeline state
		PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		// No back face culling for this tutorial
		PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
		// Disable depth testing
		PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
		// clang-format on

		ShaderCreateInfo ShaderCI;
		// Tell the system that the shader source code is in HLSL.
		// For OpenGL, the engine will convert this into GLSL under the hood
		ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
		// OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
		ShaderCI.UseCombinedTextureSamplers = true;
		// Create a vertex shader
		RefCntAutoPtr<IShader> pVS;
		{
			ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
			ShaderCI.EntryPoint = "main";
			ShaderCI.Desc.Name = "Triangle vertex shader";
			ShaderCI.Source = VSSource;
			m_pDevice->CreateShader(ShaderCI, &pVS);
		}

		// Create a pixel shader
		RefCntAutoPtr<IShader> pPS;
		{
			ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
			ShaderCI.EntryPoint = "main";
			ShaderCI.Desc.Name = "Triangle pixel shader";
			ShaderCI.Source = PSSource;
			m_pDevice->CreateShader(ShaderCI, &pPS);
		}

		// Finally, create the pipeline state
		PSOCreateInfo.pVS = pVS;
		PSOCreateInfo.pPS = pPS;
		m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
	}

	void Render()
	{
		auto& WndInfo = m_Window;

		ITextureView* pRTV = WndInfo.pSwapChain->GetCurrentBackBufferRTV();
		ITextureView* pDSV = WndInfo.pSwapChain->GetDepthBufferDSV();
		m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		// Clear the back buffer
		const float ClearColor[] = { 0.350f, 0.350f, 0.350f, 1.0f };
		// Let the engine perform required state transitions
		m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		// Set the pipeline state in the immediate context
		m_pImmediateContext->SetPipelineState(m_pPSO);

		DrawAttribs drawAttrs;
		drawAttrs.NumVertices = 3; // Render 3 vertices
		m_pImmediateContext->Draw(drawAttrs);
	}

	void Present()
	{
		if (m_Window.pSwapChain)
			m_Window.pSwapChain->Present();
	}

	void WindowResize(HWND hWnd, Uint32 Width, Uint32 Height)
	{
		if (m_Window.hWnd == hWnd)
		{
			if (m_Window.pSwapChain)
				m_Window.pSwapChain->Resize(Width, Height);
		}
	}

private:
	RefCntAutoPtr<IRenderDevice>  m_pDevice;
	RefCntAutoPtr<IDeviceContext> m_pImmediateContext;
	RefCntAutoPtr<IPipelineState> m_pPSO;
	struct WindowInfo
	{
		RefCntAutoPtr<ISwapChain> pSwapChain;
		HWND                      hWnd;
	};
	WindowInfo m_Window;
};

std::unique_ptr<Tutorial00App> g_pTheApp;

// Called every time the NativeNativeAppBase receives a message
LRESULT CALLBACK MessageProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(wnd, &ps);
		EndPaint(wnd, &ps);
		return 0;
	}
	case WM_SIZE: // Window size has been changed
		if (g_pTheApp)
		{
			g_pTheApp->WindowResize(wnd, LOWORD(lParam), HIWORD(lParam));
		}
		return 0;

	case WM_CHAR:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 320;
		lpMMI->ptMinTrackSize.y = 240;
		return 0;
	}

	default:
		return DefWindowProc(wnd, message, wParam, lParam);
	}
}

namespace live::tritone::vie {
	VieView::VieView(FrequencyParameter* frequencyParameter) :
		nbRef_(0),
		frame_(nullptr),
		width_(1024),
		height_(600),
		frequencyParameter_(frequencyParameter),
		//view_(nullptr),
		parent_(nullptr)
	{
	}

	VieView::~VieView()
	{
	}

	tresult PLUGIN_API VieView::queryInterface(const TUID iid, void** obj)
	{
		if (::Steinberg::FUnknownPrivate::iidEqual(iid, ::FUnknown::iid)) {
			addRef();
			*obj = static_cast<VieView*>(this);
			return ::Steinberg::kResultOk;
		}

		return kResultFalse;
	}

	uint32 PLUGIN_API VieView::addRef()
	{
		return ++nbRef_;
	}

	uint32 PLUGIN_API VieView::release()
	{
		return --nbRef_;
	}

	tresult PLUGIN_API VieView::isPlatformTypeSupported(FIDString type)
	{
		if (strcmp(type, "HWND") == 0) {
			return kResultTrue;
		}

		return kResultFalse;
	}

	tresult PLUGIN_API VieView::attached(void* parent, FIDString /*type*/)
	{
		parent_ = parent;
		g_pTheApp.reset(new Tutorial00App);

		// Register our window class
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, MessageProc,
						   0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"SampleApp", NULL };
		RegisterClassEx(&wcex);

		HWND hWnd;
		RECT WndRect = { 0, 0, 1024, 768 };

		std::wstringstream TitleSS;
		TitleSS << L"Tutorial15: Multiple Windows";
		auto Title = TitleSS.str();

		AdjustWindowRect(&WndRect, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindow(L"SampleApp", Title.c_str(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			WndRect.right - WndRect.left, WndRect.bottom - WndRect.top, NULL, NULL, GetModuleHandle(NULL), NULL);
		if (!hWnd)
		{
			MessageBox(NULL, L"Cannot create window", L"Error", MB_OK | MB_ICONERROR);
			return -1;
		}
		ShowWindow(hWnd, 1);
		UpdateWindow(hWnd);

		if (!g_pTheApp->InitializeDiligentEngine(hWnd))
			return -1;

		g_pTheApp->CreateResources();

		// Main message loop
		MSG msg = { 0 };
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				g_pTheApp->Render();
				g_pTheApp->Present();
			}
		}

		//g_pTheApp.reset();

		return kResultTrue;
	}

	tresult PLUGIN_API VieView::removed()
	{
		return kResultTrue;
	}

	tresult PLUGIN_API VieView::onWheel(float /*distance*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieView::onKeyDown(char16 /*key*/, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieView::onKeyUp(char16 /*key*/, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieView::getSize(ViewRect* size)
	{
		size->left = 0;
		size->top = 0;
		size->right = width_;
		size->bottom = height_;

		return kResultOk;
	}

	tresult PLUGIN_API VieView::onSize(ViewRect* newSize)
	{
		//if (m_Window.hWnd == hWnd)
		//{
		//	if (m_Window.pSwapChain)
		//		m_Window.pSwapChain->Resize(Width, Height);
		//}

		return kResultOk;
	}

	tresult PLUGIN_API VieView::onFocus(TBool /*state*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieView::setFrame(IPlugFrame* frame)
	{
		frame_ = frame;

		return kResultOk;
	}

	tresult PLUGIN_API VieView::canResize()
	{
		return kResultFalse;
	}
	
	tresult PLUGIN_API VieView::checkSizeConstraint(ViewRect* /*rect*/)
	{
		return kResultOk;
	}
} // namespace
