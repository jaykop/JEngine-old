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

#include <cmath>
#include "MathUtils.h"
#include "Matrix4x4.h"

JE_BEGIN

/******************************************************************************/
/*!
\brief - Matrix4x4 Destructor
*/
/******************************************************************************/
Matrix4x4::~Matrix4x4(void)
{}

/******************************************************************************/
/*!
\brief - Matrix4x4 Constructor
*/
/******************************************************************************/
Matrix4x4::Matrix4x4(float element)
{
	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		m[i][j] = element;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 Copy Constructor
*/
/******************************************************************************/
Matrix4x4::Matrix4x4(const mat4& rhs)
{
	if (this != &rhs)
	{
		for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = rhs.m[i][j];
	}
}

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
\brief - Matrix4x4 = operator 
\param rhs - matrix to assign
\return *this
*/
/******************************************************************************/
Matrix4x4& Matrix4x4::operator = (const mat4& rhs)
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
Matrix4x4& Matrix4x4::operator-(void)
{
	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		m[i][j] = -m[i][j];

	return *this;
}

void Matrix4x4::operator+=(const mat4& rhs)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] += rhs.m[i][j];
}

void Matrix4x4::operator*=(const mat4& rhs)
{
	(*this) = (*this)*rhs;
}

void Matrix4x4::operator-=(const mat4& rhs)
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
		JE_DEBUG_PRINT("*Matrix4x4: Cannot devide by 0.\n");
}

/******************************************************************************/
/*!
\brief - Matrix4x4 + operator
\param rhs - matrix to add
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator + (const mat4& rhs) const
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
\param rhs - number to add
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
Matrix4x4 Matrix4x4::operator - (const mat4& rhs) const
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
\param rhs - number to subtract
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
\param rhs - number to multiply
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
Matrix4x4 Matrix4x4::operator*(const mat4& rhs) const
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
Vector4 Matrix4x4::operator*(const vec4& rhs) const
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
\param rhs - number to divide
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

/******************************************************************************/
/*!
\brief - Set matrix identity
*/
/******************************************************************************/
void Matrix4x4::SetIdentity(void)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (i == j)
				m[i][j] = 1;
			else
				m[i][j] = 0;
		}
	}
}

void Matrix4x4::Set(float _member1, float _member2, float _member3, float _member4, float _member5, float _member6, float _member7, float _member8, float _member9, float _member10, float _member11, float _member12, float _member13, float _member14, float _member15, float _member16)
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
\param constant - number to add
\param rhs - this
\return result
*/
/******************************************************************************/
Matrix4x4 operator+(float constant, const mat4& rhs)
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
Matrix4x4 operator*(float constant, const mat4& rhs)
{
	Matrix4x4 result;
	result = rhs * constant;
	return result;
}

/******************************************************************************/
/*!
\brief - Friend function << operator
\param os - storage to put contents
\param contents - contents to print out
\return os
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& os, const mat4& contents)
{
	for (int i = 0; i < 4; ++i)	// row
	{
		os << "[ ";
		for (int j = 0; j < 4; ++j)	// column
		{
			os.setf(std::ios_base::showpoint);
			os << contents.m[i][j] << " ";
		}
		os << "]\n";
	}

	return os;
}

/******************************************************************************/
/*!
\brief - Transpose matrix
\return result
*/
/******************************************************************************/

Matrix4x4 Matrix4x4::GetTranspose(void) const
{
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j <= i; ++j) {
			result.m[j][i] = m[i][j];
			result.m[i][j] = m[j][i];
		}
	return result;
}

Matrix4x4& Matrix4x4::Transpose(void)
{
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j <= i; ++j) {
			result.m[j][i] = m[i][j];
			result.m[i][j] = m[j][i];
		}

	*this = result;
	return *this;
}

Matrix4x4 Matrix4x4::GetInverse() const
{
	Matrix4x4 result;

	// Caculate all the elements in matrix
	result.m[0][0] = m[1][1] * m[2][2] * m[3][3] -
		m[1][1] * m[2][3] * m[3][2] -
		m[2][1] * m[1][2] * m[3][3] +
		m[2][1] * m[1][3] * m[3][2] +
		m[3][1] * m[1][2] * m[2][3] -
		m[3][1] * m[1][3] * m[2][2];

	result.m[1][0] = -m[1][0] * m[2][2] * m[3][3] +
		m[1][0] * m[2][3] * m[3][2] +
		m[2][0] * m[1][2] * m[3][3] -
		m[2][0] * m[1][3] * m[3][2] -
		m[3][0] * m[1][2] * m[2][3] +
		m[3][0] * m[1][3] * m[2][2];

	result.m[2][0] = m[1][0] * m[2][1] * m[3][3] -
		m[1][0] * m[2][3] * m[3][1] -
		m[2][0] * m[1][1] * m[3][3] +
		m[2][0] * m[1][3] * m[3][1] +
		m[3][0] * m[1][1] * m[2][3] -
		m[3][0] * m[1][3] * m[2][1];

	result.m[3][0] = -m[1][0] * m[2][1] * m[3][2] +
		m[1][0] * m[2][2] * m[3][1] +
		m[2][0] * m[1][1] * m[3][2] -
		m[2][0] * m[1][2] * m[3][1] -
		m[3][0] * m[1][1] * m[2][2] +
		m[3][0] * m[1][2] * m[2][1];

	result.m[0][1] = -m[0][1] * m[2][2] * m[3][3] +
		m[0][1] * m[2][3] * m[3][2] +
		m[2][1] * m[0][2] * m[3][3] -
		m[2][1] * m[0][3] * m[3][2] -
		m[3][1] * m[0][2] * m[2][3] +
		m[3][1] * m[0][3] * m[2][2];

	result.m[1][1] = m[0][0] * m[2][2] * m[3][3] -
		m[0][0] * m[2][3] * m[3][2] -
		m[2][0] * m[0][2] * m[3][3] +
		m[2][0] * m[0][3] * m[3][2] +
		m[3][0] * m[0][2] * m[2][3] -
		m[3][0] * m[0][3] * m[2][2];

	result.m[2][1] = -m[0][0] * m[2][1] * m[3][3] +
		m[0][0] * m[2][3] * m[3][1] +
		m[2][0] * m[0][1] * m[3][3] -
		m[2][0] * m[0][3] * m[3][1] -
		m[3][0] * m[0][1] * m[2][3] +
		m[3][0] * m[0][3] * m[2][1];

	result.m[3][1] = m[0][0] * m[2][1] * m[3][2] -
		m[0][0] * m[2][2] * m[3][1] -
		m[2][0] * m[0][1] * m[3][2] +
		m[2][0] * m[0][2] * m[3][1] +
		m[3][0] * m[0][1] * m[2][2] -
		m[3][0] * m[0][2] * m[2][1];

	result.m[0][2] = m[0][1] * m[1][2] * m[3][3] -
		m[0][1] * m[1][3] * m[3][2] -
		m[1][1] * m[0][2] * m[3][3] +
		m[1][1] * m[0][3] * m[3][2] +
		m[3][1] * m[0][2] * m[1][3] -
		m[3][1] * m[0][3] * m[1][2];

	result.m[1][2] = -m[0][0] * m[1][2] * m[3][3] +
		m[0][0] * m[1][3] * m[3][2] +
		m[1][0] * m[0][2] * m[3][3] -
		m[1][0] * m[0][3] * m[3][2] -
		m[3][0] * m[0][2] * m[1][3] +
		m[3][0] * m[0][3] * m[1][2];

	result.m[2][2] = m[0][0] * m[1][1] * m[3][3] -
		m[0][0] * m[1][3] * m[3][1] -
		m[1][0] * m[0][1] * m[3][3] +
		m[1][0] * m[0][3] * m[3][1] +
		m[3][0] * m[0][1] * m[1][3] -
		m[3][0] * m[0][3] * m[1][1];

	result.m[3][2] = -m[0][0] * m[1][1] * m[3][2] +
		m[0][0] * m[1][2] * m[3][1] +
		m[1][0] * m[0][1] * m[3][2] -
		m[1][0] * m[0][2] * m[3][1] -
		m[3][0] * m[0][1] * m[1][2] +
		m[3][0] * m[0][2] * m[1][1];

	result.m[0][3] = -m[0][1] * m[1][2] * m[2][3] +
		m[0][1] * m[1][3] * m[2][2] +
		m[1][1] * m[0][2] * m[2][3] -
		m[1][1] * m[0][3] * m[2][2] -
		m[2][1] * m[0][2] * m[1][3] +
		m[2][1] * m[0][3] * m[1][2];

	result.m[1][3] = m[0][0] * m[1][2] * m[2][3] -
		m[0][0] * m[1][3] * m[2][2] -
		m[1][0] * m[0][2] * m[2][3] +
		m[1][0] * m[0][3] * m[2][2] +
		m[2][0] * m[0][2] * m[1][3] -
		m[2][0] * m[0][3] * m[1][2];

	result.m[2][3] = -m[0][0] * m[1][1] * m[2][3] +
		m[0][0] * m[1][3] * m[2][1] +
		m[1][0] * m[0][1] * m[2][3] -
		m[1][0] * m[0][3] * m[2][1] -
		m[2][0] * m[0][1] * m[1][3] +
		m[2][0] * m[0][3] * m[1][1];

	result.m[3][3] = m[0][0] * m[1][1] * m[2][2] -
		m[0][0] * m[1][2] * m[2][1] -
		m[1][0] * m[0][1] * m[2][2] +
		m[1][0] * m[0][2] * m[2][1] +
		m[2][0] * m[0][1] * m[1][2] -
		m[2][0] * m[0][2] * m[1][1];

	// Get determine
	float det = m[0][0] * result.m[0][0] + 
		m[0][1] * result.m[1][0] + 
		m[0][2] * result.m[2][0] + 
		m[0][3] * result.m[3][0];

	// No inverse in this case
	if (det == 0)
		return false;

	// Make denomitator
	det = 1.f / det;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		result.m[i][j] = result.m[i][j] * det;

	return result;
}

Matrix4x4& Matrix4x4::Inverse()
{
	Matrix4x4 result;

	// Caculate all the elements in matrix
	result.m[0][0] = m[1][1] * m[2][2] * m[3][3] -
							m[1][1] * m[2][3] * m[3][2] -
							m[2][1] * m[1][2] * m[3][3] +
							m[2][1] * m[1][3] * m[3][2] +
							m[3][1] * m[1][2] * m[2][3] -
							m[3][1] * m[1][3] * m[2][2];

	result.m[1][0] = -m[1][0] * m[2][2] * m[3][3] +
							m[1][0] * m[2][3] * m[3][2] +
							m[2][0] * m[1][2] * m[3][3] -
							m[2][0] * m[1][3] * m[3][2] -
							m[3][0] * m[1][2] * m[2][3] +
							m[3][0] * m[1][3] * m[2][2];

	result.m[2][0] = m[1][0] * m[2][1] * m[3][3] -
							m[1][0] * m[2][3] * m[3][1] -
							m[2][0] * m[1][1] * m[3][3] +
							m[2][0] * m[1][3] * m[3][1] +
							m[3][0] * m[1][1] * m[2][3] -
							m[3][0] * m[1][3] * m[2][1];

	result.m[3][0] = -m[1][0] * m[2][1] * m[3][2] +
							m[1][0] * m[2][2] * m[3][1] +
							m[2][0] * m[1][1] * m[3][2] -
							m[2][0] * m[1][2] * m[3][1] -
							m[3][0] * m[1][1] * m[2][2] +
							m[3][0] * m[1][2] * m[2][1];

	result.m[0][1] = -m[0][1] * m[2][2] * m[3][3] +
							m[0][1] * m[2][3] * m[3][2] +
							m[2][1] * m[0][2] * m[3][3] -
							m[2][1] * m[0][3] * m[3][2] -
							m[3][1] * m[0][2] * m[2][3] +
							m[3][1] * m[0][3] * m[2][2];

	result.m[1][1] = m[0][0] * m[2][2] * m[3][3] -
							m[0][0] * m[2][3] * m[3][2] -
							m[2][0] * m[0][2] * m[3][3] +
							m[2][0] * m[0][3] * m[3][2] +
							m[3][0] * m[0][2] * m[2][3] -
							m[3][0] * m[0][3] * m[2][2];

	result.m[2][1] = -m[0][0] * m[2][1] * m[3][3] +
							m[0][0] * m[2][3] * m[3][1] +
							m[2][0] * m[0][1] * m[3][3] -
							m[2][0] * m[0][3] * m[3][1] -
							m[3][0] * m[0][1] * m[2][3] +
							m[3][0] * m[0][3] * m[2][1];

	result.m[3][1] = m[0][0] * m[2][1] * m[3][2] -
							m[0][0] * m[2][2] * m[3][1] -
							m[2][0] * m[0][1] * m[3][2] +
							m[2][0] * m[0][2] * m[3][1] +
							m[3][0] * m[0][1] * m[2][2] -
							m[3][0] * m[0][2] * m[2][1];

	result.m[0][2] = m[0][1] * m[1][2] * m[3][3] -
							m[0][1] * m[1][3] * m[3][2] -
							m[1][1] * m[0][2] * m[3][3] +
							m[1][1] * m[0][3] * m[3][2] +
							m[3][1] * m[0][2] * m[1][3] -
							m[3][1] * m[0][3] * m[1][2];

	result.m[1][2] = -m[0][0] * m[1][2] * m[3][3] +
							m[0][0] * m[1][3] * m[3][2] +
							m[1][0] * m[0][2] * m[3][3] -
							m[1][0] * m[0][3] * m[3][2] -
							m[3][0] * m[0][2] * m[1][3] +
							m[3][0] * m[0][3] * m[1][2];

	result.m[2][2] = m[0][0] * m[1][1] * m[3][3] -
							m[0][0] * m[1][3] * m[3][1] -
							m[1][0] * m[0][1] * m[3][3] +
							m[1][0] * m[0][3] * m[3][1] +
							m[3][0] * m[0][1] * m[1][3] -
							m[3][0] * m[0][3] * m[1][1];

	result.m[3][2] = -m[0][0] * m[1][1] * m[3][2] +
							m[0][0] * m[1][2] * m[3][1] +
							m[1][0] * m[0][1] * m[3][2] -
							m[1][0] * m[0][2] * m[3][1] -
							m[3][0] * m[0][1] * m[1][2] +
							m[3][0] * m[0][2] * m[1][1];

	result.m[0][3] = -m[0][1] * m[1][2] * m[2][3] +
							m[0][1] * m[1][3] * m[2][2] +
							m[1][1] * m[0][2] * m[2][3] -
							m[1][1] * m[0][3] * m[2][2] -
							m[2][1] * m[0][2] * m[1][3] +
							m[2][1] * m[0][3] * m[1][2];

	result.m[1][3] = m[0][0] * m[1][2] * m[2][3] -
							m[0][0] * m[1][3] * m[2][2] -
							m[1][0] * m[0][2] * m[2][3] +
							m[1][0] * m[0][3] * m[2][2] +
							m[2][0] * m[0][2] * m[1][3] -
							m[2][0] * m[0][3] * m[1][2];

	result.m[2][3] = -m[0][0] * m[1][1] * m[2][3] +
							m[0][0] * m[1][3] * m[2][1] +
							m[1][0] * m[0][1] * m[2][3] -
							m[1][0] * m[0][3] * m[2][1] -
							m[2][0] * m[0][1] * m[1][3] +
							m[2][0] * m[0][3] * m[1][1];

	result.m[3][3] = m[0][0] * m[1][1] * m[2][2] -
							m[0][0] * m[1][2] * m[2][1] -
							m[1][0] * m[0][1] * m[2][2] +
							m[1][0] * m[0][2] * m[2][1] +
							m[2][0] * m[0][1] * m[1][2] -
							m[2][0] * m[0][2] * m[1][1];

	// Get determine
	float det = m[0][0] * result.m[0][0] +
		m[0][1] * result.m[1][0] +
		m[0][2] * result.m[2][0] +
		m[0][3] * result.m[3][0];

	// No inverse in this case
	if (det == 0) {
		JE_DEBUG_PRINT("*Matrix4x4: Cannot devide by 0.\n");
		return *this;
	}

	// Make denomitator
	det = 1.f / det;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			(*this).m[i][j] = result.m[i][j] * det;

	return *this;
}


Matrix4x4 Matrix4x4::Perspective(float _fovy, float _aspect, float _zNear, float _zFar)
{
	float distance = 1.f / tanf(_fovy * .5f);

	Matrix4x4 Result;

	Result.m[0][0] = distance / _aspect;
	Result.m[1][1] = distance;
	Result.m[2][2] = (_zFar + _zNear) / (_zNear - _zFar);

	Result.m[2][3] = (2 * _zFar * _zNear) / (_zNear -_zFar);
	Result.m[3][2] = -1.f;
	
	return Result;
}

Matrix4x4 Matrix4x4::Orthogonal(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar)
{
	Matrix4x4 Result;

	// TODO
	// Need offset here to consider resolution?
	// I don't know...

	Result.m[0][0] = 2.f / (_right - _left);
	Result.m[1][1] = 2.f / (_top - _bottom);
	Result.m[2][2] = 2.f / (_zNear - _zFar);
	Result.m[0][3] = (_right + _left) / (_left - _right);
	Result.m[1][3] = (_top + _bottom) / (_bottom - _top);
	Result.m[2][3] = (_zFar + _zNear) / (_zNear - _zFar);
	Result.m[3][3] = 1.f;

	return Result;
}

Matrix4x4 Matrix4x4::Camera(const Vector3 _eye, const Vector3 _target, const Vector3 _up)
{
	Vector3 look = (_eye - _target).GetNormalize();
	Vector3 up = _up;
	Vector3 right = up.CrossProduct(look);
	up = look.CrossProduct(right);

	Matrix4x4 Result;

	Result.m[0][0] = right.x;
	Result.m[0][1] = right.y;
	Result.m[0][2] = right.z;
	Result.m[0][3] = -right.DotProduct(_eye);

	Result.m[1][0] = up.x;
	Result.m[1][1] = up.y;
	Result.m[1][2] = up.z;
	Result.m[1][3] = -up.DotProduct(_eye);

	Result.m[2][0] = look.x;
	Result.m[2][1] = look.y;
	Result.m[2][2] = look.z;
	Result.m[2][3] = -look.DotProduct(_eye);

	Result.m[3][3] = 1.f;

	return Result;
}


Matrix4x4 Matrix4x4::Translate(const vec3& _vec)
{
	Matrix4x4 Result;
	Result.SetIdentity();

	Result.m[0][3] = _vec.x;
	Result.m[1][3] = _vec.y;
	Result.m[2][3] = _vec.z;

	return Result;
}


Matrix4x4 Matrix4x4::Scale(const vec3& _vec)
{
	Matrix4x4 Result;

	Result.m[0][0] = _vec.x;
	Result.m[1][1] = _vec.y;
	Result.m[2][2] = _vec.z;
	Result.m[3][3] = 1.f;

	return Result;
}

Matrix4x4 Matrix4x4::Rotate(float _degree, const vec3& _vec)
{
	Matrix4x4 Result;

	float radian = Math::DegToRad(_degree);

	float cosine = cos(radian);
	float sine = sin(radian);

	Vector3 norm = _vec.GetNormalize();
	Vector3 offset = (1.f - cosine) * norm;

	Result.m[0][0] = offset.x * norm.x + cosine;
	Result.m[1][0] = offset.x * norm.y + norm.z * sine;
	Result.m[2][0] = offset.x * norm.z - norm.y * sine;

	Result.m[0][1] = offset.x * norm.y - norm.z * sine;
	Result.m[1][1] = offset.y * norm.y + cosine;
	Result.m[2][1] = offset.y * norm.z + norm.x * sine;

	Result.m[0][2] = offset.x * norm.z + norm.y * sine;
	Result.m[1][2] = offset.y * norm.z - norm.x * sine;
	Result.m[2][2] = offset.z * norm.z + cosine;

	Result.m[3][3] = 1.f;

	return Result;
}

JE_END
