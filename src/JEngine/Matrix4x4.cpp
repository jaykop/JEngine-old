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
Matrix4x4::Matrix4x4(float _diagonal)
{
	Math::SetIdentity(*this, _diagonal);
}

/******************************************************************************/
/*!
* \brief - Matrix4x4 Copy Constructor
* \param _rhs 
*/
/******************************************************************************/
Matrix4x4::Matrix4x4(CR_Mat4 _rhs)
{
	if (this != &_rhs)
	{
		for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = _rhs.m[i][j];
	}
}

/******************************************************************************/
/*!
* \brief - Matrix4x4 Copy Constructor
* \param _member1 ~ _member16
* 
* 01 02 03 04 
* 05 06 07 08
* 09 10 11 12
* 13 14 15 16
*/
/******************************************************************************/
Matrix4x4::Matrix4x4(float _member1, float _member2, float _member3, float _member4, 
	float _member5, float _member6, float _member7, float _member8, 
	float _member9, float _member10, float _member11, float _member12, 
	float _member13, float _member14, float _member15, float _member16)
{
	m[0][0] = _member1;
	m[0][1] = _member2;
	m[0][2] = _member3;
	m[0][3] = _member4;

	m[1][0] = _member5;
	m[1][1] = _member6;
	m[1][2] = _member7;
	m[1][3] = _member8;

	m[2][0] = _member9;
	m[2][1] = _member10;
	m[2][2] = _member11;
	m[2][3] = _member12;

	m[3][0] = _member13;
	m[3][1] = _member14;
	m[3][2] = _member15;
	m[3][3] = _member16;
}

/******************************************************************************/
/*!
* \brief - Matrix4x4 = operator 
* \param _rhs - matrix to assign
* \return *this
*/
/******************************************************************************/
Matrix4x4& Matrix4x4::operator = (CR_Mat4 _rhs)
{
	if (this != &_rhs)
	{
		for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = _rhs.m[i][j];
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
\param _constant - number to add
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator + (float _constant) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] += _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 - operator
\param _rhs - matrix to subtract
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator - (CR_Mat4 _rhs) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] -= _rhs.m[i][j];

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 - operator
\param _constant - number to subtract
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator - (float _constant) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] -= _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 * operator
\param _constant - number to multiply
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator * (float _constant) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] *= _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 * operator
\param _rhs - matrix to multiply
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator*(CR_Mat4 _rhs) const
{
	Matrix4x4 result;

	result.m[0][0] = m[0][0] * _rhs.m[0][0] + m[0][1] * _rhs.m[1][0] + m[0][2] * _rhs.m[2][0] + m[0][3] * _rhs.m[3][0];
	result.m[0][1] = m[0][0] * _rhs.m[0][1] + m[0][1] * _rhs.m[1][1] + m[0][2] * _rhs.m[2][1] + m[0][3] * _rhs.m[3][1];
	result.m[0][2] = m[0][0] * _rhs.m[0][2] + m[0][1] * _rhs.m[1][2] + m[0][2] * _rhs.m[2][2] + m[0][3] * _rhs.m[3][2];
	result.m[0][3] = m[0][0] * _rhs.m[0][3] + m[0][1] * _rhs.m[1][3] + m[0][2] * _rhs.m[2][3] + m[0][3] * _rhs.m[3][3];

	result.m[1][0] = m[1][0] * _rhs.m[0][0] + m[1][1] * _rhs.m[1][0] + m[1][2] * _rhs.m[2][0] + m[1][3] * _rhs.m[3][0];
	result.m[1][1] = m[1][0] * _rhs.m[0][1] + m[1][1] * _rhs.m[1][1] + m[1][2] * _rhs.m[2][1] + m[1][3] * _rhs.m[3][1];
	result.m[1][2] = m[1][0] * _rhs.m[0][2] + m[1][1] * _rhs.m[1][2] + m[1][2] * _rhs.m[2][2] + m[1][3] * _rhs.m[3][2];
	result.m[1][3] = m[1][0] * _rhs.m[0][3] + m[1][1] * _rhs.m[1][3] + m[1][2] * _rhs.m[2][3] + m[1][3] * _rhs.m[3][3];

	result.m[2][0] = m[2][0] * _rhs.m[0][0] + m[2][1] * _rhs.m[1][0] + m[2][2] * _rhs.m[2][0] + m[2][3] * _rhs.m[3][0];
	result.m[2][1] = m[2][0] * _rhs.m[0][1] + m[2][1] * _rhs.m[1][1] + m[2][2] * _rhs.m[2][1] + m[2][3] * _rhs.m[3][1];
	result.m[2][2] = m[2][0] * _rhs.m[0][2] + m[2][1] * _rhs.m[1][2] + m[2][2] * _rhs.m[2][2] + m[2][3] * _rhs.m[3][2];
	result.m[2][3] = m[2][0] * _rhs.m[0][3] + m[2][1] * _rhs.m[1][3] + m[2][2] * _rhs.m[2][3] + m[2][3] * _rhs.m[3][3];

	result.m[3][0] = m[3][0] * _rhs.m[0][0] + m[3][1] * _rhs.m[1][0] + m[3][2] * _rhs.m[2][0] + m[3][3] * _rhs.m[3][0];
	result.m[3][1] = m[3][0] * _rhs.m[0][1] + m[3][1] * _rhs.m[1][1] + m[3][2] * _rhs.m[2][1] + m[3][3] * _rhs.m[3][1];
	result.m[3][2] = m[3][0] * _rhs.m[0][2] + m[3][1] * _rhs.m[1][2] + m[3][2] * _rhs.m[2][2] + m[3][3] * _rhs.m[3][2];
	result.m[3][3] = m[3][0] * _rhs.m[0][3] + m[3][1] * _rhs.m[1][3] + m[3][2] * _rhs.m[2][3] + m[3][3] * _rhs.m[3][3];

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 * operator with vector4
\param _rhs - vector4 to multiply
\return result
*/
/******************************************************************************/
Vector4 Matrix4x4::operator*(CR_Vec4 _rhs) const
{
	Vector4 result;

	result.x = m[0][0] * _rhs.x + m[0][1] * _rhs.y + m[0][2] * _rhs.z + m[0][3] * _rhs.w;
	result.y = m[1][0] * _rhs.x + m[1][1] * _rhs.y + m[1][2] * _rhs.z + m[1][3] * _rhs.w;
	result.z = m[2][0] * _rhs.x + m[2][1] * _rhs.y + m[2][2] * _rhs.z + m[2][3] * _rhs.w;
	result.w = m[3][0] * _rhs.x + m[3][1] * _rhs.y + m[3][2] * _rhs.z + m[3][3] * _rhs.w;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 / operator 
\param _constant - number to divide
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator / (float _constant) const
{
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m[i][j] = m[i][j] / _constant;

	return result;
}

void Matrix4x4::Set(float _member1, float _member2, float _member3, float _member4, 
	float _member5, float _member6, float _member7, float _member8, 
	float _member9, float _member10, float _member11, float _member12, 
	float _member13, float _member14, float _member15, float _member16)
{
	m[0][0] = _member1;
	m[0][1] = _member2;
	m[0][2] = _member3;
	m[0][3] = _member4;

	m[1][0] = _member5;
	m[1][1] = _member6;
	m[1][2] = _member7;
	m[1][3] = _member8;

	m[2][0] = _member9;
	m[2][1] = _member10;
	m[2][2] = _member11;
	m[2][3] = _member12;

	m[3][0] = _member13;
	m[3][1] = _member14;
	m[3][2] = _member15;
	m[3][3] = _member16;
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
\param _constant - number to add
\param _rhs - this
\return result
*/
/******************************************************************************/
Matrix4x4 operator+(float _constant, CR_Mat4 _rhs)
{
	Matrix4x4 result;

	result = _rhs + _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Friend function * operator
\param _constant - number to multiply
\param _rhs - this
\return result
*/
/******************************************************************************/
Matrix4x4 operator*(float _constant, CR_Mat4 _rhs)
{
	Matrix4x4 result;
	result = _rhs * _constant;
	return result;
}

Vector4 operator*(CR_Vec4 _vector, CR_Mat4 _rhs)
{
	return _rhs * _vector;
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
