#pragma once
#include "math/vector4.h"
#include <vector>

namespace Math
{
class simplex
{
public:

	simplex();
	~simplex();

	void add(vector4& point);
	vector4& last();
	unsigned int size();
	void remove_a();
	void remove_b();
	void remove_c();
	void remove_d();

	vector4& a();
	vector4& b();
	vector4& c();
	vector4& d();
	void printSimplex();
	void clear();

private:
	std::vector<vector4> supportPoints;
};

inline simplex::simplex()
{
	
}

inline simplex::~simplex()
{
	
}

inline void simplex::add(vector4& point)
{
	this->supportPoints.push_back(point);
}

inline vector4& simplex::last()
{
	return this->supportPoints[this->size() - 1];
}

inline unsigned simplex::size()
{
	return this->supportPoints.size();
}

inline void simplex::remove_a()
{
	this->supportPoints.erase(this->supportPoints.begin() + (this->size() - 1));
}

inline void simplex::remove_b()
{
	this->supportPoints.erase(this->supportPoints.begin() + (this->size() - 2));
}

inline void simplex::remove_c()
{
	this->supportPoints.erase(this->supportPoints.begin() + (this->size() - 3));
}

inline void simplex::remove_d()
{
	this->supportPoints.erase(this->supportPoints.begin() + (this->size() - 4));
}

inline vector4& simplex::a()
{	
	return this->supportPoints[this->size() - 1];
}

inline vector4& simplex::b()
{
	return this->supportPoints[this->size() - 2];
}

inline vector4& simplex::c()
{
	return this->supportPoints[this->size() - 3];
}

inline vector4& simplex::d()
{
	return this->supportPoints[this->size() - 4];
}

inline void simplex::printSimplex()
{
	printf("------------ \n");
	for (unsigned int i = 0; i < this->size(); i++)
	{
		printf("P%i: %f, %f, %f \n", i, this->supportPoints[i].x(), this->supportPoints[i].y(), this->supportPoints[i].z());
	}	
}

inline void simplex::clear()
{
	this->supportPoints.clear();
}
}
