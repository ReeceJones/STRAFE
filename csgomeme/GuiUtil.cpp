#include "Gui.h"

GuiUtil *g_pGuiUtil = new GuiUtil;

DWORD GuiUtil::RGBToHex(Color_RGB color)
{	
	DWORD r = color.r;
	DWORD g = color.g;
	DWORD b = color.b;
	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

Color_RGB GuiUtil::HexToRGB(DWORD color)
{
	Color_RGB color_RGB;
	color_RGB.r = ((color >> 16) & 0xFF) / 255.0;
	color_RGB.g = ((color >> 8) & 0xFF) / 255.0;
	color_RGB.b = ((color) & 0xFF) / 255.0;

	return color_RGB;
}