/******************************************************************************/
/*!
\file   Matrix2x2.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Matrix4x4's class and members
Custom Matrix looks like this...

0  1
0	[ 00 10 ]
1	[ 01 11 ]

*/
/******************************************************************************/

#pragma once
#include "Macro.h"
#include "Vector2.h"
#include <iostream>

JE_BEGIN

//! Matrix2x2 struct
struct Matrix2x2
{
	//! Destructor
	~Matrix2x2(void);

	//! Constructors
	Matrix2x2(float _element = 0);
	Matrix2x2(float _member1, float _member2,
		float _member3, float _member4);
	Matrix2x2(const Matrix2x2& rhs);

	// Operators
	Matrix2x2&	operator=(const Matrix2x2& rhs);
	Matrix2x2&	operator-(void);

	Matrix2x2	operator+(const Matrix2x2& _rhs) const;
	Matrix2x2	operator*(const Matrix2x2& rhs) const;
	Vector2		operator*(const vec2& rhs) const;
	Matrix2x2	operator-(const Matrix2x2& rhs) const;

	Matrix2x2	operator+(float _constant) const;
	Matrix2x2	operator-(float _constant) const;
	Matrix2x2	operator*(float _constant) const;
	Matrix2x2	operator/(float _constant) const;

	// Transforming functions
	Matrix2x2	GetTranspose(void) const;
	Matrix2x2&	Transpose(void);
	Matrix2x2	GetInverse() const;
	Matrix2x2&	Inverse();
	void		SetIdentity(void);
	void		Set(float _member1, float _member2,
				float _member3, float _member4);
	void		SetZero();

	// Frriend functions
	friend Matrix2x2		operator+(float _constant, const Matrix2x2& rhs);
	friend Matrix2x2		operator*(float _constant, const Matrix2x2& rhs);
	friend std::ostream&	operator<<(std::ostream& _os, const Matrix2x2& _contents);

	float m[2][2];
};

using mat2 = Matrix2x2;
using CR_mat2 = const mat2&;

JE_END
