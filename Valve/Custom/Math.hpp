#pragma once

inline float DotProduct(const float* v1, const float* v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline void SinCos(float radians, float* sine, float* cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

//-----------------------------------------------------------------------------
// Euler QAngle -> Basis Vectors.  Each vector is optional
//-----------------------------------------------------------------------------
inline void AngleVectors(const QAngle& angles, Vec* forward, Vec* right, Vec* up)
{
	float sr, sp, sy, cr, cp, cy;
	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

inline void MatrixCopy(const Matrix3x4& source, Matrix3x4& target)
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 4; ++j)
			target.c[i][j] = source.c[i][j];
}

inline void MatrixSetColumn(const Vec& in, int column, Matrix3x4& out)
{
	out.c[0][column] = in.x;
	out.c[1][column] = in.y;
	out.c[2][column] = in.z;
}

inline void MatrixMultiply(const Matrix3x4& src1, const Matrix3x4& src2, Matrix3x4& dst)
{
	typedef float VMatrixRaw_t[4]; // ok?

	// Make sure it works if src1 == dst or src2 == dst
	Matrix3x4 tmp1, tmp2;
	const VMatrixRaw_t* s1 = (&src1 == &dst) ? tmp1.c : src1.c;
	const VMatrixRaw_t* s2 = (&src2 == &dst) ? tmp2.c : src2.c;

	if (&src1 == &dst)
	{
		MatrixCopy(src1, tmp1);
	}
	if (&src2 == &dst)
	{
		MatrixCopy(src2, tmp2);
	}

	dst.c[0][0] = s1[0][0] * s2[0][0] + s1[0][1] * s2[1][0] + s1[0][2] * s2[2][0] + s1[0][3] * s2[3][0];
	dst.c[0][1] = s1[0][0] * s2[0][1] + s1[0][1] * s2[1][1] + s1[0][2] * s2[2][1] + s1[0][3] * s2[3][1];
	dst.c[0][2] = s1[0][0] * s2[0][2] + s1[0][1] * s2[1][2] + s1[0][2] * s2[2][2] + s1[0][3] * s2[3][2];
	dst.c[0][3] = s1[0][0] * s2[0][3] + s1[0][1] * s2[1][3] + s1[0][2] * s2[2][3] + s1[0][3] * s2[3][3];

	dst.c[1][0] = s1[1][0] * s2[0][0] + s1[1][1] * s2[1][0] + s1[1][2] * s2[2][0] + s1[1][3] * s2[3][0];
	dst.c[1][1] = s1[1][0] * s2[0][1] + s1[1][1] * s2[1][1] + s1[1][2] * s2[2][1] + s1[1][3] * s2[3][1];
	dst.c[1][2] = s1[1][0] * s2[0][2] + s1[1][1] * s2[1][2] + s1[1][2] * s2[2][2] + s1[1][3] * s2[3][2];
	dst.c[1][3] = s1[1][0] * s2[0][3] + s1[1][1] * s2[1][3] + s1[1][2] * s2[2][3] + s1[1][3] * s2[3][3];

	dst.c[2][0] = s1[2][0] * s2[0][0] + s1[2][1] * s2[1][0] + s1[2][2] * s2[2][0] + s1[2][3] * s2[3][0];
	dst.c[2][1] = s1[2][0] * s2[0][1] + s1[2][1] * s2[1][1] + s1[2][2] * s2[2][1] + s1[2][3] * s2[3][1];
	dst.c[2][2] = s1[2][0] * s2[0][2] + s1[2][1] * s2[1][2] + s1[2][2] * s2[2][2] + s1[2][3] * s2[3][2];
	dst.c[2][3] = s1[2][0] * s2[0][3] + s1[2][1] * s2[1][3] + s1[2][2] * s2[2][3] + s1[2][3] * s2[3][3];

	dst.c[3][0] = s1[3][0] * s2[0][0] + s1[3][1] * s2[1][0] + s1[3][2] * s2[2][0] + s1[3][3] * s2[3][0];
	dst.c[3][1] = s1[3][0] * s2[0][1] + s1[3][1] * s2[1][1] + s1[3][2] * s2[2][1] + s1[3][3] * s2[3][1];
	dst.c[3][2] = s1[3][0] * s2[0][2] + s1[3][1] * s2[1][2] + s1[3][2] * s2[2][2] + s1[3][3] * s2[3][2];
	dst.c[3][3] = s1[3][0] * s2[0][3] + s1[3][1] * s2[1][3] + s1[3][2] * s2[2][3] + s1[3][3] * s2[3][3];
}

inline void AngleMatrix(const QAngle& angles, Matrix3x4& matrix)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	// matrix = (YAW * PITCH) * ROLL
	matrix.c[0][0] = cp * cy;
	matrix.c[1][0] = cp * sy;
	matrix.c[2][0] = -sp;

	float crcy = cr * cy;
	float crsy = cr * sy;
	float srcy = sr * cy;
	float srsy = sr * sy;
	matrix.c[0][1] = sp * srcy - crsy;
	matrix.c[1][1] = sp * srsy + crcy;
	matrix.c[2][1] = sr * cp;

	matrix.c[0][2] = (sp * crcy + srsy);
	matrix.c[1][2] = (sp * crsy - srcy);
	matrix.c[2][2] = cr * cp;

	matrix.c[0][3] = 0.0f;
	matrix.c[1][3] = 0.0f;
	matrix.c[2][3] = 0.0f;
}

inline void AngleMatrix(const QAngle& angles, const Vec& position, Matrix3x4& matrix)
{
	AngleMatrix(angles, matrix);
	MatrixSetColumn(position, 3, matrix);
}

inline void MatrixMultiply(Matrix3x4& in1, const Matrix3x4& in2)
{
	Matrix3x4 out;
	if (&in1 == &out)
	{
		Matrix3x4 in1b;
		MatrixCopy(in1, in1b);
		MatrixMultiply(in1b, in2);
		return;
	}
	if (&in2 == &out)
	{
		Matrix3x4 in2b;
		MatrixCopy(in2, in2b);
		MatrixMultiply(in1, in2b);
		return;
	}
	out.c[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	out.c[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	out.c[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	out.c[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
		in1[0][2] * in2[2][3] + in1[0][3];
	out.c[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	out.c[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	out.c[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	out.c[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
		in1[1][2] * in2[2][3] + in1[1][3];
	out.c[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	out.c[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	out.c[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
	out.c[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
		in1[2][2] * in2[2][3] + in1[2][3];

	in1 = out;
}

inline void VectorRotate(const float* in1, const Matrix3x4& in2, float* out)
{
	out[0] = DotProduct(in1, in2[0]);
	out[1] = DotProduct(in1, in2[1]);
	out[2] = DotProduct(in1, in2[2]);
}

inline void VectorRotate(const Vec& in1, const Matrix3x4& in2, Vec& out)
{
	VectorRotate(&in1.x, in2, &out.x);
}

inline void VectorRotate(const Vec& in1, const QAngle& in2, Vec& out)
{
	Matrix3x4 matRotate;
	AngleMatrix(in2, matRotate);
	VectorRotate(in1, matRotate, out);
}

inline void RotateBoneMatrix(Vec AAangle, Vec origin, Matrix3x4* pCustomBoneToWorld, Matrix3x4* BoneMatrix)
{
	Vec BonePos;
	Vec OutPos;
	for (int i = 0; i < MAXSTUDIOBONES; i++)
	{
		AngleMatrix(AAangle, BoneMatrix[i]);
		MatrixMultiply(BoneMatrix[i], pCustomBoneToWorld[i]);
		BonePos = Vec(pCustomBoneToWorld[i][0][3] - origin.x, pCustomBoneToWorld[i][1][3] - origin.y, pCustomBoneToWorld[i][2][3] - origin.z);
		VectorRotate(BonePos, AAangle, OutPos);
		OutPos += origin;
		BoneMatrix[i].c[0][3] = OutPos.x;
		BoneMatrix[i].c[1][3] = OutPos.y;
		BoneMatrix[i].c[2][3] = OutPos.z;
	}
}