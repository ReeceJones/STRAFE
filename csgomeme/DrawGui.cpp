#include "Gui.h"

DrawGui* g_pDraw = new DrawGui;

D3DTLVERTEX DrawGui::CreateD3DTLVERTEX(float X, float Y, float Z, float RHW, D3DCOLOR color, float U, float V)
{
	D3DTLVERTEX v;
	v.fX = X;
	v.fY = Y;
	v.fZ = Z;
	v.fRHW = RHW;
	v.Color = color;
	v.fU = U;
	v.fV = V;
	return v;
}

void DrawGui::DrawCircle(GuiVector2 cords, float radius, bool filled, D3DCOLOR color, IDirect3DDevice9 *pd3device)
{
	const int NUMPOINTS = 50;
	const float PI = 3.14159;
	D3DTLVERTEX Circle[NUMPOINTS + 1];
	int i;
	float X;
	float Y;
	float theta;
	float wedgeAngle;

	wedgeAngle = (float)((2 * PI) / NUMPOINTS);

	for (i = 0; i <= NUMPOINTS; i++)
	{
		theta = i * wedgeAngle;
		X = (float)(cords.x + radius * cos(theta));
		Y = (float)(cords.y - radius * sin(theta));
		Circle[i] = CreateD3DTLVERTEX(X, Y, 0.0f, 1.0f, color, 0.0f, 0.0f);
	}
	pd3device->SetFVF(D3DFVF_TL);
	pd3device->SetTexture(0, NULL);
	if (filled)
		pd3device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN/*D3DPT_LINESTRIP*/, NUMPOINTS, &Circle[0], sizeof(Circle[0]));
	else
		pd3device->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &Circle[0], sizeof(Circle[0]));
}

void DrawGui::DrawLine(GuiVector4 cords, D3DCOLOR color, IDirect3DDevice9 *pd3device)
{
	D3DTLVERTEX Line[2];
	Line[0] = CreateD3DTLVERTEX(cords.x1, cords.y1, 0.0f, 1.0f, color, 0.0f, 0.0f);
	Line[1] = CreateD3DTLVERTEX(cords.x2, cords.y2, 0.0f, 1.0f, color, 0.0f, 0.0f);
	pd3device->DrawPrimitiveUP(D3DPT_LINELIST/*D3DPT_LINESTRIP*/, 2, &Line[0], sizeof(Line[0]));
}

void DrawGui::DrawTriangle(GuiVector6 cords, bool filled, D3DCOLOR color, IDirect3DDevice9 *pd3device)
{
	if (!filled)
	{
		GuiVector4 cord1, cord2, cord3;
		/*

		'----------'
		  \       /
		   \     /
			\   /
			 \./

		*/

		cord1.x1 = cords.x1;
		cord1.y1 = cords.y1;
		cord1.x2 = cords.x2;
		cord1.y2 = cords.y2;

		cord2.x1 = cords.x2;
		cord2.y1 = cords.y2;
		cord2.x2 = cords.x3;
		cord2.y2 = cords.y3;

		cord3.x1 = cords.x1;
		cord3.y1 = cords.y1;
		cord3.x2 = cords.x3;
		cord3.y2 = cords.y3;

		this->DrawLine(cord1, color, pd3device);
		this->DrawLine(cord2, color, pd3device);
		this->DrawLine(cord3, color, pd3device);
	}
	else
	{
		D3DTLVERTEX Triangle[3];
		Triangle[0] = CreateD3DTLVERTEX(cords.x1, cords.y1, 0.0f, 1.0f, color, 0.0f, 0.0f);
		Triangle[1] = CreateD3DTLVERTEX(cords.x2, cords.y2, 0.0f, 1.0f, color, 0.0f, 0.0f);
		Triangle[2] = CreateD3DTLVERTEX(cords.x3, cords.y3, 0.0f, 1.0f, color, 0.0f, 0.0f);
		pd3device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &Triangle[0], sizeof(Triangle[0]));
	}
}

void DrawGui::DrawRectangle(GuiVector4 cords, bool filled, D3DCOLOR color, IDirect3DDevice9 *pd3device)
{

	/*
	'-----------------'
	|                 |
	|                 |
	|				  |
	'-----------------'
	*/

	GuiVector4 cord1, cord2, cord3, cord4;
	cord1.x1 = cords.x1; // Do this manually because the constructor is broken for some reason. I'm to lazy to figure out why.
	cord1.y1 = cords.y1;
	cord1.x2 = cords.x2;
	cord1.y2 = cords.y1;

	cord2.x1 = cords.x1;
	cord2.y1 = cords.y1;
	cord2.x2 = cords.x1;
	cord2.y2 = cords.y2;

	cord3.x1 = cords.x1;
	cord3.y1 = cords.y2;
	cord3.x2 = cords.x2;
	cord3.y2 = cords.y2;

	cord4.x1 = cords.x2;
	cord4.y1 = cords.y1;
	cord4.x2 = cords.x2;
	cord4.y2 = cords.y2;

	this->DrawLine(cord1, color, pd3device); //draw the lines
	this->DrawLine(cord2, color, pd3device);
	this->DrawLine(cord3, color, pd3device);
	this->DrawLine(cord4, color, pd3device);

	if (filled) //if filled is true then loop through portion inbetween the lines and draw a line to fill the rect
	{
		int lineDifference = cords.y2 - cords.y1;
		for (int i = 0; i < lineDifference; i++)
		{
			GuiVector4 tempCord;
			tempCord.x1 = cords.x1;
			tempCord.y1 = cords.y1 + i;
			tempCord.x2 = cords.x2;
			tempCord.y2 = cords.y1 + i;

			this->DrawLine(tempCord, color, pd3device);
		}
	}
}

int hexcolor(int r, int g, int b)
{
	return (r << 16) | (g << 8) | b;
}

void DrawGui::DrawCircleGradient(GuiVector2 cords, float radius, bool filled, D3DCOLOR color, D3DCOLOR colorShift, float percentage, IDirect3DDevice9 *pd3device)
{
	int firstDrawLayer = percentage * radius; //calculate some basic stuff needed later
	int shiftedColors = radius - firstDrawLayer;

	this->DrawCircle(cords, radius, filled, color, pd3device); //draw first color layer before conversion

	Color_RGB colorShift_RGB, color_RGB; //convert the input colors into rgb
	color_RGB = g_pGuiUtil->HexToRGB(color);
	colorShift_RGB = g_pGuiUtil->HexToRGB(colorShift);

	Color_RGB colorDifference_RGB; //get the difference between the two input colors so that we can fill the area inbetween the 
	colorDifference_RGB.r = colorShift_RGB.r - color_RGB.r;
	colorDifference_RGB.g = colorShift_RGB.g - color_RGB.g;
	colorDifference_RGB.b = colorShift_RGB.b - color_RGB.b;

	Color_RGB colorIncrement_RGB; //get the amount that we will add to the original color
	colorIncrement_RGB.r = colorDifference_RGB.r - shiftedColors;
	colorIncrement_RGB.g = colorDifference_RGB.g - shiftedColors;
	colorIncrement_RGB.b = colorDifference_RGB.b - shiftedColors;

	D3DCOLOR colorIncrement = g_pGuiUtil->RGBToHex(colorIncrement_RGB); //conver the rgb color back to hex format. D3DCOLOR is a DWORD (typedef DWORD D3DCOLOR;)

	for (int i = 1; i < shiftedColors; i++) //draw the color shift
			this->DrawCircle(cords, radius - i, filled, color + colorIncrement * i, pd3device);

	int startDraw = 1 + shiftedColors;

	for (int i = startDraw; i < radius; i++) //draw the internal part where there is no more need to shift color
		this->DrawCircle(cords, radius - i, filled, colorShift, pd3device);
}

void DrawGui::DrawRectangleGradientIn(GuiVector4 cords, bool filled, D3DCOLOR color, D3DCOLOR colorShift, float percentage, IDirect3DDevice9 *pd3device)
{
	int firstDrawLayer = percentage * (cords.x2 - cords.x1); //calculate some basic stuff needed later
	int shiftedColors = cords.x2 - firstDrawLayer;

	this->DrawRectangle(cords, filled, color, pd3device); //draw first layer

	Color_RGB colorShift_RGB, color_RGB; //convert the input colors into rgb
	color_RGB = g_pGuiUtil->HexToRGB(color);
	colorShift_RGB = g_pGuiUtil->HexToRGB(colorShift);

	Color_RGB colorDifference_RGB; //get the difference between the two input colors so that we can fill the area inbetween the 
	colorDifference_RGB.r = colorShift_RGB.r - color_RGB.r;
	colorDifference_RGB.g = colorShift_RGB.g - color_RGB.g;
	colorDifference_RGB.b = colorShift_RGB.b - color_RGB.b;

	Color_RGB colorIncrement_RGB; //get the amount that we will add to the original color
	colorIncrement_RGB.r = colorDifference_RGB.r - shiftedColors;
	colorIncrement_RGB.g = colorDifference_RGB.g - shiftedColors;
	colorIncrement_RGB.b = colorDifference_RGB.b - shiftedColors;

	D3DCOLOR colorIncrement = g_pGuiUtil->RGBToHex(colorIncrement_RGB); //conver the rgb color back to hex format. D3DCOLOR is a DWORD (typedef DWORD D3DCOLOR;)

	for (int i = 1; i < shiftedColors; i++) //draw the color shift
	{
		GuiVector4 newCords = cords;
		newCords.x1 += i;
		newCords.y1 += i;
		newCords.x2 -= i;
		newCords.y2 -= i;

		this->DrawRectangle(newCords, filled, color + colorIncrement * i, pd3device);
	}

	int startDraw = 1 + shiftedColors;

	for (int i = startDraw; i < ((cords.x1 + cords.x2) / 2); i++) //draw the internal part where there is no more need to shift color
	{
		GuiVector4 newCords;

		this->DrawRectangle(cords, filled, colorShift, pd3device);
	}
}