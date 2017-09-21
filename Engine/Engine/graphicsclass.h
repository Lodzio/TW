////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d2dclass.h"
#include "ComCenter.h"
#include "unit.h"
#include "QuadTreeSystem.h"
#include "Commander.h"
#include "Employee.h"
#include "Factory.h"
#include "city.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

struct Modestruct
{
	enum
	{
		JOB_IDLE = 0,
		JOB_PLACING = 1,
		JOB_MOVING = 2,
		JOB_SELECTED = 3,
	};

	UINT JobPrimary;
	UINT JOBSecondary;
	UINT Unit;
	Object* selected;
};

struct GRAPH_INP
{
	Object** Objects;
	int AmmountOfObjects;
	POINT Center;
	int Scroll;
	QuadTreeClass* QuadTree;
	Cityclass* city;
	int FPS;
	SparksClass* Sparks;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(GRAPH_INP input);
	void UpdateState(int X, int Y, int Scroll, POINT Center);
	UINT GetGlowed();
	D2D1_SIZE_U GetMapSize();
	Modestruct Mode;

private:
	bool Render(GRAPH_INP input);
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget *pRenderTarget, PCWSTR uri, ID2D1Bitmap **ppBitmap);
	bool DrawInterface();
	void DrawCursor();
	void ClearScreen(ID2D1RenderTarget* input);
	void DrawObject(Object* input);
	void DrawTextwnum(char* c_str, int num, D2D1_POINT_2F pos, float scale = 1);
	void DrawTextwnum(char* c_str, int num1 ,int num2,  D2D1_POINT_2F pos, float scale = 1);
private:
	D2DClass* m_Direct2D;
	IDWriteTextFormat* TextFormat;
	ID2D1BitmapBrush* BuildingsTexture;
	ID2D1BitmapBrush* UnitTexture;
	ID2D1Bitmap* EffectsTexture;
	ID2D1Effect* ShootingEffect;
	ID2D1BitmapBrush* IconTexture;
	ID2D1BitmapBrush* Map;
	ID2D1BitmapBrush* Cursors;
	D2D1_SIZE_U Map_Text_Size;
	D2D1_SIZE_U Map_Size;
	ID2D1SolidColorBrush* BlackBrush;
	ID2D1SolidColorBrush* GreenBrush;
	ID2D1SolidColorBrush* RedBrush;
	UINT Glowed;
	Object* Placing;
	D2D1_POINT_2U CursorPos;

private:
	D2D1_RECT_F interfaceRecFirst;
	D2D1_RECT_F interfaceRecSecond;
	D2D1_SIZE_F interfaceSizeCommand;
};

#endif