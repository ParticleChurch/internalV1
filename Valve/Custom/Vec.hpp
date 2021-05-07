#pragma once
class Matrix3x4;

class quaternion_t {
public:
	float x, y, z, w;
};

class Vec2 {
public:
	float x;
	float y;

	Vec2() {
		this->x = 0;
		this->y = 0;
	}

	Vec2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vec2 operator + (const Vec2& b) {
		return Vec2(b.x + this->x, b.y + this->y);
	}

	void operator = (const Vec2& b) {
		this->x = b.x;
		this->y = b.y;
	}

	void operator -= (const Vec2& b) {
		this->x -= b.x;
		this->y -= b.y;
	}

	Vec2 operator / (const int b) {
		return Vec2(this->x / b, this->y / b);
	}

	Vec2 operator / (const float b) {
		return Vec2(this->x / b, this->y / b);
	}

	void operator += (const Vec2& b) {
		this->x += b.x;
		this->y += b.y;
	}
};

struct Vertex {
	Vertex() { }
	Vertex(const Vec2& pos, const Vec2& coord = Vec2(0, 0)) {
		position = pos;
		m_TexCoord = coord;
	}
	void Init(const Vec2& pos, const Vec2& coord = Vec2(0, 0)) {
		position = pos;
		m_TexCoord = coord;
	}

	Vec2	position;
	Vec2	m_TexCoord;
};


class Vec {
public:
	float x;
	float y;
	float z;

	constexpr auto operator[](int i) const noexcept 
	{ 
		if (i == 0)
			return x;
		if (i == 1)
			return y;
		if (i == 2)
			return z;
		return 0.f;
	}

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

	Vec(const Vec& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	inline Vec Clone()
	{
		return Vec(*this);
	}

	//functions
	void NormalizeAngle() {
		// pitch
		if (x > 89) x = 89;
		else if (x < -89) x = -89;
		else if (isnan(x)) x = 0;

		// yaw
		if (y > 180 || y <= -180)
			y = isinf(y) ? 0 : fmodf(fmodf(y + 180, 360) + 360, 360) - 180;
		else if (isnan(y))
			y = 0;

		// roll
		z = 0;
	}

	void Normalize()
	{
		if (this->x > 89) this->x = 89;
		else if (this->x < -89) this->x = -89;

		this->y = fmod(this->y + 180.f, 360.f);
		if (this->y < 0.f)
			this->y += 180.f;
		else
			this->y -= 180.f;
	}

	void NormalizeNoClamp()
	{

		x -= floorf(x / 360.0f + 0.5f) * 360.0f;

		y -= floorf(y / 360.0f + 0.5f) * 360.0f;

		z -= floorf(z / 360.0f + 0.5f) * 360.0f;
	}

	inline float VecLength() {
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	inline float VecLength2D() {
		return sqrt(this->x * this->x + this->y * this->y);
	}

	inline float SquareLength() {
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	inline bool equals(const Vec& b, float tolerance)
	{
		if (fabsf(this->x - b.x) > tolerance)
			return false;
		if (fabsf(this->y - b.y) > tolerance)
			return false;
		if (fabsf(this->z - b.z) > tolerance)
			return false;
		return true;
	}

	inline std::string str() {
		return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->z) + ")";
	}


	inline float Dot(const Vec& vOther) const
	{
		return (this->x * vOther.x + this->y * vOther.y + this->z * vOther.z);
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

	Vec operator / (const int b) {
		return Vec(this->x / b, this->y / b, this->z / b);
	}

	Vec operator / (const float b) {
		return Vec(this->x / b, this->y / b, this->z / b);
	}

	Vec Transform(const Matrix3x4& m)
	{
		Vec a = Vec(m.c[0][0], m.c[0][1], m.c[0][2]);
		Vec b = Vec(m.c[1][0], m.c[1][1], m.c[1][2]);
		Vec d = Vec(m.c[2][0], m.c[2][1], m.c[2][2]);
		return Vec(this->Dot(a) + m.c[0][3], this->Dot(b) + m.c[1][3], this->Dot(d) + m.c[2][3]);
	}

	void operator = (const Vec b) {
		this->x = b.x;
		this->y = b.y;
		this->z = b.z;
	}

	void operator = (const int b) {
		this->x = (float)b;
		this->y = (float)b;
		this->z = (float)b;
	}

	void operator = (const float b) {
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

	constexpr auto dotProduct(const Vec& v) const noexcept
	{
		return x * v.x + y * v.y + z * v.z;
	}

	float Dist(const Vec& vOther) const
	{
		Vec delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.VecLength();
	}

	Vec Clamp()
	{
		NormalizeAngle();
		return *this;
	}

	Vec transform(Matrix3x4 mat) const noexcept
	{
		return Vec{ dotProduct({ mat[0][0], mat[0][1], mat[0][2] }) + mat[0][3],
					   dotProduct({ mat[1][0], mat[1][1], mat[1][2] }) + mat[1][3],
					   dotProduct({ mat[2][0], mat[2][1], mat[2][2] }) + mat[2][3] };
	}
};

typedef Vec QAngle;

