/******************************************************************************/
/*!
\file   Matrix4x4.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Matrix4x4's template class and member function
Custom Matrix looks like this...

		  0  1  2  3
	0	[ 00 01 02 03 ]
	1	[ 10 11 12 13 ]
	2	[ 20 21 22 23 ]
	3	[ 30 31 32 33 ]

*/
/******************************************************************************/

#include "MathUtils.h"

jeBegin

/******************************************************************************/
/*!
* \brief - Matrix4x4 Constructor
* \param _diagonal
*/
/******************************************************************************/
Matrix4x4::Matrix4x4(float diagonal)
{
	Math::SetIdentity(*this, diagonal);
}

/******************************************************************************/
/*!
* \brief - Matrix4x4 Copy Constructor
* \param rhs 
*/
/******************************************************************************/
Matrix4x4::Matrix4x4(CR_Mat4 rhs)
{
	if (this != &rhs)
	{
		for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = rhs.m[i][j];
	}
}

/******************************************************************************/
/*!
* \brief - Matrix4x4 Copy Constructor
* \param member1 ~ member16
* 
* 01 02 03 04 
* 05 06 07 08
* 09 10 11 12
* 13 14 15 16
*/
/******************************************************************************/
Matrix4x4::Matrix4x4(float member1, float member2, float member3, float member4, 
	float member5, float member6, float member7, float member8, 
	float member9, float member10, float member11, float member12, 
	float member, float member14, float member15, float member16)
{
	m[0][0] = member1;
	m[0][1] = member2;
	m[0][2] = member3;
	m[0][3] = member4;

	m[1][0] = member5;
	m[1][1] = member6;
	m[1][2] = member7;
	m[1][3] = member8;

	m[2][0] = member9;
	m[2][1] = member10;
	m[2][2] = member11;
	m[2][3] = member12;

	m[3][0] = member;
	m[3][1] = member14;
	m[3][2] = member15;
	m[3][3] = member16;
}

/******************************************************************************/
/*!
* \brief - Matrix4x4 = operator 
* \param rhs - matrix to assign
* \return *this
*/
/******************************************************************************/
Matrix4x4& Matrix4x4::operator = (CR_Mat4 rhs)
{
	if (this != &rhs)
	{
		for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = rhs.m[i][j];
	}

	return *this;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 - unary operator
\return *this
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator-(void) const
{
	mat4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] = -result.m[i][j];

	return result;
}

void Matrix4x4::operator+=(CR_Mat4 rhs)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] += rhs.m[i][j];
}

void Matrix4x4::operator*=(CR_Mat4 rhs)
{
	(*this) = (*this)*rhs;
}

void Matrix4x4::operator-=(CR_Mat4 rhs)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] -= rhs.m[i][j];
}

void Matrix4x4::operator+=(float constant)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] += constant;
}

void Matrix4x4::operator-=(float constant)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] -= constant;
}

void Matrix4x4::operator*=(float constant)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] *= constant;
}

void Matrix4x4::operator/=(float constant)
{
	if (constant)
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i][j] /= constant;

	else
		jeDebugPrint("!Matrix4x4 - Cannot devide by 0.\n");
}

/******************************************************************************/
/*!
\brief - Matrix4x4 + operator
\param rhs - matrix to add
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator + (CR_Mat4 rhs) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] += rhs.m[i][j];

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 + operator
\param constant - number to add
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator + (float constant) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] += constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 - operator
\param rhs - matrix to subtract
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator - (CR_Mat4 rhs) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] -= rhs.m[i][j];

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 - operator
\param constant - number to subtract
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator - (float constant) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] -= constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 * operator
\param constant - number to multiply
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator * (float constant) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] *= constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 * operator
\param rhs - matrix to multiply
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator*(CR_Mat4 rhs) const
{
	Matrix4x4 result;

	result.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
	result.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
	result.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
	result.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];

	result.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
	result.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
	result.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
	result.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];

	result.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
	result.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
	result.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
	result.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];

	result.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];
	result.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
	result.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];
	result.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 * operator with vector4
\param rhs - vector4 to multiply
\return result
*/
/******************************************************************************/
Vector4 Matrix4x4::operator*(CR_Vec4 rhs) const
{
	Vector4 result;

	result.x = m[0][0] * rhs.x + m[0][1] * rhs.y + m[0][2] * rhs.z + m[0][3] * rhs.w;
	result.y = m[1][0] * rhs.x + m[1][1] * rhs.y + m[1][2] * rhs.z + m[1][3] * rhs.w;
	result.z = m[2][0] * rhs.x + m[2][1] * rhs.y + m[2][2] * rhs.z + m[2][3] * rhs.w;
	result.w = m[3][0] * rhs.x + m[3][1] * rhs.y + m[3][2] * rhs.z + m[3][3] * rhs.w;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 / operator 
\param constant - number to divide
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator / (float constant) const
{
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] = m[i][j] / constant;

	return result;
}

void Matrix4x4::Set(float member1, float member2, float member3, float member4, 
	float member5, float member6, float member7, float member8, 
	float member9, float member10, float member11, float member12, 
	float member, float member14, float member15, float member16)
{
	m[0][0] = member1;
	m[0][1] = member2;
	m[0][2] = member3;
	m[0][3] = member4;

	m[1][0] = member5;
	m[1][1] = member6;
	m[1][2] = member7;
	m[1][3] = member8;

	m[2][0] = member9;
	m[2][1] = member10;
	m[2][2] = member11;
	m[2][3] = member12;

	m[3][0] = member;
	m[3][1] = member14;
	m[3][2] = member15;
	m[3][3] = member16;
}

void Matrix4x4::SetZero()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = 0.f;
}

/******************************************************************************/
/*!
\brief - Friend function + operator
\param constant - number to add
\param rhs - this
\return result
*/
/******************************************************************************/
Matrix4x4 operator+(float constant, CR_Mat4 rhs)
{
	Matrix4x4 result;

	result = rhs + constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Friend function * operator
\param constant - number to multiply
\param rhs - this
\return result
*/
/******************************************************************************/
Matrix4x4 operator*(float constant, CR_Mat4 rhs)
{
	Matrix4x4 result;
	result = rhs * constant;
	return result;
}

Vector4 operator*(CR_Vec4 _vector, CR_Mat4 rhs)
{
	return rhs * _vector;
}

/******************************************************************************/
/*!
\brief - Friend function << operator
\param _os - storage to put contents
\param _contents - contents to print out
\return os
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& _os, CR_Mat4 _contents)
{
	for (int i = 0; i < 4; ++i)	// row
	{
		_os << "[ ";
		for (int j = 0; j < 4; ++j)	// column
		{
			_os.setf(std::ios_base::showpoint);
			_os << _contents.m[i][j] << " ";
		}
		_os << "]\n";
	}

	return _os;
}

jeEnd
