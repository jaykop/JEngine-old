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
0	[ 00 10 20 30 ]
1	[ 01 11 21 31 ]
2	[ 02 12 22 32 ]
3	[ 03 13 23 33 ]


*/
/******************************************************************************/

#pragma once
#include "Macro.h"
#include "Vector3.h"
#include "Vector4.h"

NS_JE_BEGIN

// Matrix 4x4 template class
class Matrix4x4
{
public:

	//! cons and des
	~Matrix4x4(void);

	// Copy cons
	Matrix4x4(float element = 0);
	Matrix4x4(const Matrix4x4& rhs);
	Matrix4x4(float _member1, float _member2, float _member3, float _member4,
		float _member5, float _member6, float _member7, float _member8,
		float _member9, float _member10, float _member11, float _member12,
		float _member13, float _member14, float _member15, float _member16);

	// Operators
	Matrix4x4& operator=(const Matrix4x4& rhs);
	Matrix4x4& operator-(void);

	void operator+=(const Matrix4x4& rhs);
	void operator*=(const Matrix4x4& rhs);
	void operator-=(const Matrix4x4& rhs);

	void operator+=(float constant);
	void operator-=(float constant);
	void operator*=(float constant);
	void operator/=(float constant);

	Vector4	  operator*(const Vector4& rhs) const;
	Matrix4x4 operator+(const Matrix4x4& rhs) const;
	Matrix4x4 operator*(const Matrix4x4& rhs) const;
	Matrix4x4 operator-(const Matrix4x4& rhs) const;

	Matrix4x4 operator+(float constant) const;
	Matrix4x4 operator-(float constant) const;
	Matrix4x4 operator*(float constant) const;
	Matrix4x4 operator/(float constant) const;

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
	static Matrix4x4 Translate(const Vector3& vec);
	static Matrix4x4 Scale(const Vector3& vec);
	static Matrix4x4 Rotate(float degree, const Vector3& vec);
	static Matrix4x4 Perspective(float fovy, float aspect, float zNear, float zFar);
	static Matrix4x4 Orthogonal(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4x4 Camera(const Vector3 eye, const Vector3 center, const Vector3 up);

	//Friend functions
	friend Matrix4x4		operator+(float constant, const Matrix4x4& rhs);
	friend Matrix4x4		operator*(float constant, const Matrix4x4& rhs);
	friend std::ostream&	operator<<(std::ostream& os, const Matrix4x4& contents);

	float m_member[4][4];

private:

};

typedef Matrix4x4 mat4;

NS_JE_END
