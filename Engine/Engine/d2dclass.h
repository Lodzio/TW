////////////////////////////////////////////////////////////////////////////////
// Filename: D2DClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D2DClass_H_
#define _D2DClass_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "Windowscodecs.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "D3D11.lib")



//////////////
// INCLUDES //
//////////////
#include <d2d1.h>
#include <D2d1_1.h>
#include <DXGI1_2.h>
#include <D3D11.h>
#include <d2d1effects.h>
#include <d2d1helper.h>
#include <windows.h>
#include <math.h>
#include <Wincodec.h>
#include <Dwrite.h>
#include <string.h>


using namespace D2D1;


////////////////////////////////////////////////////////////////////////////////
// Class name: D2DClass
////////////////////////////////////////////////////////////////////////////////
class D2DClass
{
public:
	D2DClass();
	D2DClass(const D2DClass&);
	~D2DClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	ID2D1DeviceContext* GetRenderTarget();
	D2D1_SIZE_U GetResolution();

	void BeginScene();
	HRESULT EndScene();


private:
	ID2D1DeviceContext* m_Context;
	ID2D1Device* m_Device;
	ID2D1Bitmap1* m_d2dTargetBitmap;
	ID2D1Factory* m_Factory;
	IDXGISwapChain1* m_Swapchain;
	D3D_FEATURE_LEVEL m_featureLevel;
	int scrWidth;
	int scrHeight;
};

#endif