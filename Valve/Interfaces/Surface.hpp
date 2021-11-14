#pragma once
class Surface
{
public:
	DECLARE_VIRTUAL_METHOD(void, DrawSetColor, 14, (Color col), (col));
	DECLARE_VIRTUAL_METHOD(void, DrawSetColor, 15, (int r, int g, int b, int a), (r, g, b, a));
	DECLARE_VIRTUAL_METHOD(void, DrawFilledRect, 16, (int x0, int y0, int x1, int y1), (x0, y0, x1, y1));
	DECLARE_VIRTUAL_METHOD(void, DrawOutlinedRect, 18, (int x0, int y0, int x1, int y1), (x0, y0, x1, y1));
	DECLARE_VIRTUAL_METHOD(void, DrawLine, 19, (int x0, int y0, int x1, int y1), (x0, y0, x1, y1));
	DECLARE_VIRTUAL_METHOD(void, DrawPolyLine, 20, (int* x, int* y, int count), (x, y, count));
	DECLARE_VIRTUAL_METHOD(void, DrawSetTextFont, 23, (unsigned long font), (font));
	DECLARE_VIRTUAL_METHOD(void, DrawSetTextColor, 24, (Color col), (col));
	DECLARE_VIRTUAL_METHOD(void, DrawSetTextColor, 25, (int r, int g, int b, int a), (r, g, b, a));
	DECLARE_VIRTUAL_METHOD(void, DrawSetTextPos, 26, (int x, int y), (x, y));
	DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(void, DrawPrintText, 28, (const wchar_t* text, int textLen), (const wchar_t*, int, int), (text, textLen, 0));
	DECLARE_VIRTUAL_METHOD(int, DrawGetTextureId, 34, (const char* filename), (filename));
	DECLARE_VIRTUAL_METHOD(void, DrawSetTextureFile, 36, (int id, const char* fileName, int hardwareFilter, bool forceReload), (id, fileName, hardwareFilter, forceReload));
	DECLARE_VIRTUAL_METHOD(void, DrawSetTextureRGBA, 37, (int id, const unsigned char* rgba, int wide, int tall), (id, rgba, wide, tall));
	DECLARE_VIRTUAL_METHOD(void, DrawSetTexture, 38, (int textureID), (textureID));
	DECLARE_VIRTUAL_METHOD(void, DrawTexturedRect, 41, (int x0, int y0, int x1, int y1), (x0, y0, x1, y1));
	DECLARE_VIRTUAL_METHOD(bool, IsTextureIDValid, 42, (int id), (id));
	DECLARE_VIRTUAL_METHOD(int, CreateNewTextureID, 43, (bool procedural), (procedural));
	DECLARE_VIRTUAL_METHOD(void, GetScreenSize, 44, (int& width, int& height), (width, height));
	DECLARE_VIRTUAL_METHOD(unsigned long, FontCreate, 71, (), ());
	DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(void, SetFontGlyphSet, 72, (unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags), (unsigned long, const char*, int, int, int, int, int, int, int), (font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0));
	DECLARE_VIRTUAL_METHOD(void, AddCustomFontFile, 73, (const char* fontFileName), (fontFileName));
	DECLARE_VIRTUAL_METHOD(void, GetTextSize, 79, (unsigned long font, const wchar_t* text, int& wide, int& tall), (font, text, wide, tall));
	DECLARE_VIRTUAL_METHOD(void, PlaySoundFile, 82, (const char* filename), (filename));
	DECLARE_VIRTUAL_METHOD(void, DrawOutlinedCircle, 103, (int x, int y, int r, int seg), (x, y, r, seg));
	DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(void, DrawPolygon, 106, (int n, Vertex* vertice, bool clip_vertices = true), (int, Vertex*, bool), (n, vertice, clip_vertices));

	/*
	void SurfaceGetCursorPos(int& x, int& y)
	{
		typedef void(__thiscall* oSurfaceGetCursorPos)(PVOID, int&, int&);
		return VMT::GetVirtualMethod< oSurfaceGetCursorPos >(this, 66)(this, x, y);
	}
	*/

	DECLARE_VIRTUAL_METHOD(void, UnlockCursor, 66, (), ());
	DECLARE_VIRTUAL_METHOD(void, LockCursor, 67, (), ());

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