#pragma once
#include "vector4.h"
#include "ray.h"

namespace Math
{
struct Ray;

class plane
{
public:
	plane();
	plane(vector4& p1, vector4& p2, vector4& p3);
	~plane();
		
	float& a();
	float& b();	
	float& c();	
	float& d();

	float a() const;	
	float b() const;	
	float c() const;	
	float d() const;

	float dot(const vector4& v);

	vector4 getNormal();
	vector4 getPoint();
	bool intersection(vector4& start, vector4& end, vector4& isp);
private:
	vector4 vec;
};

//------------------------------------------------------------------------------
/**
*/
inline plane::plane()
{

}

//------------------------------------------------------------------------------
/**
*/
inline plane::plane(vector4& p1, vector4& p2, vector4& p3)
{
	//create 2 vectors from the same point, in this case p1
	vector4 v21 = p1 - p2;
	vector4 v31 = p1 - p3;

	//get the cross product (the normal) between those vectors, then normalize it
	vector4 cr = vector4::cross3(v21, v31);
	cr = vector4::normalize4(cr);	
	
	this->vec = cr;
	//d is calculated by performing the dot product between the normal and p1 and then flip the sign
	float d = vector4::dot3(cr, p1);
	this->vec.set_w(-d);
}

//------------------------------------------------------------------------------
/**
*/
inline plane::~plane()
{

}

//------------------------------------------------------------------------------
/**
*/
inline float& plane::a()
{
	return this->vec.x();
}

//------------------------------------------------------------------------------
/**
*/
inline float& plane::b()
{
	return this->vec.y();
}

//------------------------------------------------------------------------------
/**
*/
inline float& plane::c()
{
	return this->vec.z();
}

//------------------------------------------------------------------------------
/**
*/
inline float& plane::d()
{
	return this->vec.w();
}

//------------------------------------------------------------------------------
/**
*/
inline float plane::a() const
{
	return this->vec.x();
}

//------------------------------------------------------------------------------
/**
*/
inline float plane::b() const
{
	return this->vec.y();
}

//------------------------------------------------------------------------------
/**
*/
inline float plane::c() const
{
	return this->vec.z();
}

//------------------------------------------------------------------------------
/**
*/
inline float plane::d() const
{
	return this->vec.w();
}

//------------------------------------------------------------------------------
/**
*/
inline float plane::dot(const vector4& v)
{
	return vector4::dot4(this->vec, v.vec);
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 plane::getNormal()
{
	vector4 norm = this->vec;
	norm.set_w(0);
	return norm;
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 plane::getPoint()
{
	return this->getNormal() * -this->d();
}

//------------------------------------------------------------------------------
/**
*/
inline bool plane::intersection(vector4& start, vector4& end, vector4& isp)
{	
	vector4 p0 = this->getPoint();

	vector4 u = end - start;
	vector4 w = start - p0;

	float _d = vector4::dot3(this->vec, u);
	float _n = -vector4::dot3(this->vec, w);

	if(abs(_d) < 0.000001f)
	{
		return false;
	}
	float s = _n / _d;

	if(s < 0 || s > 1)
	{
		return false;
	}
	isp = start + (u * s);
	return true;

	//float v1 = vector4::dot3(this->vec, start);
	//float v2 = vector4::dot3(this->vec, end);

	//float d = v1 - v2;

	////checking if the lines are parallel, if not there is 1 intersection point
	//if(abs(d) < 0.000001f)
	//{
	//	return false;
	//}

	//d = 1.0f / d;

	//float pd = this->dot(start);
	//pd *= d;
	//vector4 p = end - start;
	//p = p * pd;
	//p += start;
	//isp = p;

	return true;
}


}
