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

JE_BEGIN

// class template Vector4
class Vector4
{
public:

	// Constructors and destructor
	~Vector4(void);
	Vector4(float _x = 0, float _y = 0, float _z = 0, float _w = 0);
	Vector4(const Vector4& _copy);

	// Operators
	bool		operator<(const Vector4& _rhs) const;
	bool		operator==(const Vector4& _rhs) const;
	bool		operator!=(const Vector4& _rhs) const;
	Vector4		operator+(float _constant) const;
	Vector4		operator+(const Vector4& _rhs) const;
	Vector4		operator-(float _constant) const;
	Vector4		operator-(const Vector4& _rhs) const;
	Vector4		operator*(float _constant) const;
	Vector4		operator*(const Vector4& _rhs) const;
	Vector4		operator/(float _constant) const;
	Vector4&	operator=(const Vector4& _rhs);
	Vector4&	operator-(void);
	Vector4&	operator+=(const Vector4& _rhs);
	Vector4&	operator+=(float _constant);
	Vector4&	operator-=(const Vector4& _rhs);
	Vector4&	operator-=(float _constant);
	Vector4&	operator*=(const Vector4& _rhs);
	Vector4&	operator*=(float _constant);
	Vector4&	operator/=(float _constant);

	// Trasnforming functions and helpers
	void		Set(const Vector4& _copy);
	void		Set(float _x, float _y, float _z, float _w);
	void		SetZero();
	void		SetOne();
	void		SetUnitX();
	void		SetUnitY();
	void		SetUnitZ();
	void		SetUnitW();
	bool		IsZero() const;
	bool		IsOne() const;
	float		Length() const;
	float		LengthSq() const;
	float		DotProduct(const Vector4& _rhs);
	Vector4&	Normalize();
	Vector4		GetNormalize() const;
	Vector4&	Absolute();
	Vector4		GetAbsolute() const;

	// Friend functions
	friend Vector4 operator+(float constant, Vector4& rhs);
	friend Vector4 operator*(float constant, Vector4& rhs);
	friend std::ostream& operator<<(std::ostream& os, const Vector4& contents);

	// Static variables
	static const Vector4 ZERO;
	static const Vector4 ONE;
	static const Vector4 UNIT_X;
	static const Vector4 UNIT_Y;
	static const Vector4 UNIT_Z;
	static const Vector4 UNIT_W;

	// Memver variables
	float x, y, z, w;

private:
	
};

using vec4 = Vector4;
using CR_vec4 = const vec4&;

JE_END
