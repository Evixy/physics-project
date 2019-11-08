#pragma once
#include <cmath>

#include <iostream>
#include "vector4.h"
#define PI 3.14159265f

namespace Math
{
class vector4;

class matrix44
{

public:
	//Constructor
	matrix44();

	//Destructor
	~matrix44();
	static matrix44 normalizeAxis(matrix44& matrix44);
	void normalizeAxis();
	static matrix44 transMat(vector4 & vec);
	//Sets a unit matrix to have elements (1,4) = x, (2,4) = y and (3,4) = z as values. 
	static matrix44 transMat(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	static matrix44 transMatFromMatrix(matrix44& mat);
	static matrix44 transMatFromVec(vector4& vec);
	static matrix44 matrixFrom_Trans_Rot_Scale(vector4 & trans, vector4 & rot, vector4 & scale);

	static matrix44 genProjMatrix(float fov, float aspect, float near, float far);

	static matrix44 scaleMatFromVect(vector4 & scale);
	
	static matrix44 quaternionToMatrix(vector4& q);
	static vector4 matrixToQuaternion(matrix44& mat);

	static matrix44 createSkewMatrix(vector4& vec);

	float* get();

	//Returns rotation matrices for rotation around an axis or around a vector. Angle is in radians
	static matrix44 rotation_x(float angle, bool isDegrees = false);
	static matrix44 rotation_y(float angle, bool isDegrees = false);
	static matrix44 rotation_z(float angle, bool isDegrees = false);
	static matrix44 rotation_vect(const float& angle, vector4& vect);

	static vector4 transform(vector4& v, matrix44& m);
	
	static vector4 toAxisAngle(matrix44& m);

	static bool isIdenity(matrix44& mat);
	bool isIdentity();

	void translateMatrix(vector4& v);
	//Transposes a matrix
	matrix44 transpose();
	static matrix44 inverse(matrix44& matrix);
	//Inverts a matrix if it is possible, otherwise false will be returned
	matrix44 inverse();

	//Multiplication with a vector. Vector must be multiplied from the right. 
	vector4 operator* (vector4& vect);
	

	//Multiplication with matrices
	matrix44 operator* (matrix44& matrix);
	void operator*= (matrix44& matrix);
	matrix44 operator+ (matrix44& matrix);
	void operator+= (matrix44& matrix);
	vector4 getPos();
	void setPos(vector4& pos);
	//Changing values. Returns a vector4 which then uses the [] operator in vector4. 
	//Usage:
	//-------------------
	//matrix44 matrix;
	//matrix[0][0] = 1.0f; 
	//-------------------
	//Will set the value at position (1,1) in the matrix to 1.0
	float* operator[](unsigned int index);

	//For printing a matrix
	friend std::ostream& operator<< (std::ostream& o, matrix44& matrix);
private:

	float matrix[4][4];

};

//------------------------------------------------------------------------------
/**
*/
inline matrix44::matrix44()
{
	this->matrix[0][0] = 1.0f;	this->matrix[0][1] = 0;		this->matrix[0][2] = 0;		this->matrix[0][3] = 0;
	this->matrix[1][0] = 0;		this->matrix[1][1] = 1.0f;	this->matrix[1][2] = 0;		this->matrix[1][3] = 0;
	this->matrix[2][0] = 0;		this->matrix[2][1] = 0;		this->matrix[2][2] = 1.0f;	this->matrix[2][3] = 0;
	this->matrix[3][0] = 0;		this->matrix[3][1] = 0;		this->matrix[3][2] = 0;		this->matrix[3][3] = 1.0f;
	/*
	vector4 vect1(1.0f, 0, 0, 0);
	vector4 vect2(0, 1.0f, 0, 0);
	vector4 vect3(0, 0, 1.0f, 0);
	vector4 vect4(0, 0, 0, 1.0f);
	this->matrix[0] = vect1;
	this->matrix[1] = vect2;
	this->matrix[2] = vect3;
	this->matrix[3] = vect4;
	*/
}


//------------------------------------------------------------------------------
/**
*/
inline matrix44::~matrix44()
{
	//- empty
}

inline matrix44 matrix44::normalizeAxis(matrix44& m)
{
	matrix44 temp;
	vector4 x(m[0][0], m[0][1], m[0][2]);
	vector4 y(m[1][0], m[1][1], m[1][2]);
	vector4 z(m[2][0], m[2][1], m[2][2]);

	x = vector4::normalize3(x);
	y = vector4::normalize3(y);
	z = vector4::normalize3(z);

	temp[0][0] = x.x();
	temp[0][1] = x.y();
	temp[0][2] = x.z();
	temp[0][3] = 0.0f;

	temp[1][0] = y.x();
	temp[1][1] = y.y();
	temp[1][2] = y.z();
	temp[1][3] = 0.0f;

	temp[2][0] = z.x();
	temp[2][1] = z.y();
	temp[2][2] = z.z();
	temp[2][3] = 0.0f;

	temp[3][0] = 0.0f;
	temp[3][1] = 0.0f;
	temp[3][2] = 0.0f;
	temp[3][3] = 1.0f;

	return temp;
}

inline void matrix44::normalizeAxis()
{
	vector4 x(this->matrix[0][0], this->matrix[0][1], this->matrix[0][2]);
	vector4 y(this->matrix[1][0], this->matrix[1][1], this->matrix[1][2]);
	vector4 z(this->matrix[2][0], this->matrix[2][1], this->matrix[2][2]);

	x = vector4::normalize3(x);
	y = vector4::normalize3(y);
	z = vector4::normalize3(z);

	this->matrix[0][0] = x.x();
	this->matrix[0][1] = x.y();
	this->matrix[0][2] = x.z();
	this->matrix[0][3] = 0.0f;

	this->matrix[1][0] = y.x();
	this->matrix[1][1] = y.y();
	this->matrix[1][2] = y.z();
	this->matrix[1][3] = 0.0f;

	this->matrix[2][0] = z.x();
	this->matrix[2][1] = z.y();
	this->matrix[2][2] = z.z();	
	this->matrix[2][3] = 0.0f;

	this->matrix[3][0] = 0.0f;
	this->matrix[3][1] = 0.0f;
	this->matrix[3][2] = 0.0f;
	this->matrix[3][3] = 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::transMat(vector4 & vec)
{
	matrix44 mat;

	mat[3][0] = vec[0];
	mat[3][1] = vec[1];
	mat[3][2] = vec[2];

	return mat;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::transMat(float x, float y, float z)
{
	matrix44 mat;

	mat[3][0] = x;
	mat[3][1] = y;
	mat[3][2] = z;

	return mat;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::matrixFrom_Trans_Rot_Scale(vector4 & pos, vector4 & rot, vector4 & scale)
{
	matrix44 transmat;
	matrix44 scalemat;
	matrix44 rotmat;
	//trans
	transmat = transMat(pos);
	
	//scale
	scalemat[0][0] = scale[0];
	scalemat[1][1] = scale[1];
	scalemat[2][2] = scale[2];
	scalemat[3][3] = 1.0f;

	//rot
	rotmat = quaternionToMatrix(rot);
	
	matrix44 final1 = transmat * rotmat * scalemat;
	matrix44 final2 = scalemat * rotmat * transmat;
	return final2;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::genProjMatrix(float fov, float aspect, float near, float far)
{
	matrix44 projMatrix;	//Creates a unit matrix;
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
	projMatrix.transpose();
	return projMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::scaleMatFromVect(vector4 & scale)
{
	matrix44 temp;

	temp[0][0] = scale[0];
	temp[1][1] = scale[1];
	temp[2][2] = scale[2];

	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::transMatFromMatrix(matrix44& mat)
{
	matrix44 temp;
	vector4 pos = mat.getPos();
	temp.setPos(pos);
	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::transMatFromVec(vector4& vec)
{
	matrix44 temp;
	temp.setPos(vec);
	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::quaternionToMatrix(vector4& q)
{
	matrix44 mat;
		
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

//------------------------------------------------------------------------------
/**
*/
inline vector4 matrix44::matrixToQuaternion(matrix44& mat)
{
	float qw = sqrtf(1 + mat[0][0] + mat[1][1] + mat[2][2]) * 0.5f;
	float qx = (mat[2][1] - mat[1][2]) / (4 * qw);
	float qy = (mat[0][2] - mat[2][0]) / (4 * qw);
	float qz = (mat[1][0] - mat[0][1]) / (4 * qw);
	return vector4(qx, qy, qz, qw);
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::createSkewMatrix(vector4& vec)
{
	matrix44 mat;
	
	mat[0][0] = 0.0f;		mat[0][1] = -vec.z();	mat[0][2] = vec.y();	mat[0][3] = 0.0f;
	mat[1][0] = vec.z();	mat[1][1] = 0.0f;		mat[1][2] = -vec.x();	mat[1][3] = 0.0f;
	mat[2][0] = -vec.y();	mat[2][1] = vec.x();	mat[2][2] = 0.0f;		mat[2][3] = 0.0f;
	mat[3][0] = 0.0f;		mat[3][1] = 0.0f;		mat[3][2] = 0.0f;		mat[3][3] = 0.0f;

	return mat;
}

//------------------------------------------------------------------------------
/**
*/
inline float* matrix44::get()
{
	return reinterpret_cast<float*>(matrix);
}

//------------------------------------------------------------------------------
/**
*/
inline void matrix44::operator*= (matrix44& matrix)
{
	matrix44 temp = *this;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			this->matrix[j][i] = 0;
			for (int x = 0; x < 4; x++)
			{
				this->matrix[j][i] += temp[j][x] * matrix[x][i];
			}
		}
	}	
}

inline matrix44 matrix44::operator+(matrix44& matrix)
{
	matrix44 m;
	for(int j = 0; j < 4; j++)
	{
		for(int i = 0; i < 4; i++)
		{
			m[j][i] = this->matrix[j][i] + matrix[j][i];
		}
	}
	return m;
}

inline void matrix44::operator+=(matrix44& matrix)
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			this->matrix[j][i] += matrix[j][i];
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 matrix44::getPos()
{
	return vector4(this->matrix[3][0], this->matrix[3][1], this->matrix[3][2], 1.0f);
}

//------------------------------------------------------------------------------
/**
*/
inline void matrix44::setPos(vector4& pos)
{
	this->matrix[3][0] = pos.x();
	this->matrix[3][1] = pos.y();
	this->matrix[3][2] = pos.z();
	this->matrix[3][3] = 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::operator* (matrix44& matrix)
{
	matrix44 temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[i][j] = 0;
			for (int x = 0; x < 4; x++)
			{
				temp[i][j] = temp[i][j] + (this->matrix[i][x] * matrix[x][j]);
			}
		}
	}
	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 matrix44::operator* (vector4& vect)
{
	vector4 temp;
	for (int j = 0; j < 4; j++)
	{
		temp[j] = 0;
		for (int x = 0; x < 4; x++)
		{
			float val = vect[x];
			temp[j] += this->matrix[j][x] * vect[x];
		}
	}
	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline std::ostream& operator<< (std::ostream& o, matrix44& matrix)
{
	for (int i = 0; i < 4; i++)
	{
		o << "|";
		o << matrix[i][0];
		for (int j = 1; j < 4; j++)
		{
			o << ", " << matrix[i][j];
		}
		o << "|" << std::endl;
	}
	std::cout << std::endl;
	return o;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::rotation_x(float angle, bool isDegrees)
{
	if (isDegrees)
	{
		angle *= (PI/ 180.0f);
	}
	matrix44 temp;
	float s = sin(angle);
	float c = cos(angle);
	/*
	temp[0][0] = 1.0f;	temp[0][1] = 0;		temp[0][2] = 0;		temp[0][3] = 0;
	temp[1][0] = 0;		temp[1][1] = c;		temp[1][2] = s;		temp[1][3] = 0;
	temp[2][0] = 0;		temp[2][1] = -s;	temp[2][2] = c;		temp[2][3] = 0;
	temp[3][0] = 0;		temp[3][1] = 0;		temp[3][2] = 0;		temp[3][3] = 1.0f;
	*/
	
	//original
	temp[0][0] = 1.0f;	temp[0][1] = 0;		temp[0][2] = 0;		temp[0][3] = 0;
	temp[1][0] = 0;		temp[1][1] = c;		temp[1][2] = -s;	temp[1][3] = 0;
	temp[2][0] = 0;		temp[2][1] = s;		temp[2][2] = c;		temp[2][3] = 0;
	temp[3][0] = 0;		temp[3][1] = 0;		temp[3][2] = 0;		temp[3][3] = 1.0f;
	
	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::rotation_y(float angle, bool isDegrees)
{
	if (isDegrees)
	{
		angle *= (PI / 180.0f);
	}
	matrix44 temp;
	float s = sin(angle);
	float c = cos(angle);
	/*
	temp[0][0] = c;		temp[0][1] = 0;		temp[0][2] = -s;	temp[0][3] = 0;
	temp[1][0] = 0;		temp[1][1] = 1.0f;	temp[1][2] = 0;		temp[1][3] = 0;
	temp[2][0] = s;		temp[2][1] = 0;		temp[2][2] = c;		temp[2][3] = 0;
	temp[3][0] = 0;		temp[3][1] = 0;		temp[3][2] = 0;		temp[3][3] = 1.0f;
	*/
	
	//original 
	temp[0][0] = c;		temp[0][1] = 0;		temp[0][2] = s;		temp[0][3] = 0;
	temp[1][0] = 0;		temp[1][1] = 1.0f;	temp[1][2] = 0;		temp[1][3] = 0;
	temp[2][0] = -s;	temp[2][1] = 0;		temp[2][2] = c;		temp[2][3] = 0;
	temp[3][0] = 0;		temp[3][1] = 0;		temp[3][2] = 0;		temp[3][3] = 1.0f;
	
	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::rotation_z(float angle, bool isDegrees)
{
	if(isDegrees)
	{
		angle *= (PI / 180.0f);
	}
	matrix44 temp;
	float s = sin(angle);
	float c = cos(angle);

	/*
	temp[0][0] = c;		temp[0][1] = s;		temp[0][2] = 0;		temp[0][3] = 0;
	temp[1][0] = -s;	temp[1][1] = c;		temp[1][2] = 0;		temp[1][3] = 0;
	temp[2][0] = 0;		temp[2][1] = 0;		temp[2][2] = 1.0f;	temp[2][3] = 0;
	temp[3][0] = 0;		temp[3][1] = 0;		temp[3][2] = 0;		temp[3][3] = 1.0f;
	*/
	
	//original
	temp[0][0] = c;		temp[0][1] = -s;	temp[0][2] = 0;		temp[0][3] = 0;
	temp[1][0] = s;		temp[1][1] = c;		temp[1][2] = 0;		temp[1][3] = 0;
	temp[2][0] = 0;		temp[2][1] = 0;		temp[2][2] = 1.0f;	temp[2][3] = 0;
	temp[3][0] = 0;		temp[3][1] = 0;		temp[3][2] = 0;		temp[3][3] = 1.0f;
	
	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::rotation_vect(const float& angle, vector4& vect)
{
	matrix44 temp;
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

//------------------------------------------------------------------------------
/**
*/
inline vector4 matrix44::transform(vector4& v, matrix44& m)
{
	return m * v;
}

inline vector4 matrix44::toAxisAngle(matrix44& m)
{
	float angle, x, y, z;

	float e1 = 0.01f;
	float e2 = 0.1f;

	if ((abs(m[0][1] - m[1][0])< e1) && (abs(m[0][2] - m[2][0]) < e1) && (abs(m[1][2] - m[2][1]) < e1))
	{
		if ((abs(m[0][1] + m[1][0]) < e2) && (abs(m[0][2] + m[2][0]) < e2) && (abs(m[1][2] + m[2][1]) < e2)	&& (abs(m[0][0] + m[1][1] + m[2][2] - 3) < e2))
		{
			return vector4(0, 1, 0, 0);
		}
		angle = PI;
		float xx = (m[0][0] + 1) / 2.0f;
		float yy = (m[1][1] + 1) / 2.0f;
		float zz = (m[2][2] + 1) / 2.0f;
		float xy = (m[0][1] + m[1][0]) / 4.0f;
		float xz = (m[0][2] + m[2][0]) / 4.0f;
		float yz = (m[1][2] + m[2][1]) / 4.0f;

		if ((xx > yy) && (xx > zz))
		{ 
			if (xx < e1) 
			{
				x = 0;
				y = 0.7071f;
				z = 0.7071f;
			}
			else 
			{
				x = sqrt(xx);
				y = xy / x;
				z = xz / x;
			}
		}
		else if (yy > zz) 
		{ 
			if (yy < e1) 
			{
				x = 0.7071f;
				y = 0;
				z = 0.7071f;
			}
			else 
			{
				y = sqrt(yy);
				x = xy / y;
				z = yz / y;
			}
		}
		else 
		{ 
			if (zz < e1) 
			{
				x = 0.7071f;
				y = 0.7071f;
				z = 0;
			}
			else 
			{
				z = sqrt(zz);
				x = xz / z;
				y = yz / z;
			}
		}
		return vector4(angle, x, y, z); 
	}
	float s = sqrt((m[2][1] - m[1][2])*(m[2][1] - m[1][2]) + (m[0][2] - m[2][0])*(m[0][2] - m[2][0]) + (m[1][0] - m[0][1])*(m[1][0] - m[0][1]));

	if(abs(s) < 0.001f)
	{
		s = 1.0f;
	}
	angle = acos((m[0][0] + m[1][1] + m[2][2] - 1) / 2);
	x = (m[2][1] - m[1][2]) / s;
	y = (m[0][2] - m[2][0]) / s;
	z = (m[1][0] - m[0][1]) / s;

	return vector4(angle, x, y, z);
}

//------------------------------------------------------------------------------
/**
*/
inline bool matrix44::isIdenity(matrix44& mat)
{
	return(abs(1.0f - mat[0][0]) < 0.001f && abs(1.0f - mat[1][1]) < 0.001f && abs(1.0f - mat[2][2]) < 0.001f && abs(1.0f - mat[3][3]) < 0.001f);
}

//------------------------------------------------------------------------------
/**
*/
inline bool matrix44::isIdentity()
{
	return(abs(1.0f - this->matrix[0][0]) < 0.001f && abs(1.0f - this->matrix[1][1]) < 0.001f && abs(1.0f - this->matrix[2][2]) < 0.001f && abs(1.0f - this->matrix[3][3]) < 0.001f);
}

//------------------------------------------------------------------------------
/**
*/
inline void matrix44::translateMatrix(vector4& v)
{
	vector4 pos = this->getPos();
	pos = pos + v;
	this->setPos(pos);
}

//------------------------------------------------------------------------------
/**
*/
inline float* matrix44::operator[](unsigned int index)
{
	return this->matrix[index];
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::transpose()
{
	matrix44 temp;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			temp[i][j] = this->matrix[j][i];
		}
	}
	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::inverse(matrix44& matrix)
{
	matrix44 temp;
	float a11 = matrix[0][0];		   ///  a11	a12	a13	a14
	float a12 = matrix[0][1];		   ///	a21	a22 a23 a24
	float a13 = matrix[0][2];		   ///	a31	a32	a33	a34
	float a14 = matrix[0][3];		   ///	a41	a42	a43	a44

	float a21 = matrix[1][0];
	float a22 = matrix[1][1];
	float a23 = matrix[1][2];
	float a24 = matrix[1][3];

	float a31 = matrix[2][0];
	float a32 = matrix[2][1];
	float a33 = matrix[2][2];
	float a34 = matrix[2][3];

	float a41 = matrix[3][0];
	float a42 = matrix[3][1];
	float a43 = matrix[3][2];
	float a44 = matrix[3][3];

	float det = a11*a22*a33*a44 + a11*a23*a34*a42 + a11*a24*a32*a43 +
		a12*a21*a34*a43 + a12*a23*a31*a44 + a12*a24*a33*a41 +
		a13*a21*a32*a44 + a13*a22*a34*a41 + a13*a24*a31*a42 +
		a14*a21*a33*a42 + a14*a22*a31*a43 + a14*a23*a32*a41 -
		a11*a22*a34*a43 - a11*a23*a32*a44 - a11*a24*a33*a42 -
		a12*a21*a33*a44 - a12*a23*a34*a41 - a12*a24*a31*a43 -
		a13*a21*a34*a42 - a13*a22*a31*a44 - a13*a24*a32*a41 -
		a14*a21*a32*a43 - a14*a22*a33*a41 - a14*a23*a31*a42;
	if (det == 0)
	{
		return temp;		//Returns a 4x4 unit matrix if det = 0. Though this should never happen
	}
	float k = 1.0f / det;

	temp[0][0] = a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a24*a33*a42;
	temp[0][1] = a12*a34*a43 + a13*a32*a44 + a14*a33*a42 - a12*a33*a44 - a13*a34*a42 - a14*a32*a43;
	temp[0][2] = a12*a23*a44 + a13*a24*a42 + a14*a22*a43 - a12*a24*a43 - a13*a22*a44 - a14*a23*a42;
	temp[0][3] = a12*a24*a33 + a13*a22*a34 + a14*a23*a32 - a12*a23*a34 - a13*a24*a32 - a14*a22*a33;

	temp[1][0] = a21*a34*a43 + a23*a31*a44 + a24*a33*a41 - a21*a33*a44 - a23*a34*a41 - a24*a31*a43;
	temp[1][1] = a11*a33*a44 + a13*a34*a41 + a14*a31*a43 - a11*a34*a43 - a13*a31*a44 - a14*a33*a41;
	temp[1][2] = a11*a24*a43 + a13*a21*a44 + a14*a23*a41 - a11*a23*a44 - a13*a24*a41 - a14*a21*a43;
	temp[1][3] = a11*a23*a34 + a13*a24*a31 + a14*a21*a33 - a11*a24*a33 - a13*a21*a34 - a14*a23*a31;

	temp[2][0] = a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a22*a31*a44 - a24*a32*a41;
	temp[2][1] = a11*a34*a42 + a12*a31*a44 + a14*a32*a41 - a11*a32*a44 - a12*a34*a41 - a14*a31*a42;
	temp[2][2] = a11*a22*a44 + a12*a24*a41 + a14*a21*a42 - a11*a24*a42 - a12*a21*a44 - a14*a22*a41;
	temp[2][3] = a11*a24*a32 + a12*a21*a34 + a14*a22*a31 - a11*a22*a34 - a12*a24*a31 - a14*a21*a32;

	temp[3][0] = a21*a33*a42 + a22*a31*a43 + a23*a32*a41 - a21*a32*a43 - a22*a33*a41 - a23*a31*a42;
	temp[3][1] = a11*a32*a43 + a12*a33*a41 + a13*a31*a42 - a11*a33*a42 - a12*a31*a43 - a13*a32*a41;
	temp[3][2] = a11*a23*a42 + a12*a21*a43 + a13*a22*a41 - a11*a22*a43 - a12*a23*a41 - a13*a21*a42;
	temp[3][3] = a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 - a13*a22*a31;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			temp[j][i] *= k;
		}
	}

	return temp;
}

//------------------------------------------------------------------------------
/**
*/
inline matrix44 matrix44::inverse()
{
	matrix44 temp;
	float a11 = this->matrix[0][0];		   ///  a11	a12	a13	a14
	float a12 = this->matrix[0][1];		   ///	a21	a22 a23 a24
	float a13 = this->matrix[0][2];		   ///	a31	a32	a33	a34
	float a14 = this->matrix[0][3];		   ///	a41	a42	a43	a44

	float a21 = this->matrix[1][0];
	float a22 = this->matrix[1][1];
	float a23 = this->matrix[1][2];
	float a24 = this->matrix[1][3];

	float a31 = this->matrix[2][0];
	float a32 = this->matrix[2][1];
	float a33 = this->matrix[2][2];
	float a34 = this->matrix[2][3];

	float a41 = this->matrix[3][0];
	float a42 = this->matrix[3][1];
	float a43 = this->matrix[3][2];
	float a44 = this->matrix[3][3];

	float det = a11*a22*a33*a44 + a11*a23*a34*a42 + a11*a24*a32*a43 +
				a12*a21*a34*a43 + a12*a23*a31*a44 + a12*a24*a33*a41 +
				a13*a21*a32*a44 + a13*a22*a34*a41 + a13*a24*a31*a42 +
				a14*a21*a33*a42 + a14*a22*a31*a43 + a14*a23*a32*a41 -
				a11*a22*a34*a43 - a11*a23*a32*a44 - a11*a24*a33*a42 -
				a12*a21*a33*a44 - a12*a23*a34*a41 - a12*a24*a31*a43 -
				a13*a21*a34*a42 - a13*a22*a31*a44 - a13*a24*a32*a41 -
				a14*a21*a32*a43 - a14*a22*a33*a41 - a14*a23*a31*a42;
	if (det == 0)
	{
		return temp;		//Returns a 4x4 unit matrix if det = 0. Though this should never happen
	}
	float k = 1.0f / det;

	temp[0][0] = a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a24*a33*a42;
	temp[0][1] = a12*a34*a43 + a13*a32*a44 + a14*a33*a42 - a12*a33*a44 - a13*a34*a42 - a14*a32*a43;
	temp[0][2] = a12*a23*a44 + a13*a24*a42 + a14*a22*a43 - a12*a24*a43 - a13*a22*a44 - a14*a23*a42;
	temp[0][3] = a12*a24*a33 + a13*a22*a34 + a14*a23*a32 - a12*a23*a34 - a13*a24*a32 - a14*a22*a33;

	temp[1][0] = a21*a34*a43 + a23*a31*a44 + a24*a33*a41 - a21*a33*a44 - a23*a34*a41 - a24*a31*a43;
	temp[1][1] = a11*a33*a44 + a13*a34*a41 + a14*a31*a43 - a11*a34*a43 - a13*a31*a44 - a14*a33*a41;
	temp[1][2] = a11*a24*a43 + a13*a21*a44 + a14*a23*a41 - a11*a23*a44 - a13*a24*a41 - a14*a21*a43;
	temp[1][3] = a11*a23*a34 + a13*a24*a31 + a14*a21*a33 - a11*a24*a33 - a13*a21*a34 - a14*a23*a31;

	temp[2][0] = a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a22*a31*a44 - a24*a32*a41;
	temp[2][1] = a11*a34*a42 + a12*a31*a44 + a14*a32*a41 - a11*a32*a44 - a12*a34*a41 - a14*a31*a42;
	temp[2][2] = a11*a22*a44 + a12*a24*a41 + a14*a21*a42 - a11*a24*a42 - a12*a21*a44 - a14*a22*a41;
	temp[2][3] = a11*a24*a32 + a12*a21*a34 + a14*a22*a31 - a11*a22*a34 - a12*a24*a31 - a14*a21*a32;

	temp[3][0] = a21*a33*a42 + a22*a31*a43 + a23*a32*a41 - a21*a32*a43 - a22*a33*a41 - a23*a31*a42;
	temp[3][1] = a11*a32*a43 + a12*a33*a41 + a13*a31*a42 - a11*a33*a42 - a12*a31*a43 - a13*a32*a41;
	temp[3][2] = a11*a23*a42 + a12*a21*a43 + a13*a22*a41 - a11*a22*a43 - a12*a23*a41 - a13*a21*a42;
	temp[3][3] = a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 - a13*a22*a31;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			temp[j][i] *= k;
		}
	}

	return temp;	
}
}
