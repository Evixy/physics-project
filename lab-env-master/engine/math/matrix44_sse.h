/*
#pragma once
#include <cmath>

#include <iostream>
#include "vector4.h"
#define PI 3.14159265f

namespace Math
{

typedef ALIGN16 struct mm128_mat
{
	union
	{
		mm128_vec r[4];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
} mm128_mat;

class vector4;

class matrix44_sse
{

public:
	//Constructor
	matrix44_sse();

	matrix44_sse(const mm128_mat& m);
	matrix44_sse(const vector4 &row0, const vector4 &row1, const vector4 &row2, const vector4 &row3);
	void operator= (const matrix44_sse& m);
	void operator= (const mm128_mat& m);
	//Destructor
	~matrix44_sse();
	static matrix44_sse transMat(vector4 & vec);
	//Sets a unit matrix to have elements (1,4) = x, (2,4) = y and (3,4) = z as values. 
	static matrix44_sse transMat(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	static matrix44_sse matrixFrom_Trans_Rot_Scale(vector4 & trans, vector4 & rot, vector4 & scale);

	static matrix44_sse genProjMatrix(float fov, float aspect, float near, float far);

	static matrix44_sse scaleMatFromVect(vector4 & scale);

	static matrix44_sse quaternionToMatrix(vector4& q);
	static vector4 matrixToQuaternion(matrix44_sse& mat);

	float* get();

	//Returns rotation matrices for rotation around an axis or around a vector. Angle is in radians
	static matrix44_sse rotation_x(float angle, bool isDegrees = false);
	static matrix44_sse rotation_y(float angle, bool isDegrees = false);
	static matrix44_sse rotation_z(float angle, bool isDegrees = false);
	static matrix44_sse rotation_vect(const float& angle, vector4& vect);

	static vector4 transform(vector4& v, matrix44_sse& m);

	//Transposes a matrix
	

	//Inverts a matrix if it is possible, otherwise false will be returned
	matrix44_sse inverse(const matrix44_sse& m);

	//Multiplication with a vector. Vector must be multiplied from the right. 
	vector4 operator* (vector4& vect);

	//Multiplication with matrices
	matrix44_sse operator* (matrix44_sse& matrix);
	static matrix44_sse multiply(matrix44_sse& m1, matrix44_sse& m2);
	
	

	vector4 getPos();
	void setPos(vector4& pos);
	//Changing values. Returns a vector4 which then uses the [] operator in vector4. 
	//Usage:
	//-------------------
	//matrix44_sse matrix;
	//matrix[0][0] = 1.0f; 
	//-------------------
	//Will set the value at position (1,1) in the matrix to 1.0
	float* operator[](unsigned int index);
	matrix44_sse transpose(const matrix44_sse& m);
	//For printing a matrix
	friend std::ostream& operator<< (std::ostream& o, matrix44_sse& matrix);
private:

	//float matrix[4][4];
	mm128_mat mat;
};

///Constructor
inline matrix44_sse::matrix44_sse()
{
	this->mat.r[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
	this->mat.r[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
	this->mat.r[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
	this->mat.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
	*/
/*
	vector4 vect1(1.0f, 0, 0, 0);
	vector4 vect2(0, 1.0f, 0, 0);
	vector4 vect3(0, 0, 1.0f, 0);
	vector4 vect4(0, 0, 0, 1.0f);
	this->matrix[0] = vect1;
	this->matrix[1] = vect2;
	this->matrix[2] = vect3;
	this->matrix[3] = vect4;
	*//*

}

inline matrix44_sse::matrix44_sse(const mm128_mat& m):
	mat(m)
{
	
}

inline matrix44_sse::matrix44_sse(const vector4& r0, const vector4& r1, const vector4& r2, const vector4& r3)
{
	this->mat.r[0].vec = r0.vec;
	this->mat.r[1].vec = r1.vec;
	this->mat.r[2].vec = r2.vec;
	this->mat.r[3].vec = r3.vec;
}

inline void matrix44_sse::operator=(const matrix44_sse& m)
{
	this->mat = m.mat;
}

inline void matrix44_sse::operator=(const mm128_mat& m)
{
	this->mat = m;
}

///Destructor
inline matrix44_sse::~matrix44_sse()
{
	//- empty
}

inline matrix44_sse matrix44_sse::transMat(vector4 & vec)
{
	matrix44_sse mat;

	mat[3][0] = vec[0];
	mat[3][1] = vec[1];
	mat[3][2] = vec[2];

	return mat;
}

inline matrix44_sse matrix44_sse::transMat(float x, float y, float z)
{
	matrix44_sse mat;

	mat[3][0] = x;
	mat[3][1] = y;
	mat[3][2] = z;

	return mat;
}

inline matrix44_sse matrix44_sse::matrixFrom_Trans_Rot_Scale(vector4 & pos, vector4 & rot, vector4 & scale)
{
	matrix44_sse transmat;
	matrix44_sse scalemat;
	matrix44_sse rotmat;
	//trans
	transmat = transMat(pos);
	
	//scale
	scalemat[0][0] = scale[0];
	scalemat[1][1] = scale[1];
	scalemat[2][2] = scale[2];
	scalemat[3][3] = 1;

	//rot
	rotmat = quaternionToMatrix(rot);
	
	matrix44_sse final1 = transmat * rotmat * scalemat;
	matrix44_sse final2 = scalemat * rotmat * transmat;
	return final2;
}

inline matrix44_sse matrix44_sse::genProjMatrix(float fov, float aspect, float near, float far)
{
	matrix44_sse projMatrix;	//Creates a unit matrix;
	float toRadians = float(PI / 180);

	float y_scale, x_scale, frustum_length;
	y_scale = (1.0f / tan(toRadians * (fov / 2.0f))) * aspect;
	x_scale = y_scale / aspect;
	frustum_length = far - near;

	projMatrix[0][0] = x_scale;

	projMatrix[1][1] = y_scale;

	projMatrix[2][2] = -((far + near) / frustum_length);
	projMatrix[2][3] = -1.0f;

	projMatrix[3][2] = -((2.0f* near * far) / frustum_length);
	projMatrix[3][3] = 0.0f;
	projMatrix = projMatrix.transpose(projMatrix);
	return projMatrix;
}

inline matrix44_sse matrix44_sse::scaleMatFromVect(vector4 & scale)
{
	matrix44_sse temp;

	temp[0][0] = scale[0];
	temp[1][1] = scale[1];
	temp[2][2] = scale[2];

	return temp;
}

inline matrix44_sse matrix44_sse::quaternionToMatrix(vector4& q)
{
	matrix44_sse mat;
		
	float x = q[0], y = q[1], z = q[2], w = q[3];
	
	mat[0][0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
	mat[1][0] = 2.0f * x * y + 2.0f * z * w;
	mat[2][0] = 2.0f * x * z - 2.0f * y * w;
	mat[3][0] = 0;

	mat[0][1] = 2.0f * x * y - 2.0f * z * w;
	mat[1][1] = 1.0f - 2.0f * x * x - 2.0f * z * z;
	mat[2][1] = 2.0f * y * z + 2.0f * x * w;
	mat[3][1] = 0;

	mat[0][2] = 2.0f * x * z + 2.0f * y * w;
	mat[1][2] = 2.0f * y * z - 2.0f * x * w;
	mat[2][2] = 1.0f - 2.0f * x * x - 2.0f * y * y;
	mat[3][2] = 0;

	mat[0][3] = 0;
	mat[1][3] = 0;
	mat[2][3] = 0;
	mat[3][3] = 1;

	mat = mat.transpose();

	return mat;
}

inline vector4 matrix44_sse::matrixToQuaternion(matrix44_sse& mat)
{
	float qw = sqrtf(1 + mat[0][0] + mat[1][1] + mat[2][2]) * 0.5f;
	float qx = (mat[2][1] - mat[1][2]) / (4 * qw);
	float qy = (mat[0][2] - mat[2][0]) / (4 * qw);
	float qz = (mat[1][0] - mat[0][1]) / (4 * qw);
	return vector4(qx, qy, qz, qw);
}

inline float* matrix44_sse::get()
{
	return reinterpret_cast<float*>(mat.r);
}


inline vector4 matrix44_sse::getPos()
{
	return vector4(this->mat.r[3]);
}

inline void matrix44_sse::setPos(vector4& pos)
{
	this->mat.r[3] = pos.vec;
}

///Multiplication with another Matrix
inline matrix44_sse matrix44_sse::operator* (matrix44_sse& matrix)
{
	return this->multiply(*this, matrix);	
}

inline matrix44_sse matrix44_sse::multiply(matrix44_sse& m1, matrix44_sse& m2)
{
	matrix44_sse ret;
	vector4 mx; 
	vector4 my;
	vector4 mz;
	vector4 mw = m1.mat.r[0];

	// Splat the all components of the first row
	mx = vector4::splat_x(mw);
	my = vector4::splat_y(mw);
	mz = vector4::splat_z(mw);
	mw = vector4::splat_w(mw);

	vector4 m1x = m1.mat.r[0];
	vector4 m1y = m1.mat.r[1];
	vector4 m1z = m1.mat.r[2];
	vector4 m1w = m1.mat.r[3];

	//multiply first row
	mx = vector4::multiply(mx, m1x);
	my = vector4::multiply(my, m1y);
	mz = vector4::multiply(mz, m1z);
	mw = vector4::multiply(mw, m1w);

	mx = mx + my;
	mz = mz + mw;
	ret.mat.r[0] = (mx + mz).vec;

	// rinse and repeat
	mw = m1.mat.r[0];

	mx = vector4::splat_x(mw);
	my = vector4::splat_y(mw);
	mz = vector4::splat_z(mw);
	mw = vector4::splat_w(mw);

	mx = vector4::multiply(mx, m1x);
	my = vector4::multiply(my, m1y);
	mz = vector4::multiply(mz, m1z);
	mw = vector4::multiply(mw, m1w);

	mx = mx + my;
	mz = mz + mw;
	ret.mat.r[1] = (mx + mz).vec;

	mw = m1.mat.r[1];

	mx = vector4::splat_x(mw);
	my = vector4::splat_y(mw);
	mz = vector4::splat_z(mw);
	mw = vector4::splat_w(mw);

	mx = vector4::multiply(mx, m2.mat.r[0]);
	my = vector4::multiply(my, m2.mat.r[1]);
	mz = vector4::multiply(mz, m2.mat.r[2]);
	mw = vector4::multiply(mw, m2.mat.r[3]);

	mx = mx + my;
	mz = mz + mw;
	ret.mat.r[2] = (mx + mz).vec;

	mw = m1.mat.r[2];

	mx = vector4::splat_x(mw);
	my = vector4::splat_y(mw);
	mz = vector4::splat_z(mw);
	mw = vector4::splat_w(mw);

	mx = vector4::multiply(mx, m1x);
	my = vector4::multiply(my, m1y);
	mz = vector4::multiply(mz, m1z);
	mw = vector4::multiply(mw, m1w);

	mx = mx + my;
	mz = mz + mw;
	ret.mat.r[3] = (mx + mz).vec;

	return ret;
}

///Method for printing a specific matrix. Purely for bug-testing
inline std::ostream& operator<< (std::ostream& o, matrix44_sse& matrix)
{
	for (int i = 0; i < 4; i++)
	{
		o << "[";
		o << matrix[i][0];
		for (int j = 1; j < 4; j++)
		{
			o << ", " << matrix[i][j];
		}
		o << "]" << std::endl;
	}
	std::cout << std::endl;
	return o;
}

///Returns a rotation matrix for rotation around the x-axis
inline matrix44_sse matrix44_sse::rotation_x(float angle, bool isDegrees)
{
	if (isDegrees)
	{
		angle *= (PI/ 180.0f);
	}
	matrix44_sse m;
	
	float sangle = sin(angle);
	float cangle = cos(angle);

	m.mat.m[1][1] = cangle;
	m.mat.m[1][2] = sangle;

	m.mat.m[2][1] = -sangle;
	m.mat.m[2][2] = cangle;
	return m;
}

///Returns a rotation matrix for rotation around the y-axis
inline matrix44_sse matrix44_sse::rotation_y(float angle, bool isDegrees)
{
	if (isDegrees)
	{
		angle *= (PI / 180.0f);
	}
	matrix44_sse m;

	float sangle = sin(angle);
	float cangle = cos(angle);

	m.mat.m[0][0] = cangle;
	m.mat.m[0][2] = -sangle;

	m.mat.m[2][0] = sangle;
	m.mat.m[2][2] = cangle;
	return m;
}

///Returns a rotation matrix for rotation around the z-axis
inline matrix44_sse matrix44_sse::rotation_z(float angle, bool isDegrees)
{
	if(isDegrees)
	{
		angle *= (PI / 180.0f);
	}
	matrix44_sse m;

	float sangle = sin(angle);
	float cangle = cos(angle);

	m.mat.m[0][0] = cangle;
	m.mat.m[0][1] = sangle;

	m.mat.m[1][0] = -sangle;
	m.mat.m[1][1] = cangle;
	return m;
}

///Returns a rotation matrix for rotation around a vector
inline matrix44_sse matrix44_sse::rotation_vect(const float& angle, vector4& vect)
{
	matrix44_sse temp;
	float s = sin(angle);
	float c = cos(angle);
	float x = vect[0];
	float y = vect[1];
	float z = vect[2];
	temp[0][0] = c + x * x * (1.0f - c);
	temp[0][1] = x * y * (1.0f - c) - z * s;
	temp[0][2] = x * z * (1.0f - c) + y * s;
	temp[0][3] = 0;

	temp[1][0] = y * x * (1.0f - c) + z * s;
	temp[1][1] = c + y * y * (1.0f - c);
	temp[1][2] = y * z * (1.0f - c) - x * s;
	temp[1][3] = 0;

	temp[2][0] = z * x * (1.0f - c) - y * s;
	temp[2][1] = z * y * (1.0f - c) + x * s;
	temp[2][2] = c + z * z *(1.0f - c);
	temp[2][3] = 0;

	temp[3][0] = 0;
	temp[3][1] = 0;
	temp[3][2] = 0;
	temp[3][3] = 1.0f;
	return temp;
}

inline vector4 matrix44_sse::transform(vector4& v, matrix44_sse& m)
{
	__m128 x = _mm_shuffle_ps(v.vec.vec, v.vec.vec, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 y = _mm_shuffle_ps(v.vec.vec, v.vec.vec, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 z = _mm_shuffle_ps(v.vec.vec, v.vec.vec, _MM_SHUFFLE(2, 2, 2, 2));
	__m128 w = _mm_shuffle_ps(v.vec.vec, v.vec.vec, _MM_SHUFFLE(3, 3, 3, 3));

	return _mm_add_ps(
		_mm_add_ps(_mm_mul_ps(x, m.mat.r[0].vec), _mm_mul_ps(y, m.mat.r[1].vec)),
		_mm_add_ps(_mm_mul_ps(z, m.mat.r[2].vec), _mm_mul_ps(w, m.mat.r[3].vec))
	);
}

///Used to set values for the matrices.
inline float* matrix44_sse::operator[](unsigned int index)
{
	return this->matrix[index];
}

///Transposes the specified matrix
inline matrix44_sse matrix44_sse::transpose(const matrix44_sse& m)
{
	__m128 xy1 = _mm_shuffle_ps(m.mat.r[0].vec, m.mat.r[1].vec, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 zw1 = _mm_shuffle_ps(m.mat.r[0].vec, m.mat.r[1].vec, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 xy2 = _mm_shuffle_ps(m.mat.r[2].vec, m.mat.r[3].vec, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 zw2 = _mm_shuffle_ps(m.mat.r[2].vec, m.mat.r[3].vec, _MM_SHUFFLE(3, 2, 3, 2));

	matrix44_sse r;
	r.mat.r[0].vec = _mm_shuffle_ps(xy1, xy2, _MM_SHUFFLE(2, 0, 2, 0));
	r.mat.r[1].vec = _mm_shuffle_ps(xy1, xy2, _MM_SHUFFLE(3, 1, 3, 1));
	r.mat.r[2].vec = _mm_shuffle_ps(zw1, zw2, _MM_SHUFFLE(2, 0, 2, 0));
	r.mat.r[3].vec = _mm_shuffle_ps(zw1, zw2, _MM_SHUFFLE(3, 1, 3, 1));
	return r;
}

#define _mm_ror_ps(vec,i)	\
	(((i)%4) ? (_mm_shuffle_ps(vec,vec, _MM_SHUFFLE((unsigned char)(i+3)%4,(unsigned char)(i+2)%4,(unsigned char)(i+1)%4,(unsigned char)(i+0)%4))) : (vec))
///Creates the inverse of a matrix A using reference provided the determinant of A is != 0. If det A = 0 it will return false
inline matrix44_sse matrix44_sse::inverse(const matrix44_sse& m)
{
	__m128 Va, Vb, Vc;
	__m128 r1, r2, r3, tt, tt2;
	__m128 sum, Det, RDet;
	__m128 trns0, trns1, trns2, trns3;

	const mm128_ivec pnpn = { 0x00000000, static_cast<int>(0x80000000), 0x00000000, static_cast<int>(0x80000000) };
	const mm128_ivec npnp = { static_cast<int>(0x80000000), 0x00000000, static_cast<int>(0x80000000), 0x00000000 };
	const mm128_vec zeroone = { 1.0f, 0.0f, 0.0f, 1.0f };

	__m128 _L1 = m.mat.r[0];
	__m128 _L2 = m.mat.r[1];
	__m128 _L3 = m.mat.r[2];
	__m128 _L4 = m.mat.r[3];
	// Calculating the minterms for the first line.

	// _mm_ror_ps is just a macro using _mm_shuffle_ps().
	tt = _L4; tt2 = _mm_ror_ps(_L3, 1);
	Vc = _mm_mul_ps(tt2, _mm_ror_ps(tt, 0));					// V3'dot V4
	Va = _mm_mul_ps(tt2, _mm_ror_ps(tt, 2));					// V3'dot V4"
	Vb = _mm_mul_ps(tt2, _mm_ror_ps(tt, 3));					// V3' dot V4^

	r1 = _mm_sub_ps(_mm_ror_ps(Va, 1), _mm_ror_ps(Vc, 2));		// V3" dot V4^ - V3^ dot V4"
	r2 = _mm_sub_ps(_mm_ror_ps(Vb, 2), _mm_ror_ps(Vb, 0));		// V3^ dot V4' - V3' dot V4^
	r3 = _mm_sub_ps(_mm_ror_ps(Va, 0), _mm_ror_ps(Vc, 1));		// V3' dot V4" - V3" dot V4'

	tt = _L2;
	Va = _mm_ror_ps(tt, 1);		sum = _mm_mul_ps(Va, r1);
	Vb = _mm_ror_ps(tt, 2);		sum = _mm_add_ps(sum, _mm_mul_ps(Vb, r2));
	Vc = _mm_ror_ps(tt, 3);		sum = _mm_add_ps(sum, _mm_mul_ps(Vc, r3));

	// Calculating the determinant.
	Det = _mm_mul_ps(sum, _L1);
	Det = _mm_add_ps(Det, _mm_movehl_ps(Det, Det));


	__m128 mtL1 = _mm_xor_ps(sum, pnpn);

	// Calculating the minterms of the second line (using previous results).
	tt = _mm_ror_ps(_L1, 1);		sum = _mm_mul_ps(tt, r1);
	tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r2));
	tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r3));
	__m128 mtL2 = _mm_xor_ps(sum, npnp);

	// Testing the determinant.
	Det = _mm_sub_ss(Det, _mm_shuffle_ps(Det, Det, 1));

	// Calculating the minterms of the third line.
	tt = _mm_ror_ps(_L1, 1);
	Va = _mm_mul_ps(tt, Vb);									// V1' dot V2"
	Vb = _mm_mul_ps(tt, Vc);									// V1' dot V2^
	Vc = _mm_mul_ps(tt, _L2);								// V1' dot V2

	r1 = _mm_sub_ps(_mm_ror_ps(Va, 1), _mm_ror_ps(Vc, 2));		// V1" dot V2^ - V1^ dot V2"
	r2 = _mm_sub_ps(_mm_ror_ps(Vb, 2), _mm_ror_ps(Vb, 0));		// V1^ dot V2' - V1' dot V2^
	r3 = _mm_sub_ps(_mm_ror_ps(Va, 0), _mm_ror_ps(Vc, 1));		// V1' dot V2" - V1" dot V2'

	tt = _mm_ror_ps(_L4, 1);		sum = _mm_mul_ps(tt, r1);
	tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r2));
	tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r3));
	__m128 mtL3 = _mm_xor_ps(sum, pnpn);

	// Dividing is FASTER than rcp_nr! (Because rcp_nr causes many register-memory RWs).
	RDet = _mm_div_ss(zeroone, Det); // TODO: just 1.0f?
	RDet = _mm_shuffle_ps(RDet, RDet, 0x00);

	// Devide the first 12 minterms with the determinant.
	mtL1 = _mm_mul_ps(mtL1, RDet);
	mtL2 = _mm_mul_ps(mtL2, RDet);
	mtL3 = _mm_mul_ps(mtL3, RDet);

	// Calculate the minterms of the forth line and devide by the determinant.
	tt = _mm_ror_ps(_L3, 1);		sum = _mm_mul_ps(tt, r1);
	tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r2));
	tt = _mm_ror_ps(tt, 1);		sum = _mm_add_ps(sum, _mm_mul_ps(tt, r3));
	__m128 mtL4 = _mm_xor_ps(sum, npnp);
	mtL4 = _mm_mul_ps(mtL4, RDet);

	// Now we just have to transpose the minterms matrix.
	trns0 = _mm_unpacklo_ps(mtL1, mtL2);
	trns1 = _mm_unpacklo_ps(mtL3, mtL4);
	trns2 = _mm_unpackhi_ps(mtL1, mtL2);
	trns3 = _mm_unpackhi_ps(mtL3, mtL4);
	_L1 = _mm_movelh_ps(trns0, trns1);
	_L2 = _mm_movehl_ps(trns1, trns0);
	_L3 = _mm_movelh_ps(trns2, trns3);
	_L4 = _mm_movehl_ps(trns3, trns2);

	return matrix44_sse(vector4(_L1), vector4(_L2), vector4(_L3), vector4(_L4));

}
}
*/
