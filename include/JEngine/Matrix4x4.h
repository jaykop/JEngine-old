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
#include "Vector3.h"
#include "Vector4.h"

jeBegin

// Matrix 4x4 template class
class Matrix4x4
{
public:

	//! cons and des
	~Matrix4x4(void);

	// Copy cons
	Matrix4x4(float element = 0);
	Matrix4x4(const Matrix4x4& _rhs);
	Matrix4x4(float _member1, float _member2, float _member3, float _member4,
		float _member5, float _member6, float _member7, float _member8,
		float _member9, float _member10, float _member11, float _member12,
		float _member13, float _member14, float _member15, float _member16);

	// Operators
	Matrix4x4& operator=(const Matrix4x4& _rhs);
	Matrix4x4& operator-(void);

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

	// Transforming functions
	Matrix4x4	GetTranspose(void) const;
	Matrix4x4&	Transpose(void);
	Matrix4x4	GetInverse() const;
	Matrix4x4&	Inverse();
	void		SetIdentity(void);
	void		Set(float _member1, float _member2, float _member3, float _member4,
				float _member5, float _member6, float _member7, float _member8, 
				float _member9, float _member10, float _member11, float _member12, 
				float _member13, float _member14, float _member15, float _member16);
	void		SetZero();

	// To use graphic...
	static Matrix4x4 Translate(const vec3& _vec);
	static Matrix4x4 Scale(const vec3& _vec);
	static Matrix4x4 Rotate(float _radian, const vec3& _vec);
	static Matrix4x4 RotateX(float _radian);
	static Matrix4x4 RotateY(float _radian);
	static Matrix4x4 RotateZ(float _radian);
	static Matrix4x4 Perspective(float _fovy, float _aspect, float _zNear, float _zFar);
	static Matrix4x4 Orthogonal(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar);
	static Matrix4x4 Orthogonal(float _left, float _right, float _bottom, float _top);
	static Matrix4x4 LookAt(const Vector3& _eye, const Vector3& _target, const Vector3& _up);

	//Friend functions
	friend Matrix4x4		operator+(float _constant, const Matrix4x4& _rhs);
	friend Matrix4x4		operator*(float _constant, const Matrix4x4& _rhs);
	friend Vector4			operator*(const Vector4& _vector, const Matrix4x4& _rhs);
	friend std::ostream&	operator<<(std::ostream& _os, const Matrix4x4& _contents);

	float m[4][4];

private:

};

using mat4 = Matrix4x4;
using CR_mat4 = const mat4&;

jeEnd
