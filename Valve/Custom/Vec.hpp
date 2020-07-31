#pragma once
class Vec {
public:
	float x;
	float y;
	float z;

	//constructors
	Vec() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	Vec(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	//functions
	void Normalize() {
		if (this->x > 89) {
			this->x = 89;
		}
		else if (this->x < -89) {
			this->x = -89;
		}

		while (this->y > 180) {
			this->y -= 360;
		}

		while (this->y < -180) {
			this->y += 360;
		}
	}

	float VecLength() {
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	float VecLength2D() {
		return sqrt(this->x * this->x + this->y * this->y);
	}

	float SquareLength() {
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	bool equals(const Vec& b, float tolerance)
	{
		if (fabsf(this->x - b.x) > tolerance)
			return false;
		if (fabsf(this->y - b.y) > tolerance)
			return false;
		if (fabsf(this->z - b.z) > tolerance)
			return false;
		return true;
	}

	std::string str() {
		return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->z) + ")";
	}

	//operators
	Vec operator + (const Vec& b) {
		return Vec(b.x + this->x, b.y + this->y, b.z + this->z);
	}

	Vec operator - (const Vec& b) {
		return Vec(b.x - this->x, b.y - this->y, b.z - this->z);
	}

	Vec operator * (const int b) {
		return Vec(this->x * b, this->y * b, this->z * b);
	}

	void operator = (const int b) {
		this->x = b;
		this->y = b;
		this->z = b;
	}

	void operator += (const Vec& b) {
		this->x += b.x;
		this->y += b.y;
		this->z += b.z;
	}

	void operator -= (const Vec& b) {
		this->x -= b.x;
		this->y -= b.y;
		this->z -= b.z;
	}

	void operator *= (const int b) {
		this->x *= b;
		this->y *= b;
		this->z *= b;
	}

	void operator *= (const float b) {
		this->x *= b;
		this->y *= b;
		this->z *= b;
	}

	void operator /= (const int b) {
		this->x /= b;
		this->y /= b;
		this->z /= b;
	}

	void operator /= (const float b) {
		this->x /= b;
		this->y /= b;
		this->z /= b;
	}
};

typedef Vec QAngle;