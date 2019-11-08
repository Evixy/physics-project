#pragma once
#include <cmath>
#include <iostream>
#include <assert.h>
//#include <x86intrin.h>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <pmmintrin.h>
#include <nmmintrin.h>
#include <ammintrin.h>
#include <core/defines.h>

namespace Math
{

typedef ALIGN16 struct mm128_vec
{
	union {
		float f[4];
		unsigned int u[4];
		__m128 vec;
	};
	inline operator __m128() const { return this->vec; }
} mm128_vec;

class ALIGN16 vector4
{

public:

	///Constructor
	
	vector4();
	vector4(float x, float y, float z, float w = 0.0f);
	vector4(const mm128_vec& v);
	///copy constructor
	vector4(const __m128& v);
	///Destructor
	~vector4();

	///assignment operator
	void operator=(const vector4 & v);
	void operator=(const __m128& v);
	
	///check if vectors are equal
	bool operator==(vector4& vect);
	///check if vectors are not equal
	bool operator!=(vector4& vect);
	
	///subtract a vector from this vector and return a new vector
	vector4 operator- (const vector4& v) const;
	vector4 operator- () const;
	void operator-= (const vector4& v);

	void operator/= (const vector4& v);
	vector4 operator/ (const vector4& v);

	void set(float x, float y, float z);
	
	float& x();
	float& y();
	float& z();
	float& w();

	float x() const;
	float y() const;
	float z() const;
	float w() const;

	void set_x(float val);
	void set_y(float val);
	void set_z(float val);
	void set_w(float val);

	//Addition
	vector4 operator+(const vector4& v);
	void operator+=(const vector4 &v);

	//Multiplication with a scalar from the right (A * x)
	vector4 operator* (float scal);
	void operator*= (const vector4& v);

	vector4 operator/ (float scal);


	//Multiplication with a scalar from the left (x * A)
	friend vector4 operator* (float scal, const vector4& v);

	//Changing values
	float& operator[](unsigned int index);

	static vector4 splat_x(const vector4& v);
	static vector4 splat_y(const vector4& v);
	static vector4 splat_z(const vector4& v);
	static vector4 splat_w(const vector4& v);

	static vector4 multiply(const vector4& v1, const vector4& v2);
	static float dot3(const vector4& v1, const vector4& v2);
	static float dot4(const vector4& v1, const vector4& v2);
		
	static vector4 cross3(const vector4& v1, const vector4& v2);
	static vector4 cross3test(const vector4& v1, const vector4& v2);
	static vector4 crossdouble3(const vector4& v1, const vector4& v2);

	static bool equal3_all(const vector4& v1, const vector4& v2);
	static bool equal4_all(const vector4& v1, const vector4& v2);

	static vector4 zero();

	//Get length of a vector
	float length3();
	float length4();

	//Get angle between two vectors in radians
	float angleBetweenTwoVectors(vector4& v);

	//Normalize vectors
	static vector4 normalize3(vector4& v);
	static vector4 normalize4(vector4& v);

	//Printing vectors
	friend std::ostream& operator<< (std::ostream& o, vector4& vect);

	mm128_vec vec;

};

//------------------------------------------------------------------------------
/**
*/
inline vector4::vector4()
{
	this->vec.vec = _mm_setr_ps(0.0f, 0.0f, 0.0f, 0.0f);	
}

//------------------------------------------------------------------------------
/**
*/
inline vector4::vector4(float x, float y, float z, float w)
{
	this->vec.vec = _mm_setr_ps(x, y, z, w);
}

inline vector4::vector4(const mm128_vec& v):
	vec(v)
{

}

//------------------------------------------------------------------------------
/**
*/
inline vector4::vector4(const __m128& v)
{
	this->vec.vec = v;
}

//------------------------------------------------------------------------------
/**
*/
inline vector4::~vector4()
{
	//- empty
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::set(float x, float y, float z)
{
	this->vec.vec = _mm_setr_ps(x, y, z, 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
inline float& vector4::x()
{
	return this->vec.f[0];
}

//------------------------------------------------------------------------------
/**
*/
inline float& vector4::y()
{
	return this->vec.f[1];
}

//------------------------------------------------------------------------------
/**
*/
inline float& vector4::z()
{
	return this->vec.f[2];
}

//------------------------------------------------------------------------------
/**
*/
inline float& vector4::w()
{
	return this->vec.f[3];
}

//------------------------------------------------------------------------------
/**
*/
inline float vector4::x() const
{
	float temp;
	_mm_store_ss(&temp, this->vec.vec);
	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline float vector4::y() const
{
	float ret;
	__m128 temp = _mm_shuffle_ps(this->vec.vec, this->vec.vec, _MM_SHUFFLE(1, 1, 1, 1));
	_mm_store_ss(&ret, temp);
	return ret;
}

//------------------------------------------------------------------------------
/**
*/
inline float vector4::z() const
{
	float ret;
	__m128 temp = _mm_shuffle_ps(this->vec.vec, this->vec.vec, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(&ret, temp);
	return ret;
}

//------------------------------------------------------------------------------
/**
*/
inline float vector4::w() const
{
	float ret;
	__m128 temp = _mm_shuffle_ps(this->vec.vec, this->vec.vec, _MM_SHUFFLE(3, 3, 3, 3));
	_mm_store_ss(&ret, temp);
	return ret;
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::set_x(float val)
{
	__m128 temp = _mm_load_ps1(&val);
	this->vec.vec = _mm_move_ss(this->vec.vec, temp);
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::set_y(float val)
{
	__m128 temp = _mm_load_ps1(&val);
	this->vec.vec = _mm_blend_ps(this->vec.vec, temp, 2);
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::set_z(float val)
{
	__m128 temp = _mm_load_ps1(&val);
	this->vec.vec = _mm_blend_ps(this->vec.vec, temp, 4);
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::set_w(float val)
{
	__m128 temp = _mm_load_ps1(&val);
	this->vec.vec = _mm_blend_ps(this->vec.vec, temp, 8);
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::operator=(const vector4& v)
{
	this->vec.vec = v.vec;
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::operator=(const __m128& v)
{
	this->vec.vec = v;
}

//------------------------------------------------------------------------------
/**
*/
inline bool vector4::operator==(vector4& v)
{
	/*__m128 temp = _mm_cmpeq_ps(this->vec, v.vec);
	return ((_mm_movemask_ps(temp) == 0x0f) == 0.0f);	*/
	return (this->x() == v.x() && this->y() == v.y() && this->z() == v.z());
}

//------------------------------------------------------------------------------
/**
*/
inline bool vector4::operator!=(vector4& v)
{
	__m128 temp = _mm_cmpeq_ps(this->vec, v.vec);
	return ((_mm_movemask_ps(temp) == 0x0f) != 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::operator+=(const vector4& v)
{
	this->vec.vec = _mm_add_ps(this->vec.vec, v.vec.vec);
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::operator+(const vector4& v)
{
	return _mm_add_ps(this->vec.vec, v.vec.vec);
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::operator-= (const vector4& v)
{
	this->vec.vec = _mm_sub_ps(this->vec.vec, v.vec.vec);
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::operator/=(const vector4& v)
{
	this->vec.vec = _mm_div_ps(this->vec.vec, v.vec.vec);
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::operator/(const vector4& v)
{
	return _mm_div_ps(this->vec.vec, v.vec.vec);
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::operator- (const vector4& v) const
{
	return _mm_sub_ps(this->vec.vec, v.vec.vec);
}

inline vector4 vector4::operator-() const
{
	return vector4(-this->x(), -this->y(), -this->z(), -this->w());
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::operator* (float scal)
{
	__m128 temp = _mm_set1_ps(scal);
	return _mm_mul_ps(this->vec.vec, temp);
}

//------------------------------------------------------------------------------
/**
*/
inline void vector4::operator*= (const vector4& v)
{
	this->vec.vec = _mm_mul_ps(this->vec.vec, v.vec);
}

inline vector4 vector4::operator/(float scal)
{
	__m128 temp = _mm_set1_ps(scal);
	return _mm_div_ps(this->vec.vec, temp);
}

//------------------------------------------------------------------------------
/**
*/
inline float vector4::length3()
{
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(this->vec.vec, this->vec.vec, 0x71)));
}

//------------------------------------------------------------------------------
/**
*/
inline float vector4::length4()
{
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(this->vec.vec, this->vec.vec, 0xF1)));
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::normalize3(vector4& v)
{
	if (equal3_all(v, vector4(0, 0, 0, 0)))
	{
		return v;
	}
	vector4 vec(_mm_div_ps(v.vec.vec, _mm_sqrt_ps(_mm_dp_ps(v.vec.vec, v.vec.vec, 0xF7))));
	vec.set_w(0.0f);
	return vec;
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::normalize4(vector4& v)
{
	if (equal4_all(v, vector4(0, 0, 0, 0)))
	{
		return v;
	}
	vector4 vec(_mm_div_ps(v.vec.vec, _mm_sqrt_ps(_mm_dp_ps(v.vec.vec, v.vec.vec, 0xFF))));
	return vec;
}

//------------------------------------------------------------------------------
/**
*/
inline float& vector4::operator[](unsigned int index)
{	
	return this->vec.f[index];
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::splat_x(const vector4& v)
{
	return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(0, 0, 0, 0));
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::splat_y(const vector4& v)
{
	return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(1, 1, 1, 1));
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::splat_z(const vector4& v)
{
	return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(2, 2, 2, 2));
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::splat_w(const vector4& v)
{
	return _mm_shuffle_ps(v.vec, v.vec, _MM_SHUFFLE(3, 3, 3, 3));
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::multiply(const vector4& v1, const vector4& v2)
{
	return _mm_mul_ps(v1.vec.vec, v2.vec.vec);
}

//------------------------------------------------------------------------------
/**
*/
inline float vector4::dot3(const vector4& v1, const vector4& v2)
{
	return _mm_cvtss_f32(_mm_dp_ps(v1.vec.vec, v2.vec.vec, 0x71));
}

//------------------------------------------------------------------------------
/**
*/
inline float vector4::dot4(const vector4& v1, const vector4& v2)
{
	return _mm_cvtss_f32(_mm_dp_ps(v1.vec.vec, v2.vec.vec, 0xF1));
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::cross3(const vector4& v1, const vector4& v2)
{
	__m128 tmp0, tmp1, tmp2, tmp3, result;
	tmp0 = _mm_shuffle_ps(v1.vec.vec, v1.vec.vec, _MM_SHUFFLE(3, 0, 2, 1));
	tmp1 = _mm_shuffle_ps(v2.vec.vec, v2.vec.vec, _MM_SHUFFLE(3, 1, 0, 2));
	tmp2 = _mm_shuffle_ps(v1.vec.vec, v1.vec.vec, _MM_SHUFFLE(3, 1, 0, 2));
	tmp3 = _mm_shuffle_ps(v2.vec.vec, v2.vec.vec, _MM_SHUFFLE(3, 0, 2, 1));
	result = _mm_mul_ps(tmp0, tmp1);
	result = _mm_sub_ps(result, _mm_mul_ps(tmp2, tmp3));
	return result;

}

//------------------------------------------------------------------------------
/**
*/
inline vector4 vector4::cross3test(const vector4& v1, const vector4& v2)
{
	return vector4(v1.y() * v2.z() - v1.z() * v2.y(),
		v1.z() * v2.x() - v1.x() * v2.z(),
		v1.x() * v2.y() - v1.y() * v2.x());
}

inline vector4 vector4::crossdouble3(const vector4& v1, const vector4& v2)
{
	return cross3(cross3(v1, v2), v1);
}

//------------------------------------------------------------------------------
/**
*/
inline bool vector4::equal3_all(const vector4& v1, const vector4& v2)
{
	__m128 vTemp = _mm_cmpeq_ps(v1.vec, v2.vec);
	int res = _mm_movemask_ps(vTemp) & 7;
	return res == 0x7;
}

//------------------------------------------------------------------------------
/**
*/
inline bool vector4::equal4_all(const vector4& v1, const vector4& v2)
{
	__m128 vTemp = _mm_cmpeq_ps(v1.vec, v2.vec);
	int res = _mm_movemask_ps(vTemp);
	return res == 0xf;
}

inline vector4 vector4::zero()
{
	return vector4();
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 operator* (float scal, const vector4& v)
{
	__m128 temp = _mm_set1_ps(scal);
	return _mm_mul_ps(v.vec.vec, temp);
}
}
