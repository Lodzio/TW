////////////////////////////////////////////////////////////////////////////////
// Filename: D2DClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "d2dclass.h"


D2DClass::D2DClass()
{

}


D2DClass::D2DClass(const D2DClass& other)
{
}


D2DClass::~D2DClass()
{
}


bool D2DClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	HRESULT HR;
	scrWidth = screenWidth;
	scrHeight = screenHeight;

	HR = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Factory);
	if (HR != S_OK)
	{
		return false;
	}
	
	ID2D1Factory1* Factory;
	HR = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);
	if (HR != S_OK)
	{
		return false;
	}

	// This flag adds support for surfaces with a different color channel ordering than the API default.
	// You need it for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	// This array defines the set of DirectX hardware feature levels this app  supports.
	// The ordering is important and you should  preserve it.
	// Don't forget to declare your app's minimum required feature level in its
	// description.  All apps are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// Create the DX11 API device object, and get a corresponding context.
	ID3D11Device* device3D;
	ID3D11DeviceContext* context;

	HR = D3D11CreateDevice(
		nullptr,                    // specify null to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,              // optionally set debug and Direct2D compatibility flags
		featureLevels,              // list of feature levels this app can support
		ARRAYSIZE(featureLevels),   // number of possible feature levels
		D3D11_SDK_VERSION,
		&device3D,                    // returns the Direct3D device created
		&m_featureLevel,            // returns feature level of device created
		&context                    // returns the device immediate context
	);
	if (HR != S_OK)
	{
		return false;
	}

	IDXGIDevice1* dxgiDevice;
	// Obtain the underlying DXGI device of the Direct3D11 device.
	HR = device3D->QueryInterface(__uuidof(dxgiDevice), (void **)&dxgiDevice);
	if (HR != S_OK)
	{
		return false;
	}

	// Obtain the Direct2D device for 2-D rendering.
	HR = Factory->CreateDevice(dxgiDevice, &m_Device);
	if (HR != S_OK)
	{
		return false;
	}

	HR = m_Device->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&m_Context);
	if (HR != S_OK)
	{
		return false;
	}

	// Allocate a descriptor.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.Width = 0;                           // use automatic sizing
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most common swapchain format
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all apps must use this SwapEffect
	swapChainDesc.Flags = 0;

	// Identify the physical adapter (GPU or card) this device is runs on.
	IDXGIAdapter* dxgiAdapter;
	HR = dxgiDevice->GetAdapter(&dxgiAdapter);

	// Get the factory object that created the DXGI device.
	IDXGIFactory2* dxgiFactory;
	HR = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

	// Get the final swap chain for this window from the DXGI factory.
	HR = dxgiFactory->CreateSwapChainForHwnd(
		device3D,
		hwnd,
		&swapChainDesc,
		nullptr,   // allow on all displays
		NULL,
		&m_Swapchain
	);
	if (HR != S_OK)
	{
		return false;
	}

	// Ensure that DXGI doesn't queue more than one frame at a time.
	HR = dxgiDevice->SetMaximumFrameLatency(1);
	if (HR != S_OK)
	{
		return false;
	}

	// Get the backbuffer for this window which is be the final 3D render target.
	ID3D11Texture2D* backBuffer;
	HR = m_Swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (HR != S_OK)
	{
		return false;
	}

	// Now we set up the Direct2D render target bitmap linked to the swapchain. 
	// Whenever we render to this bitmap, it is directly rendered to the 
	// swap chain associated with the window.
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)
		);

	// Direct2D needs the dxgi version of the backbuffer surface pointer.
	IDXGISurface* dxgiBackBuffer;
	HR = m_Swapchain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
	if (HR != S_OK)
	{
		return false;
	}

	// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
	HR = m_Context->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer,
		&bitmapProperties,
		&m_d2dTargetBitmap
	);
	if (HR != S_OK)
	{
		return false;
	}

	// Now we can set the Direct2D render target.
	m_Context->SetTarget(m_d2dTargetBitmap);

	return true;
}


void D2DClass::Shutdown()
{
	return;
}

ID2D1DeviceContext * D2DClass::GetRenderTarget()
{
	return m_Context;
}

D2D1_SIZE_U D2DClass::GetResolution()
{
	D2D1_SIZE_U output;
	output.height = scrHeight;
	output.width = scrWidth;
	return output;
}


void D2DClass::BeginScene()
{
	m_Context->BeginDraw();
	return;
}


HRESULT D2DClass::EndScene()
{
	HRESULT HR;
	if (HR = m_Context->EndDraw() != S_OK)
	{
		return HR;
	}
	HR = m_Swapchain->Present(1, 0);
	return HR;
}
