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
	Matrix4x4(float diagonal = 0);
	Matrix4x4(const Matrix4x4& rhs);
	Matrix4x4(float member1, float member2, float member3, float member4,
		float member5, float member6, float member7, float member8,
		float member9, float member10, float member11, float member12,
		float member, float member14, float member15, float member16);

	// Operators
	Matrix4x4&	operator=(const Matrix4x4& rhs);
	Matrix4x4	operator-(void) const;

	void operator+=(const Matrix4x4& rhs);
	void operator*=(const Matrix4x4& rhs);
	void operator-=(const Matrix4x4& rhs);

	void operator+=(float constant);
	void operator-=(float constant);
	void operator*=(float constant);
	void operator/=(float constant);

	Vector4	  operator*(const vec4& rhs) const;
	Matrix4x4 operator+(const Matrix4x4& rhs) const;
	Matrix4x4 operator*(const Matrix4x4& rhs) const;
	Matrix4x4 operator-(const Matrix4x4& rhs) const;

	Matrix4x4 operator+(float constant) const;
	Matrix4x4 operator-(float constant) const;
	Matrix4x4 operator*(float constant) const;
	Matrix4x4 operator/(float constant) const;

	void Set(float member1, float member2, float member3, float member4,
				float member5, float member6, float member7, float member8, 
				float member9, float member10, float member11, float member12, 
				float member, float member14, float member15, float member16);
	void SetZero();

	//Friend functions
	friend Matrix4x4		operator+(float constant, const Matrix4x4& rhs);
	friend Matrix4x4		operator*(float constant, const Matrix4x4& rhs);
	friend Vector4			operator*(const Vector4& _vector, const Matrix4x4& rhs);
	friend std::ostream&	operator<<(std::ostream& _os, const Matrix4x4& _contents);

};

using mat4 = Matrix4x4;
using CR_Mat4 = const mat4&;

jeEnd
