#pragma once
class Color
{
public:

	unsigned char color[4];

	Color()
	{
		color[0] = 255;
		color[1] = 255;
		color[2] = 255;
		color[3] = 255;
	}

	Color(float r, float g, float b, float a = 255.00f)
	{
		color[0] = (unsigned char) r;
		color[1] = (unsigned char) g;
		color[2] = (unsigned char) b;
		color[3] = (unsigned char) a;
	}

	float* Base()
	{
		float clr[3];

		clr[0] = color[0] / 255.0f;
		clr[1] = color[1] / 255.0f;
		clr[2] = color[2] / 255.0f;

		return &clr[0];
	}

	inline float r() {
		return color[0];
	}

	inline float g() {
		return color[1];
	}

	inline float b() {
		return color[2];
	}

	inline float a() {
		return color[3];
	}

};