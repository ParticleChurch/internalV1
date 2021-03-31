#pragma once
class Surface
{
public:
	void DrawSetColor(Color col)
	{
		//DrawSetColor(col.r, col.g, col.b, col.a);
		typedef void(__thiscall* oDrawSetColor)(void*, Color);
		return GetVFunc< oDrawSetColor >(this, 14)(this, col);
	}

	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		GetVFunc< OriginalFn >(this, 15)(this, r, g, b, a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		GetVFunc< OriginalFn >(this, 16)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		GetVFunc< OriginalFn >(this, 18)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		GetVFunc< OriginalFn >(this, 19)(this, x0, y0, x1, y1);
	}

	void DrawPolyLine(int* x, int* y, int count)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int*, int*, int);
		GetVFunc< OriginalFn >(this, 20)(this, x, y, count);
	}

	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
		GetVFunc< OriginalFn >(this, 23)(this, font);
	}

	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		GetVFunc< OriginalFn >(this, 25)(this, r, g, b, a);
	}

	void DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, Color);
		return GetVFunc< oDrawSetTextColor >(this, 24)(this, col);
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		GetVFunc< OriginalFn >(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t* text, int textLen)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t*, int, int);
		GetVFunc< OriginalFn >(this, 28)(this, text, textLen, 0);
	}

	int DrawGetTextureId(const char* filename)
	{
		typedef int(__thiscall* oDrawGetTextureId)(void*, const char*);
		return GetVFunc<oDrawGetTextureId>(this, 34)(this, filename);
	}

	void DrawSetTextureFile(int id, const char* fileName, int hardwareFilter, bool forceReload)
	{
		typedef void(__thiscall* oDrawSetTextureFile)(void*, int, const char*, int, bool);
		GetVFunc<oDrawSetTextureFile>(this, 36)(this, id, fileName, hardwareFilter, forceReload);
	}

	void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, const unsigned char*, int, int);
		return GetVFunc<OriginalFn>(this, 37)(this, id, rgba, wide, tall);
	}

	void DrawSetTexture(int textureID)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, int);
		return GetVFunc< oDrawSetTextColor >(this, 38)(this, textureID); // --
	}

	void DrawTexturedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* oDrawTexturedRect)(void*, int, int, int, int);
		return GetVFunc<oDrawTexturedRect>(this, 41)(this, x0, y0, x1, y1); // --
	}

	bool IsTextureIDValid(int id)
	{
		typedef bool(__thiscall* oIsTextureIDValid)(void*, int);
		return GetVFunc<oIsTextureIDValid>(this, 42)(this, id); // --
	}

	int CreateNewTextureID(bool procedural)
	{
		typedef int(__thiscall* oDrawSetTextColor)(PVOID, bool);
		return GetVFunc< oDrawSetTextColor >(this, 43)(this, procedural); // --
	}

	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* oGetScreenSize)(void*, int&, int&);
		return GetVFunc<oGetScreenSize>(this, 44)(this, width, height);
	}

	unsigned long FontCreate()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return GetVFunc< OriginalFn >(this, 71)(this);
	}

	void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		GetVFunc< OriginalFn >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	bool AddCustomFontFile(const char* fontFileName)
	{
		typedef bool(__thiscall* oAddCustomFontFile)(void*, const char*);
		return GetVFunc<oAddCustomFontFile>(this, 73)(this, fontFileName);
	}

	void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t*, int&, int&);
		GetVFunc< OriginalFn >(this, 79)(this, font, text, wide, tall);
	}

	void PlaySoundFile(const char* filename)
	{
		typedef void(__thiscall* oPlaySound)(PVOID, const char*);
		return GetVFunc< oPlaySound >(this, 82)(this, filename);
	}

	void DrawOutlinedCircle(int x, int y, int r, int seg)
	{
		typedef void(__thiscall* oDrawOutlinedCircle)(PVOID, int, int, int, int);
		return GetVFunc< oDrawOutlinedCircle >(this, 103)(this, x, y, r, seg);
	}

	void DrawPolygon(int n, Vertex* vertice, bool clip_vertices = true)
	{
		typedef void(__thiscall* oDrawPolygon)(PVOID, int, Vertex*, bool);
		return GetVFunc< oDrawPolygon >(this, 106)(this, n, vertice, clip_vertices);
	}

	/*
	void SurfaceGetCursorPos(int& x, int& y)
	{
		typedef void(__thiscall* oSurfaceGetCursorPos)(PVOID, int&, int&);
		return GetVFunc< oSurfaceGetCursorPos >(this, 66)(this, x, y);
	}
	*/

	void UnlockCursor()
	{
		typedef void(__thiscall* ounlockCursor)(PVOID);
		return GetVFunc< ounlockCursor>(this, 66)(this);
	}

	void LockCursor()
	{
		typedef void(__thiscall* olockCursor)(PVOID);
		return GetVFunc< olockCursor>(this, 67)(this);
	}

	void add_textured_polygon(int n, Vertex* vertice, int r, int g, int b, int a) {
		static int texture_id = CreateNewTextureID(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		DrawSetColor(r, g, b, a);
		DrawSetTexture(texture_id);
		DrawPolygon(n, vertice);
	}

	void draw_filled_triangle(std::array< Vec2, 3 >points, Color colour) {
		std::array< Vertex, 3 >vertices{ Vertex(points.at(0)), Vertex(points.at(1)), Vertex(points.at(2)) };
		add_textured_polygon(3, vertices.data(), colour.r(), colour.g(), colour.b(), colour.a());
	}
};