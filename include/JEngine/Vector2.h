/******************************************************************************/
/*!
\file   Vector2.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Vector2's class

*/
/******************************************************************************/

#pragma once
#include "Macro.h"
#include <iostream>

jeBegin

// vector2 struct
struct Vector2
{
	// Constructors and destructor
	Vector2(float _x = 0, float _y = 0);
	Vector2(const Vector2& _copy);
	~Vector2();

	// Operators
	bool		operator<(const Vector2& _rhs) const;
	bool		operator==(const Vector2& _rhs) const;
	bool		operator!=(const Vector2& _rhs) const;
	Vector2		operator+(float _constant) const;
	Vector2		operator+(const Vector2& _rhs) const;
	Vector2		operator-(float _constant) const;
	Vector2		operator-(const Vector2& _rhs) const;
	Vector2		operator*(float _constant) const;
	Vector2		operator*(const Vector2& _rhs) const;
	Vector2		operator/(float _constant) const;
	Vector2&	operator=(const Vector2& _rhs);
	Vector2		operator-(void);
	Vector2&	operator+=(const Vector2& _rhs);
	Vector2&	operator+=(float _constant);
	Vector2&	operator-=(const Vector2& _rhs);
	Vector2&	operator-=(float _constant);
	Vector2&	operator*=(const Vector2& _rhs);
	Vector2&	operator*=(float _constant);
	Vector2&	operator/=(float _constant);

	// Transform functions
	void		Set(const Vector2& _copy);
	void		Set(float _x, float _y);
	void		SetZero();
	void		SetOne();
	void		SetUnitX();
	void		SetUnitY();
	bool		IsZero() const;
	bool		IsOne() const;
	float		GetLength() const;
	float		GetLengthSq() const;
	float		DotProduct(const Vector2& _rhs) const;
	Vector2		CrossProduct(const Vector2& _rhs) const;
	Vector2&	Normalize();
	Vector2		GetNormalize() const;
	Vector2&	Absolute();
	Vector2		GetAbsolute() const;
	float		GetAngle() const;

	// Friend functions
	friend Vector2			operator+(float _constant, const Vector2& _rhs);
	friend Vector2			operator*(float _constant, const Vector2& _rhs);
	friend std::ostream&	operator<<(std::ostream& _os, const Vector2& _constents);

	// Static variables
	static const Vector2 ZERO;
	static const Vector2 ONE;
	static const Vector2 UNIT_X;
	static const Vector2 UNIT_Y;

	// Memver variables
	float x, y;
};

using vec2 = Vector2;
using CR_vec2 = const vec2&;

jeEnd
