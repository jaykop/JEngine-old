/******************************************************************************/
/*!
\file   Vector3.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Vector3's template class


*/
/******************************************************************************/

#pragma once
#include "Macro.h"
#include <iostream>

jeBegin

// Vector3 struct
struct Vector3
{
	// Member variables
	float x = 0.f, y = 0.f, z = 0.f;

	// Constructor and destructor
	Vector3(float _x = 0, float _y = 0, float _z = 0);
	Vector3(const Vector3& copy);

	// Static variables
	static const Vector3 ZERO;
	static const Vector3 ONE;
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;

	// Operators
	bool		operator<(const Vector3& rhs) const;
	bool		operator==(const Vector3& rhs) const;
	bool		operator!=(const Vector3& rhs) const;
	Vector3		operator+(float constant) const;
	Vector3		operator+(const Vector3& rhs) const;
	Vector3		operator-(float constant) const;
	Vector3		operator-(const Vector3& rhs) const;
	Vector3		operator*(float constant) const;
	Vector3		operator*(const Vector3& rhs) const;
	Vector3		operator/(float constant) const;
	Vector3&	operator=(const Vector3& rhs);
	Vector3		operator-(void) const;
	Vector3&	operator+=(const Vector3& rhs);
	Vector3&	operator+=(float constant);
	Vector3&	operator-=(const Vector3& rhs);
	Vector3&	operator-=(float constant);
	Vector3&	operator*=(const Vector3& rhs);
	Vector3&	operator*=(float constant);
	Vector3&	operator/=(float constant);

	// Transforming functions
	void		Set(const Vector3& copy);
	void		Set(float _x, float _y, float _z);
	void		SetZero();
	void		SetOne();
	void		SetUnitX();
	void		SetUnitY();
	void		SetUnitZ();

	// Friend Functions
	friend Vector3			operator+(float constant, const Vector3& rhs);
	friend Vector3			operator*(float constant, const Vector3& rhs);
	friend std::ostream&	operator<<(std::ostream& _os, const Vector3& _constents);

};

using vec3 = Vector3;
using CR_Vec3 = const vec3&;

jeEnd
