/******************************************************************************/
/*!
\file   Vector4.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Vector4's template class
*/
/******************************************************************************/
#pragma once
#include "Macro.h"
#include <iostream>

jeBegin

// Vector4 struct
struct Vector4
{
	// Memver variables
	float x, y, z, w;

	// Constructors and destructor
	Vector4(float _x = 0, float _y = 0, float _z = 0, float _w = 0);
	Vector4(const Vector4& copy);

	// Static variables
	static const Vector4 ZERO;
	static const Vector4 ONE;
	static const Vector4 UNIT_X;
	static const Vector4 UNIT_Y;
	static const Vector4 UNIT_Z;
	static const Vector4 UNIT_W;

	// Operators
	bool		operator<(const Vector4& rhs) const;
	bool		operator==(const Vector4& rhs) const;
	bool		operator!=(const Vector4& rhs) const;
	Vector4		operator+(float constant) const;
	Vector4		operator+(const Vector4& rhs) const;
	Vector4		operator-(float constant) const;
	Vector4		operator-(const Vector4& rhs) const;
	Vector4		operator*(float constant) const;
	Vector4		operator*(const Vector4& rhs) const;
	Vector4		operator/(float constant) const;
	Vector4&	operator=(const Vector4& rhs);
	Vector4		operator-(void) const;
	Vector4&	operator+=(const Vector4& rhs);
	Vector4&	operator+=(float constant);
	Vector4&	operator-=(const Vector4& rhs);
	Vector4&	operator-=(float constant);
	Vector4&	operator*=(const Vector4& rhs);
	Vector4&	operator*=(float constant);
	Vector4&	operator/=(float constant);

	// Trasnforming functions and helpers
	void		Set(const Vector4& copy);
	void		Set(float _x, float _y, float _z, float _w);
	void		SetZero();
	void		SetOne();
	void		SetUnitX();
	void		SetUnitY();
	void		SetUnitZ();
	void		SetUnitW();

	// Friend functions
	friend Vector4 operator+(float constant, Vector4& rhs);
	friend Vector4 operator*(float constant, Vector4& rhs);
	friend std::ostream& operator<<(std::ostream& os, const Vector4& contents);

};

using vec4 = Vector4;
using CR_Vec4 = const vec4&;

jeEnd
