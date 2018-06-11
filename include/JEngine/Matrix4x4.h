/******************************************************************************/
/*!
\file   Matrix4x4.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Matrix4x4's template class and members
Custom Matrix looks like this...

		  0  1  2  3
	0	[ 00 01 02 03 ]
	1	[ 10 11 12 13 ]
	2	[ 20 21 22 23 ]
	3	[ 30 31 32 33 ]

*/
/******************************************************************************/

#pragma once
#include "Vector4.h"

jeBegin

// Matrix 4x4 template struct
struct Matrix4x4
{
	float m[4][4];

	// Copy cons
	Matrix4x4(float _element = 0);
	Matrix4x4(const Matrix4x4& _rhs);
	Matrix4x4(float _member1, float _member2, float _member3, float _member4,
		float _member5, float _member6, float _member7, float _member8,
		float _member9, float _member10, float _member11, float _member12,
		float _member13, float _member14, float _member15, float _member16);

	// Operators
	Matrix4x4&	operator=(const Matrix4x4& _rhs);
	Matrix4x4	operator-(void) const;

	void operator+=(const Matrix4x4& _rhs);
	void operator*=(const Matrix4x4& _rhs);
	void operator-=(const Matrix4x4& _rhs);

	void operator+=(float _constant);
	void operator-=(float _constant);
	void operator*=(float _constant);
	void operator/=(float _constant);

	Vector4	  operator*(const vec4& _rhs) const;
	Matrix4x4 operator+(const Matrix4x4& _rhs) const;
	Matrix4x4 operator*(const Matrix4x4& _rhs) const;
	Matrix4x4 operator-(const Matrix4x4& _rhs) const;

	Matrix4x4 operator+(float _constant) const;
	Matrix4x4 operator-(float _constant) const;
	Matrix4x4 operator*(float _constant) const;
	Matrix4x4 operator/(float _constant) const;

	void Set(float _member1, float _member2, float _member3, float _member4,
				float _member5, float _member6, float _member7, float _member8, 
				float _member9, float _member10, float _member11, float _member12, 
				float _member13, float _member14, float _member15, float _member16);
	void SetZero();

	//Friend functions
	friend Matrix4x4		operator+(float _constant, const Matrix4x4& _rhs);
	friend Matrix4x4		operator*(float _constant, const Matrix4x4& _rhs);
	friend Vector4			operator*(const Vector4& _vector, const Matrix4x4& _rhs);
	friend std::ostream&	operator<<(std::ostream& _os, const Matrix4x4& _contents);

};

using mat4 = Matrix4x4;
using CR_Mat4 = const mat4&;

jeEnd
