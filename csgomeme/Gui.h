#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#define D3DFVF_TL (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct D3DTLVERTEX
{
	float fX;
	float fY;
	float fZ;
	float fRHW;
	D3DCOLOR Color;
	float fU;
	float fV;
};

struct Color_RGB
{
	float r;
	float g;
	float b;
};

struct GuiVector2
{
	GuiVector2() {};
	GuiVector2(int _x, int _y) { x = _x; x = _y; }
	int x;
	int y;
};



struct GuiVector4
{
	GuiVector4() {};
	GuiVector4(int _x1, int _y1, int _x2, int _y2) { x1 = _x1; y1 = _y1; x2 = _x2; x2 = _x2; }
	int x1;
	int y1;
	int x2;
	int y2;
};

struct GuiVector6
{
	GuiVector6() {};
	GuiVector6(int _x1, int _y1, int _x2, int _y2, int _x3, int _y3) { x1 = _x1; y1 = _y1; x2 = _x2; y2 = _y2; x3 = _x3; y3 = _y3; }
	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;
};

class GuiUtil
{
public:
	DWORD RGBToHex(Color_RGB color);
	Color_RGB HexToRGB(DWORD color);
};
extern GuiUtil* g_pGuiUtil;

class DrawGui
{
public:
	D3DTLVERTEX CreateD3DTLVERTEX(float X, float Y, float Z, float RHW, D3DCOLOR color, float U, float V);
	void DrawCircle(GuiVector2 cords, float radius, bool filled, D3DCOLOR color, IDirect3DDevice9 *pd3device);
	void DrawLine(GuiVector4 cords, D3DCOLOR color, IDirect3DDevice9 *pd3device);
	void DrawTriangle(GuiVector6 cords, bool filled, D3DCOLOR color, IDirect3DDevice9 *pd3device);
	void DrawRectangle(GuiVector4 cords, bool filled, D3DCOLOR color, IDirect3DDevice9 *pd3device);

	void DrawCircleGradient(GuiVector2 cords, float radius, bool filled, D3DCOLOR color, D3DCOLOR colorShift, float percentage, IDirect3DDevice9 *pd3device);
	void DrawRectangleGradientIn(GuiVector4 cords, bool filled, D3DCOLOR color, D3DCOLOR colorShift, float percentage, IDirect3DDevice9 *pd3device);
};
extern DrawGui* g_pDraw;