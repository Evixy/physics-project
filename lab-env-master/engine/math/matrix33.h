#pragma once
#include <cmath>
#include "vector4.h"
#include <iostream>


namespace Math
{
class matrix33
{

public:
	matrix33();
	~matrix33();

	float* get();

	//Returns rotation matrices for rotation around an axis or around a vector. Angle is in radians
	static matrix33 rotation_x(const float& angle);
	static matrix33 rotation_y(const float& angle);
	static matrix33 rotation_z(const float& angle);
	static matrix33 rotation_vect(const float& angle, vector4& vect);

	//Transposes a matrix
	matrix33 transpose();

	//Inverts a matrix if it is possible, otherwise false will be returned
	inline matrix33 inverse();

	//Multiplication with a vector. Vector must be multiplied from the right. 
	matrix33 operator* (vector4& vect);
	matrix33 operator*= (vector4& vect);

	//Multiplication with matrices
	matrix33 operator* (matrix33& matrix);
	matrix33 operator*= (matrix33& matrix);

	//Changing values. Returns a vector3 which then uses the [] operator in vector3. 
	//Usage:
	//-------------------
	//matrix33 matrix;
	//matrix[0][0] = 1.0f; 
	//-------------------
	//Will set the value at position (1,1) in the matrix to 1.0
	float* operator[] (size_t index);

	//For printing a matrix
	friend std::ostream& operator<< (std::ostream& o, matrix33& matrix);

private:

	float matrix[3][3];
	//vector3 matrix[3];
};

///Constructor
inline matrix33::matrix33()
{
	matrix[0][0] = 1.0f;	matrix[0][1] = 0;		matrix[0][2] = 0;
	matrix[1][0] = 0;		matrix[1][1] = 1.0f;	matrix[1][2] = 0;
	matrix[2][0] = 0;		matrix[2][1] = 0;		matrix[2][2] = 1.0f;

	/*
	vector3 vect1(1.0f, 0, 0);
	vector3 vect2(0, 1.0f, 0);
	vector3 vect3(0, 0, 1.0f);
	this->matrix[0] = vect1;
	this->matrix[1] = vect2;
	this->matrix[2] = vect3;
	*/
}

///Destructor
inline matrix33::~matrix33()
{
	//- empty
}

inline float* matrix33::get()
{
	return (float*)matrix;
}

///Multiplication with another Matrix
inline matrix33 matrix33::operator*= (matrix33& matrix)
{
	matrix33 temp;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[j][i] = 0;
			for (int x = 0; x < 3; x++)
			{
				temp[j][i] = temp[j][i] + this->matrix[j][x] * matrix[x][i];
			}
		}
	}
	return temp;
}

///Multiplication with a vector3. Vector must be multiplied from the right  						
inline matrix33 matrix33::operator*= (vector4& vect)
{
	matrix33 temp;
	for (int j = 0; j < 3; j++)
	{
		temp[j][0] = 0;
		for (int x = 0; x < 3; x++)
		{
			temp[j][0] = temp[j][0] + this->matrix[j][x] * vect[x];
		}

	}
	return temp;
}

///Multiplication with another Matrix
inline matrix33 matrix33::operator* (matrix33& matrix)
{
	matrix33 temp;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[j][i] = 0;
			for (int x = 0; x < 3; x++)
			{
				temp[j][i] = temp[j][i] + this->matrix[j][x] * matrix[x][i];
			}
		}
	}
	return temp;
}

///Multiplication with a vector3. Vector must be multiplied from the right 
inline matrix33 matrix33::operator* (vector4& vect)
{
	matrix33 temp;
	for (int j = 0; j < 3; j++)
	{
		temp[j][0] = 0;
		for (int x = 0; x < 3; x++)
		{
			temp[j][0] = temp[j][0] + this->matrix[j][x] * vect[x];
		}

	}
	return temp;
}

///Method for printing a specific matrix. Purely for bug-testing
inline std::ostream& operator<< (std::ostream& o, matrix33& matrix)
{
	for (int i = 0; i < 3; i++)
	{
		o << "[";
		o << matrix[i][0];
		for (int j = 1; j < 3; j++)
		{
			o << ", " << matrix[i][j];
		}
		o << "]" << std::endl;
	}
	std::cout << std::endl;
	return o;
}

///Returns a rotation matrix for rotation around the x-axis
inline matrix33 matrix33::rotation_x(const float& angle)
{
	matrix33 temp;
	float s = sin(angle);
	float c = cos(angle);

	temp[0][0] = 1.0f;
	temp[0][1] = 0;
	temp[0][2] = 0;

	temp[1][0] = 0;
	temp[1][1] = c;
	temp[1][2] = -s;

	temp[2][0] = 0;
	temp[2][1] = s;
	temp[2][2] = c;

	return temp;
}

///Returns a rotation matrix for rotation around the y-axis
inline matrix33 matrix33::rotation_y(const float& angle)
{
	matrix33 temp;
	float s = sin(angle);
	float c = cos(angle);

	temp[0][0] = c;
	temp[0][1] = 0;
	temp[0][2] = s;

	temp[1][0] = 0;
	temp[1][1] = 1.0f;
	temp[1][2] = 0;

	temp[2][0] = -s;
	temp[2][1] = 0;
	temp[2][2] = c;

	return temp;
}

///Returns a rotation matrix for rotation around the z-axis
inline matrix33 matrix33::rotation_z(const float& angle)
{
	matrix33 temp;
	float s = sin(angle);
	float c = cos(angle);

	temp[0][0] = c;
	temp[0][1] = -s;
	temp[0][2] = 0;

	temp[1][0] = s;
	temp[1][1] = c;
	temp[1][2] = 0;

	temp[2][0] = 0;
	temp[2][1] = 0;
	temp[2][2] = 1.0f;

	return temp;
}

///Returns a rotation matrix for rotation around a vector
inline matrix33 matrix33::rotation_vect(const float& angle, vector4& vect)
{
	matrix33 temp;
	float s = sin(angle);
	float c = cos(angle);
	float x = vect[0];
	float y = vect[1];
	float z = vect[2];

	temp[0][0] = c + x * x * (1.0f - c);
	temp[0][1] = x * y * (1.0f - c) - z * s;
	temp[0][2] = x * z * (1.0f - c) + y * s;

	temp[1][0] = y * x * (1.0f - c) + z * s;
	temp[1][1] = c + y * y * (1.0f - c);
	temp[1][2] = y * z * (1.0f - c) - x * s;

	temp[2][0] = z * x * (1.0f - c) - y * s;
	temp[2][1] = z * y * (1.0f - c) + x * s;
	temp[2][2] = c + z * z *(1.0f - c);

	return temp;
}

//Used to set values for the matrices.
inline float* matrix33::operator[] (size_t index)
{
	return this->matrix[index];
}

///Transposes the specified matrix
inline matrix33 matrix33::transpose()
{
	matrix33 temp;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[i][j] = this->matrix[j][i];
		}
	}
	return temp;
}

///Creates the inverse of a matrix A provided the determinant of A is != 0. If det A = 0 it will return false
inline matrix33 matrix33::inverse()
{
	matrix33 temp;
	float a = this->matrix[0][0];
	float b = this->matrix[0][1];
	float c = this->matrix[0][2];

	float d = this->matrix[1][0];
	float e = this->matrix[1][1];
	float f = this->matrix[1][2];

	float g = this->matrix[2][0];
	float h = this->matrix[2][1];
	float i = this->matrix[2][2];

	float det = a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g);
	if (det = 0)
	{
		return temp; //Returns a 3x3 unit matrix if det = 0. Though this should never happen
	}
	float k = 1 / det;

	temp[0][0] = e*i - f*h;
	temp[0][1] = f*g - d*i;
	temp[0][2] = d*h - e*g;
	temp[1][0] = c*h - b*i;
	temp[1][1] = a*i - c*g;
	temp[1][2] = b*g - a*h;
	temp[2][0] = b*f - c*e;
	temp[2][1] = c*d - a*f;
	temp[2][2] = a*e - b*d;
	temp = temp.transpose();
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			temp[j][i] *= k;
		}
	}
	return temp;
	/*
	this->matrix[0][0] = temp[0][0]; this->matrix[0][1] = temp[0][1]; this->matrix[0][2] = temp[0][2];
	this->matrix[1][0] = temp[1][0]; this->matrix[1][1] = temp[1][1]; this->matrix[1][2] = temp[1][2];
	this->matrix[2][0] = temp[2][0]; this->matrix[2][1] = temp[2][1]; this->matrix[2][2] = temp[2][2];
	*/

}

}