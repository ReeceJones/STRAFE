#pragma once
#include "Structs.h"
#include "Enums.h"
#include "Util.h"

typedef unsigned long HFont;
typedef FontVertex_t Vertex_t;


//define
class IVPanel
{
public:
	const char* GetName(int iPanel)
	{
		typedef const char*(__thiscall* OriginalFn)(void*, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 36)(this, iPanel);
	}
};
//dont define
class Color
{
public:
	Color()
	{
		*((int *)this) = 0;
	}
	Color(int color32)
	{
		*((int *)this) = color32;
	}
	Color(int _r, int _g, int _b)
	{
		SetColor(_r, _g, _b, 255);
	}
	Color(int _r, int _g, int _b, int _a)
	{
		SetColor(_r, _g, _b, _a);
	}

	void SetColor(int _r, int _g, int _b, int _a = 255)
	{
		_color[0] = (unsigned char)_r;
		_color[1] = (unsigned char)_g;
		_color[2] = (unsigned char)_b;
		_color[3] = (unsigned char)_a;
	}

	void GetColor(int &_r, int &_g, int &_b, int &_a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	void SetRawColor(int color32)
	{
		*((int*)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int*)this);
	}

	int GetD3DColor() const
	{
		return ((int)((((_color[3]) & 0xff) << 24) | (((_color[0]) & 0xff) << 16) | (((_color[1]) & 0xff) << 8) | ((_color[2]) & 0xff)));
	}

	inline int r() const { return _color[0]; }
	inline int g() const { return _color[1]; }
	inline int b() const { return _color[2]; }
	inline int a() const { return _color[3]; }

	inline float rBase() const { return _color[0] / 255.0f; }
	inline float gBase() const { return _color[1] / 255.0f; }
	inline float bBase() const { return _color[2] / 255.0f; }
	inline float aBase() const { return _color[3] / 255.0f; }

	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color &rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	bool operator != (const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	Color &operator=(const Color &rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

	float* Base()
	{
		float clr[3];

		clr[0] = _color[0] / 255.0f;
		clr[1] = _color[1] / 255.0f;
		clr[2] = _color[2] / 255.0f;

		return &clr[0];
	}

	float* BaseAlpha()
	{
		float clr[4];

		clr[0] = _color[0] / 255.0f;
		clr[1] = _color[1] / 255.0f;
		clr[2] = _color[2] / 255.0f;
		clr[3] = _color[3] / 255.0f;

		return &clr[0];
	}

	float Hue() const
	{
		if (_color[0] == _color[1] && _color[1] == _color[2])
		{
			return 0.0f;
		}

		float r = _color[0] / 255.0f;
		float g = _color[1] / 255.0f;
		float b = _color[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		float delta = max - min;
		float hue = 0.0f;

		if (r == max)
		{
			hue = (g - b) / delta;
		}
		else if (g == max)
		{
			hue = 2 + (b - r) / delta;
		}
		else if (b == max)
		{
			hue = 4 + (r - g) / delta;
		}
		hue *= 60;

		if (hue < 0.0f)
		{
			hue += 360.0f;
		}
		return hue;
	}

	float Saturation() const
	{
		float r = _color[0] / 255.0f;
		float g = _color[1] / 255.0f;
		float b = _color[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		float l, s = 0;

		if (max != min)
		{
			l = (max + min) / 2;
			if (l <= 0.5f)
				s = (max - min) / (max + min);
			else
				s = (max - min) / (2 - max - min);
		}
		return s;
	}

	float Brightness() const
	{
		float r = _color[0] / 255.0f;
		float g = _color[1] / 255.0f;
		float b = _color[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		return (max + min) / 2;
	}

	static Color FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return Color(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return Color(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

	static Color Red() { return Color(255, 0, 0); }
	static Color Green() { return Color(0, 255, 0); }
	static Color Blue() { return Color(0, 0, 255); }
	static Color LightBlue() { return Color(100, 100, 255); }
	static Color Grey() { return Color(128, 128, 128); }
	static Color DarkGrey() { return Color(45, 45, 45); }
	static Color Black() { return Color(0, 0, 0); }
	static Color White() { return Color(255, 255, 255); }
	static Color Purple() { return Color(220, 0, 220); }


private:
	unsigned char _color[4];
};
//define
class ISurface
{
public:
	void		DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 15)(this, r, g, b, a);
	}
	void		DrawSetColor(Color col)
	{
		typedef void(__thiscall* OriginalFn)(void*, Color);
		return g_pUtil->CallVFunc<OriginalFn>(this, 14)(this, col);
	}
	void		DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 16)(this, x0, y0, x1, y1);
	}
	void		DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 18)(this, x0, y0, x1, y1);
	}
	void		DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 19)(this, x0, y0, x1, y1);
	}
	void		DrawPolyLine(int *px, int *py, int numPoints)
	{
		typedef void(__thiscall* OriginalFn)(void*, int*, int*, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 20)(this, px, py, numPoints);
	}
	void		DrawSetTextFont(HFont font)
	{
		typedef void(__thiscall* OriginalFn)(void*, HFont);
		return g_pUtil->CallVFunc<OriginalFn>(this, 23)(this, font);
	}
	void		DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 25)(this, r, g, b, a);
	}
	void		DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* OriginalFn)(void*, Color);
		return g_pUtil->CallVFunc<OriginalFn>(this, 24)(this, col);
	}
	void		DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 26)(this, x, y);
	}
	void		DrawPrintText(const wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT)
	{
		typedef void(__thiscall* OriginalFn)(void*, const wchar_t*, int, FontDrawType_t);
		return g_pUtil->CallVFunc<OriginalFn>(this, 28)(this, text, textLen, drawType);
	}
	void		DrawSetTextureRGBA(int id, const unsigned char *rgba, int wide, int tall)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, const unsigned char*, int, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 37)(this, id, rgba, wide, tall);
	}
	void		DrawSetTexture(int id)
	{
		typedef void(__thiscall* OriginalFn)(void*, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 38)(this, id);
	}
	int			CreateNewTextureID(bool procedural = false)
	{
		typedef int(__thiscall* OriginalFn)(void*, bool);
		return g_pUtil->CallVFunc<OriginalFn>(this, 43)(this, procedural);
	}
	HFont		Create_Font()
	{
		typedef HFont(__thiscall* OriginalFn)(void*);
		return g_pUtil->CallVFunc<OriginalFn>(this, 71)(this);
	}
	bool		SetFontGlyphSet(HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0)
	{
		typedef bool(__thiscall* OriginalFn)(void*, HFont, const char*, int, int, int, int, int, int, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}
	void		GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* OriginalFn)(void*, HFont, const wchar_t*, int&, int&);
		return g_pUtil->CallVFunc<OriginalFn>(this, 79)(this, font, text, wide, tall);
	}
	void		DrawOutlinedCircle(int x, int y, int radius, int segments)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return g_pUtil->CallVFunc<OriginalFn>(this, 103)(this, x, y, radius, segments);
	}

	void		DrawTexturedPolygon(int n, Vertex_t *pVertice, bool bClipVertices = true)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, Vertex_t*, bool);
		return g_pUtil->CallVFunc<OriginalFn>(this, 106)(this, n, pVertice, bClipVertices);
	}

	int         GetFontTall(HFont font)
	{
		typedef int(__thiscall* OriginalFn)(void*, HFont);
		return g_pUtil->CallVFunc<OriginalFn>(this, 74)(this, font);
	}
};