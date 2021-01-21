#pragma once
class IVDebugOverlay
{
protected:
	~IVDebugOverlay() = default;
public:
	virtual void AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void AddBoxOverlay(const Vec & origin, const Vec& mins, const Vec& max, Vec const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void AddSphereOverlay(const Vec& origin, float radius, int theta, int phi, int r, int g, int b, int a, float duration) = 0;
	virtual void AddTriangleOverlay(const Vec& p1, const Vec& p2, const Vec& p3, int r, int g, int b, int a, bool no_depth_test, float duration) = 0;
private:
	virtual void a() = 0;
public:
	virtual void AddLineOverlay(const Vec& origin, const Vec& dest, int r, int g, int b, bool no_depth_test, float duration) = 0;

};