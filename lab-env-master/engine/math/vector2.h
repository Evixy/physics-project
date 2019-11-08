#pragma once
#include <cmath>

namespace Math
{
class vector2
{

public:

	//Constructor
	vector2(float x, float y);
	vector2(){ this->vec[0] = 0; this->vec[1] = 0; }

	//Destructor
	~vector2();

	inline float* Get();
	//Subtract a vector2 from this->vector2 and returns a new vector2
	vector2 operator- (vector2& vect) const;

	//Subtract a vector2 from this->vector2 and returns this->vector2
	void operator-= (vector2& vect);

	//Adds two vector2 and returns a new vector2
	vector2 operator+ (vector2& vect) const;

	//Adds two vector2 and returns this->vector2
	void operator+= (vector2& vect);

	//Multiplication with a scalar from the right (A * x), returns a new vector2
	vector2 operator* (float scal);

	//Multiplication with a scalar from the right (A * x), returns a this->vector2
	void operator*= (float scal);

	//Scale down this->vector2
	void operator/=(float scal);
		
	//Scale down this->vector2 and return a new vector2
	vector2 operator/(float scal);

	//Multiplication with a scalar from the left (x * A)(friend), modifies this->vector2 by reference
	friend void operator*= (float scal, vector2& vect);
		
	//Multiplication with a scalar from the left (x * A)(friend), returns a new vector2
	friend vector2 operator* (float scal, vector2 vect);

	float& x();
	float x() const;

	float& y();
	float y() const;

	//Changing values
	float& operator[] (size_t index);

	//Dot-product
	float operator* (vector2& vect);

	float Dot(vector2& vect);

	//Get length of a vector
	float Length();

	float LengthSq();
	//Get angle between two vectors in radians
		

	//Normalize this->vector2
	void Normalize(vector2& vec);

	//Normalize a vector2 and get a new vector2
	vector2 NormalizeNew(vector2& vec);

	float vec[2];
private:

};
	
inline vector2::vector2(float x, float y)
{
	this->vec[0] = x;
	this->vec[1] = y;
}

inline vector2::~vector2()
{

}

inline float* vector2::Get()
{
	return (float*)vec;
}

inline vector2 vector2::operator-(vector2& vect) const
{
	vector2 temp(*this);
	temp -= vect;
	return temp;
}

inline void vector2::operator-=(vector2& vect)
{
	for (int i = 0; i < 2; i++)
	{
		this->vec[i] -= vect[i];
	}
}

inline vector2 vector2::operator+(vector2& vect) const
{
	vector2 temp(*this);
	temp += vect;
	return temp;
}

inline void vector2::operator+=(vector2& vect)
{
	for (int i = 0; i < 2; i++)
	{
		this->vec[i] += vect[i];
	}
}

inline float vector2::operator*(vector2& vect)
{
	return (this->vec[0] * vect.vec[0] + this->vec[1] * vect.vec[1]);
}

inline vector2 vector2::operator*(float scal)
{
	vector2 temp(*this);
	for (int i = 0; i < 2; i++)
	{
		temp[i] *= scal;
	}
	return temp;
}

inline void vector2::operator*=(float scal)
{
	for (int i = 0; i < 2; i++)
	{
		this->vec[i] *= scal;
	}
}

inline float vector2::Length()
{
	return sqrt(this->vec[0] * this->vec[0] + this->vec[1] * this->vec[1]);
}

inline float vector2::LengthSq()
{
	return (this->LengthSq() * this->Length());
}

inline void vector2::Normalize(vector2& vec)
{
	vector2 temp(*this);
	float d = 1 / this->Length();
	vec *= d;
}

inline vector2 vector2::NormalizeNew(vector2& vec)
{
	vector2 temp(*this);
	float d = 1 / this->Length();
	temp *= d;
	return temp;
}

inline float& vector2::operator[](size_t index)
{
	return vec[index];
}

inline float vector2::Dot(vector2& vect)
{
	return (this->vec[0] * vect[0] + this->vec[1] * vect[1]);
}

inline void vector2::operator/=(float scal)
{
	this->vec[0] /= scal;
	this->vec[1] /= scal;
}

inline vector2 vector2::operator/(float scal)
{
	vector2 temp(*this);
	temp[0] /= scal;
	temp[1] /= scal;
	return temp;
}

inline float& vector2::x()
{
	return this->vec[0];
}

inline float vector2::x() const
{
	return this->vec[0];
}

inline float& vector2::y()
{
	return this->vec[1];
}

inline float vector2::y() const
{
	return this->vec[1];
}

inline void operator*= (float scal, vector2& vect)
{
	vect *= scal;
}

//Multiplication with a scalar from the left (x * A)(friend), returns a new vector2
inline vector2 operator* (float scal, vector2 vect)
{
	return (vect * scal);
}
		
	
}