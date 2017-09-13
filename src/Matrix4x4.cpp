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

#include "Matrix4x4.h"

NS_JE_BEGIN

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
		m_member[i][j] = element;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 Copy Constructor
*/
/******************************************************************************/

Matrix4x4::Matrix4x4(const Matrix4x4& rhs)
{
	if (this != &rhs)
	{
		for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m_member[i][j] = rhs.m_member[i][j];
	}
}

Matrix4x4::Matrix4x4(float _member1, float _member2, float _member3, float _member4, 
	float _member5, float _member6, float _member7, float _member8, 
	float _member9, float _member10, float _member11, float _member12, 
	float _member13, float _member14, float _member15, float _member16)
{
	m_member[0][0] = _member1;
	m_member[0][1] = _member2;
	m_member[0][2] = _member3;
	m_member[0][3] = _member4;

	m_member[1][0] = _member5;
	m_member[1][1] = _member6;
	m_member[1][2] = _member7;
	m_member[1][3] = _member8;

	m_member[2][0] = _member9;
	m_member[2][1] = _member10;
	m_member[2][2] = _member11;
	m_member[2][3] = _member12;

	m_member[3][0] = _member13;
	m_member[3][1] = _member14;
	m_member[3][2] = _member15;
	m_member[3][3] = _member16;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 = operator 
\param rhs - matrix to assign
\return *this
*/
/******************************************************************************/
Matrix4x4& Matrix4x4::operator = (const Matrix4x4& rhs)
{
	if (this != &rhs)
	{
		for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m_member[i][j] = rhs.m_member[i][j];
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
		m_member[i][j] = -m_member[i][j];

	return *this;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 + operator
\param rhs - matrix to add
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator + (const Matrix4x4& rhs) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m_member[i][j] += rhs.m_member[i][j];

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
		result.m_member[i][j] += constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 - operator
\param rhs - matrix to subtract
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator - (const Matrix4x4& rhs) const
{
	Matrix4x4 result(*this);

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		result.m_member[i][j] -= rhs.m_member[i][j];

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
		result.m_member[i][j] -= constant;

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
		result.m_member[i][j] *= constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 * operator
\param rhs - matrix to multiply
\return result
*/
/******************************************************************************/
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const
{
	Matrix4x4 result;

	result.m_member[0][0] = m_member[0][0] * rhs.m_member[0][0] + m_member[0][1] * rhs.m_member[1][0] + m_member[0][2] * rhs.m_member[2][0] + m_member[0][3] * rhs.m_member[3][0];
	result.m_member[0][1] = m_member[0][0] * rhs.m_member[0][1] + m_member[0][1] * rhs.m_member[1][1] + m_member[0][2] * rhs.m_member[2][1] + m_member[0][3] * rhs.m_member[3][1];
	result.m_member[0][2] = m_member[0][0] * rhs.m_member[0][2] + m_member[0][1] * rhs.m_member[1][2] + m_member[0][2] * rhs.m_member[2][2] + m_member[0][3] * rhs.m_member[3][2];
	result.m_member[0][3] = m_member[0][0] * rhs.m_member[0][3] + m_member[0][1] * rhs.m_member[1][3] + m_member[0][2] * rhs.m_member[2][3] + m_member[0][3] * rhs.m_member[3][3];

	result.m_member[1][0] = m_member[1][0] * rhs.m_member[0][0] + m_member[1][1] * rhs.m_member[1][0] + m_member[1][2] * rhs.m_member[2][0] + m_member[1][3] * rhs.m_member[3][0];
	result.m_member[1][1] = m_member[1][0] * rhs.m_member[0][1] + m_member[1][1] * rhs.m_member[1][1] + m_member[1][2] * rhs.m_member[2][1] + m_member[1][3] * rhs.m_member[3][1];
	result.m_member[1][2] = m_member[1][0] * rhs.m_member[0][2] + m_member[1][1] * rhs.m_member[1][2] + m_member[1][2] * rhs.m_member[2][2] + m_member[1][3] * rhs.m_member[3][2];
	result.m_member[1][3] = m_member[1][0] * rhs.m_member[0][3] + m_member[1][1] * rhs.m_member[1][3] + m_member[1][2] * rhs.m_member[2][3] + m_member[1][3] * rhs.m_member[3][3];

	result.m_member[2][0] = m_member[2][0] * rhs.m_member[0][0] + m_member[2][1] * rhs.m_member[1][0] + m_member[2][2] * rhs.m_member[2][0] + m_member[2][3] * rhs.m_member[3][0];
	result.m_member[2][1] = m_member[2][0] * rhs.m_member[0][1] + m_member[2][1] * rhs.m_member[1][1] + m_member[2][2] * rhs.m_member[2][1] + m_member[2][3] * rhs.m_member[3][1];
	result.m_member[2][2] = m_member[2][0] * rhs.m_member[0][2] + m_member[2][1] * rhs.m_member[1][2] + m_member[2][2] * rhs.m_member[2][2] + m_member[2][3] * rhs.m_member[3][2];
	result.m_member[2][3] = m_member[2][0] * rhs.m_member[0][3] + m_member[2][1] * rhs.m_member[1][3] + m_member[2][2] * rhs.m_member[2][3] + m_member[2][3] * rhs.m_member[3][3];

	result.m_member[3][0] = m_member[3][0] * rhs.m_member[0][0] + m_member[3][1] * rhs.m_member[1][0] + m_member[3][2] * rhs.m_member[2][0] + m_member[3][3] * rhs.m_member[3][0];
	result.m_member[3][1] = m_member[3][0] * rhs.m_member[0][1] + m_member[3][1] * rhs.m_member[1][1] + m_member[3][2] * rhs.m_member[2][1] + m_member[3][3] * rhs.m_member[3][1];
	result.m_member[3][2] = m_member[3][0] * rhs.m_member[0][2] + m_member[3][1] * rhs.m_member[1][2] + m_member[3][2] * rhs.m_member[2][2] + m_member[3][3] * rhs.m_member[3][2];
	result.m_member[3][3] = m_member[3][0] * rhs.m_member[0][3] + m_member[3][1] * rhs.m_member[1][3] + m_member[3][2] * rhs.m_member[2][3] + m_member[3][3] * rhs.m_member[3][3];

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix4x4 * operator with vector4
\param rhs - vector4 to multiply
\return result
*/
/******************************************************************************/
Vector4 Matrix4x4::operator*(const Vector4& rhs) const
{
	Vector4 result;

	result.x = m_member[0][0] * rhs.x + m_member[0][1] * rhs.y + m_member[0][2] * rhs.z + m_member[0][3] * rhs.w;
	result.y = m_member[1][0] * rhs.x + m_member[1][1] * rhs.y + m_member[1][2] * rhs.z + m_member[1][3] * rhs.w;
	result.z = m_member[2][0] * rhs.x + m_member[2][1] * rhs.y + m_member[2][2] * rhs.z + m_member[2][3] * rhs.w;
	result.w = m_member[3][0] * rhs.x + m_member[3][1] * rhs.y + m_member[3][2] * rhs.z + m_member[3][3] * rhs.w;

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
		result.m_member[i][j] = m_member[i][j] / constant;

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
				m_member[i][j] = 1;
			else
				m_member[i][j] = 0;
		}
	}
}

void Matrix4x4::Set(float _member1, float _member2, float _member3, float _member4, float _member5, float _member6, float _member7, float _member8, float _member9, float _member10, float _member11, float _member12, float _member13, float _member14, float _member15, float _member16)
{
	m_member[0][0] = _member1;
	m_member[0][1] = _member2;
	m_member[0][2] = _member3;
	m_member[0][3] = _member4;

	m_member[1][0] = _member5;
	m_member[1][1] = _member6;
	m_member[1][2] = _member7;
	m_member[1][3] = _member8;

	m_member[2][0] = _member9;
	m_member[2][1] = _member10;
	m_member[2][2] = _member11;
	m_member[2][3] = _member12;

	m_member[3][0] = _member13;
	m_member[3][1] = _member14;
	m_member[3][2] = _member15;
	m_member[3][3] = _member16;
}

void Matrix4x4::SetZero()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m_member[i][j] = 0.f;
}

/******************************************************************************/
/*!
\brief - Friend function + operator
\param constant - number to add
\param rhs - this
\return result
*/
/******************************************************************************/
Matrix4x4 operator+(float constant, const Matrix4x4& rhs)
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
Matrix4x4 operator*(float constant, const Matrix4x4& rhs)
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
std::ostream& operator<<(std::ostream& os, const Matrix4x4& contents)
{
	for (int i = 0; i < 4; ++i)
	{
		os << "[ ";
		for (int j = 0; j < 4; ++j)
		{
			os.setf(std::ios_base::showpoint);
			os << contents.m_member[i][j] << " ";
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
		for (int j = 0; j < 4; ++j)
			result.m_member[j][i] = m_member[i][j];

	return result;
}

Matrix4x4& Matrix4x4::Transpose(void)
{
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result.m_member[j][i] = m_member[i][j];
	*this = result;

	return *this;
}

Matrix4x4 Matrix4x4::GetInverse() const
{
	Matrix4x4 result;

	// Caculate all the elements in matrix
	result.m_member[0][0] = m_member[1][1] * m_member[2][2] * m_member[3][3] -
		m_member[1][1] * m_member[2][3] * m_member[3][2] -
		m_member[2][1] * m_member[1][2] * m_member[3][3] +
		m_member[2][1] * m_member[1][3] * m_member[3][2] +
		m_member[3][1] * m_member[1][2] * m_member[2][3] -
		m_member[3][1] * m_member[1][3] * m_member[2][2];

	result.m_member[1][0] = -m_member[1][0] * m_member[2][2] * m_member[3][3] +
		m_member[1][0] * m_member[2][3] * m_member[3][2] +
		m_member[2][0] * m_member[1][2] * m_member[3][3] -
		m_member[2][0] * m_member[1][3] * m_member[3][2] -
		m_member[3][0] * m_member[1][2] * m_member[2][3] +
		m_member[3][0] * m_member[1][3] * m_member[2][2];

	result.m_member[2][0] = m_member[1][0] * m_member[2][1] * m_member[3][3] -
		m_member[1][0] * m_member[2][3] * m_member[3][1] -
		m_member[2][0] * m_member[1][1] * m_member[3][3] +
		m_member[2][0] * m_member[1][3] * m_member[3][1] +
		m_member[3][0] * m_member[1][1] * m_member[2][3] -
		m_member[3][0] * m_member[1][3] * m_member[2][1];

	result.m_member[3][0] = -m_member[1][0] * m_member[2][1] * m_member[3][2] +
		m_member[1][0] * m_member[2][2] * m_member[3][1] +
		m_member[2][0] * m_member[1][1] * m_member[3][2] -
		m_member[2][0] * m_member[1][2] * m_member[3][1] -
		m_member[3][0] * m_member[1][1] * m_member[2][2] +
		m_member[3][0] * m_member[1][2] * m_member[2][1];

	result.m_member[0][1] = -m_member[0][1] * m_member[2][2] * m_member[3][3] +
		m_member[0][1] * m_member[2][3] * m_member[3][2] +
		m_member[2][1] * m_member[0][2] * m_member[3][3] -
		m_member[2][1] * m_member[0][3] * m_member[3][2] -
		m_member[3][1] * m_member[0][2] * m_member[2][3] +
		m_member[3][1] * m_member[0][3] * m_member[2][2];

	result.m_member[1][1] = m_member[0][0] * m_member[2][2] * m_member[3][3] -
		m_member[0][0] * m_member[2][3] * m_member[3][2] -
		m_member[2][0] * m_member[0][2] * m_member[3][3] +
		m_member[2][0] * m_member[0][3] * m_member[3][2] +
		m_member[3][0] * m_member[0][2] * m_member[2][3] -
		m_member[3][0] * m_member[0][3] * m_member[2][2];

	result.m_member[2][1] = -m_member[0][0] * m_member[2][1] * m_member[3][3] +
		m_member[0][0] * m_member[2][3] * m_member[3][1] +
		m_member[2][0] * m_member[0][1] * m_member[3][3] -
		m_member[2][0] * m_member[0][3] * m_member[3][1] -
		m_member[3][0] * m_member[0][1] * m_member[2][3] +
		m_member[3][0] * m_member[0][3] * m_member[2][1];

	result.m_member[3][1] = m_member[0][0] * m_member[2][1] * m_member[3][2] -
		m_member[0][0] * m_member[2][2] * m_member[3][1] -
		m_member[2][0] * m_member[0][1] * m_member[3][2] +
		m_member[2][0] * m_member[0][2] * m_member[3][1] +
		m_member[3][0] * m_member[0][1] * m_member[2][2] -
		m_member[3][0] * m_member[0][2] * m_member[2][1];

	result.m_member[0][2] = m_member[0][1] * m_member[1][2] * m_member[3][3] -
		m_member[0][1] * m_member[1][3] * m_member[3][2] -
		m_member[1][1] * m_member[0][2] * m_member[3][3] +
		m_member[1][1] * m_member[0][3] * m_member[3][2] +
		m_member[3][1] * m_member[0][2] * m_member[1][3] -
		m_member[3][1] * m_member[0][3] * m_member[1][2];

	result.m_member[1][2] = -m_member[0][0] * m_member[1][2] * m_member[3][3] +
		m_member[0][0] * m_member[1][3] * m_member[3][2] +
		m_member[1][0] * m_member[0][2] * m_member[3][3] -
		m_member[1][0] * m_member[0][3] * m_member[3][2] -
		m_member[3][0] * m_member[0][2] * m_member[1][3] +
		m_member[3][0] * m_member[0][3] * m_member[1][2];

	result.m_member[2][2] = m_member[0][0] * m_member[1][1] * m_member[3][3] -
		m_member[0][0] * m_member[1][3] * m_member[3][1] -
		m_member[1][0] * m_member[0][1] * m_member[3][3] +
		m_member[1][0] * m_member[0][3] * m_member[3][1] +
		m_member[3][0] * m_member[0][1] * m_member[1][3] -
		m_member[3][0] * m_member[0][3] * m_member[1][1];

	result.m_member[3][2] = -m_member[0][0] * m_member[1][1] * m_member[3][2] +
		m_member[0][0] * m_member[1][2] * m_member[3][1] +
		m_member[1][0] * m_member[0][1] * m_member[3][2] -
		m_member[1][0] * m_member[0][2] * m_member[3][1] -
		m_member[3][0] * m_member[0][1] * m_member[1][2] +
		m_member[3][0] * m_member[0][2] * m_member[1][1];

	result.m_member[0][3] = -m_member[0][1] * m_member[1][2] * m_member[2][3] +
		m_member[0][1] * m_member[1][3] * m_member[2][2] +
		m_member[1][1] * m_member[0][2] * m_member[2][3] -
		m_member[1][1] * m_member[0][3] * m_member[2][2] -
		m_member[2][1] * m_member[0][2] * m_member[1][3] +
		m_member[2][1] * m_member[0][3] * m_member[1][2];

	result.m_member[1][3] = m_member[0][0] * m_member[1][2] * m_member[2][3] -
		m_member[0][0] * m_member[1][3] * m_member[2][2] -
		m_member[1][0] * m_member[0][2] * m_member[2][3] +
		m_member[1][0] * m_member[0][3] * m_member[2][2] +
		m_member[2][0] * m_member[0][2] * m_member[1][3] -
		m_member[2][0] * m_member[0][3] * m_member[1][2];

	result.m_member[2][3] = -m_member[0][0] * m_member[1][1] * m_member[2][3] +
		m_member[0][0] * m_member[1][3] * m_member[2][1] +
		m_member[1][0] * m_member[0][1] * m_member[2][3] -
		m_member[1][0] * m_member[0][3] * m_member[2][1] -
		m_member[2][0] * m_member[0][1] * m_member[1][3] +
		m_member[2][0] * m_member[0][3] * m_member[1][1];

	result.m_member[3][3] = m_member[0][0] * m_member[1][1] * m_member[2][2] -
		m_member[0][0] * m_member[1][2] * m_member[2][1] -
		m_member[1][0] * m_member[0][1] * m_member[2][2] +
		m_member[1][0] * m_member[0][2] * m_member[2][1] +
		m_member[2][0] * m_member[0][1] * m_member[1][2] -
		m_member[2][0] * m_member[0][2] * m_member[1][1];

	// Get determine
	float det = m_member[0][0] * result.m_member[0][0] + 
		m_member[0][1] * result.m_member[1][0] + 
		m_member[0][2] * result.m_member[2][0] + 
		m_member[0][3] * result.m_member[3][0];

	// No inverse in this case
	if (det == 0)
		return false;

	// Make denomitator
	det = 1.f / det;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		result.m_member[i][j] = result.m_member[i][j] * det;

	return result;
}

Matrix4x4& Matrix4x4::Inverse()
{
	Matrix4x4 result;

	// Caculate all the elements in matrix
	result.m_member[0][0] = m_member[1][1] * m_member[2][2] * m_member[3][3] -
		m_member[1][1] * m_member[2][3] * m_member[3][2] -
		m_member[2][1] * m_member[1][2] * m_member[3][3] +
		m_member[2][1] * m_member[1][3] * m_member[3][2] +
		m_member[3][1] * m_member[1][2] * m_member[2][3] -
		m_member[3][1] * m_member[1][3] * m_member[2][2];

	result.m_member[1][0] = -m_member[1][0] * m_member[2][2] * m_member[3][3] +
		m_member[1][0] * m_member[2][3] * m_member[3][2] +
		m_member[2][0] * m_member[1][2] * m_member[3][3] -
		m_member[2][0] * m_member[1][3] * m_member[3][2] -
		m_member[3][0] * m_member[1][2] * m_member[2][3] +
		m_member[3][0] * m_member[1][3] * m_member[2][2];

	result.m_member[2][0] = m_member[1][0] * m_member[2][1] * m_member[3][3] -
		m_member[1][0] * m_member[2][3] * m_member[3][1] -
		m_member[2][0] * m_member[1][1] * m_member[3][3] +
		m_member[2][0] * m_member[1][3] * m_member[3][1] +
		m_member[3][0] * m_member[1][1] * m_member[2][3] -
		m_member[3][0] * m_member[1][3] * m_member[2][1];

	result.m_member[3][0] = -m_member[1][0] * m_member[2][1] * m_member[3][2] +
		m_member[1][0] * m_member[2][2] * m_member[3][1] +
		m_member[2][0] * m_member[1][1] * m_member[3][2] -
		m_member[2][0] * m_member[1][2] * m_member[3][1] -
		m_member[3][0] * m_member[1][1] * m_member[2][2] +
		m_member[3][0] * m_member[1][2] * m_member[2][1];

	result.m_member[0][1] = -m_member[0][1] * m_member[2][2] * m_member[3][3] +
		m_member[0][1] * m_member[2][3] * m_member[3][2] +
		m_member[2][1] * m_member[0][2] * m_member[3][3] -
		m_member[2][1] * m_member[0][3] * m_member[3][2] -
		m_member[3][1] * m_member[0][2] * m_member[2][3] +
		m_member[3][1] * m_member[0][3] * m_member[2][2];

	result.m_member[1][1] = m_member[0][0] * m_member[2][2] * m_member[3][3] -
		m_member[0][0] * m_member[2][3] * m_member[3][2] -
		m_member[2][0] * m_member[0][2] * m_member[3][3] +
		m_member[2][0] * m_member[0][3] * m_member[3][2] +
		m_member[3][0] * m_member[0][2] * m_member[2][3] -
		m_member[3][0] * m_member[0][3] * m_member[2][2];

	result.m_member[2][1] = -m_member[0][0] * m_member[2][1] * m_member[3][3] +
		m_member[0][0] * m_member[2][3] * m_member[3][1] +
		m_member[2][0] * m_member[0][1] * m_member[3][3] -
		m_member[2][0] * m_member[0][3] * m_member[3][1] -
		m_member[3][0] * m_member[0][1] * m_member[2][3] +
		m_member[3][0] * m_member[0][3] * m_member[2][1];

	result.m_member[3][1] = m_member[0][0] * m_member[2][1] * m_member[3][2] -
		m_member[0][0] * m_member[2][2] * m_member[3][1] -
		m_member[2][0] * m_member[0][1] * m_member[3][2] +
		m_member[2][0] * m_member[0][2] * m_member[3][1] +
		m_member[3][0] * m_member[0][1] * m_member[2][2] -
		m_member[3][0] * m_member[0][2] * m_member[2][1];

	result.m_member[0][2] = m_member[0][1] * m_member[1][2] * m_member[3][3] -
		m_member[0][1] * m_member[1][3] * m_member[3][2] -
		m_member[1][1] * m_member[0][2] * m_member[3][3] +
		m_member[1][1] * m_member[0][3] * m_member[3][2] +
		m_member[3][1] * m_member[0][2] * m_member[1][3] -
		m_member[3][1] * m_member[0][3] * m_member[1][2];

	result.m_member[1][2] = -m_member[0][0] * m_member[1][2] * m_member[3][3] +
		m_member[0][0] * m_member[1][3] * m_member[3][2] +
		m_member[1][0] * m_member[0][2] * m_member[3][3] -
		m_member[1][0] * m_member[0][3] * m_member[3][2] -
		m_member[3][0] * m_member[0][2] * m_member[1][3] +
		m_member[3][0] * m_member[0][3] * m_member[1][2];

	result.m_member[2][2] = m_member[0][0] * m_member[1][1] * m_member[3][3] -
		m_member[0][0] * m_member[1][3] * m_member[3][1] -
		m_member[1][0] * m_member[0][1] * m_member[3][3] +
		m_member[1][0] * m_member[0][3] * m_member[3][1] +
		m_member[3][0] * m_member[0][1] * m_member[1][3] -
		m_member[3][0] * m_member[0][3] * m_member[1][1];

	result.m_member[3][2] = -m_member[0][0] * m_member[1][1] * m_member[3][2] +
		m_member[0][0] * m_member[1][2] * m_member[3][1] +
		m_member[1][0] * m_member[0][1] * m_member[3][2] -
		m_member[1][0] * m_member[0][2] * m_member[3][1] -
		m_member[3][0] * m_member[0][1] * m_member[1][2] +
		m_member[3][0] * m_member[0][2] * m_member[1][1];

	result.m_member[0][3] = -m_member[0][1] * m_member[1][2] * m_member[2][3] +
		m_member[0][1] * m_member[1][3] * m_member[2][2] +
		m_member[1][1] * m_member[0][2] * m_member[2][3] -
		m_member[1][1] * m_member[0][3] * m_member[2][2] -
		m_member[2][1] * m_member[0][2] * m_member[1][3] +
		m_member[2][1] * m_member[0][3] * m_member[1][2];

	result.m_member[1][3] = m_member[0][0] * m_member[1][2] * m_member[2][3] -
		m_member[0][0] * m_member[1][3] * m_member[2][2] -
		m_member[1][0] * m_member[0][2] * m_member[2][3] +
		m_member[1][0] * m_member[0][3] * m_member[2][2] +
		m_member[2][0] * m_member[0][2] * m_member[1][3] -
		m_member[2][0] * m_member[0][3] * m_member[1][2];

	result.m_member[2][3] = -m_member[0][0] * m_member[1][1] * m_member[2][3] +
		m_member[0][0] * m_member[1][3] * m_member[2][1] +
		m_member[1][0] * m_member[0][1] * m_member[2][3] -
		m_member[1][0] * m_member[0][3] * m_member[2][1] -
		m_member[2][0] * m_member[0][1] * m_member[1][3] +
		m_member[2][0] * m_member[0][3] * m_member[1][1];

	result.m_member[3][3] = m_member[0][0] * m_member[1][1] * m_member[2][2] -
		m_member[0][0] * m_member[1][2] * m_member[2][1] -
		m_member[1][0] * m_member[0][1] * m_member[2][2] +
		m_member[1][0] * m_member[0][2] * m_member[2][1] +
		m_member[2][0] * m_member[0][1] * m_member[1][2] -
		m_member[2][0] * m_member[0][2] * m_member[1][1];

	// Get determine
	float det = m_member[0][0] * result.m_member[0][0] +
		m_member[0][1] * result.m_member[1][0] +
		m_member[0][2] * result.m_member[2][0] +
		m_member[0][3] * result.m_member[3][0];

	// No inverse in this case
	if (det == 0)
		return *this;

	// Make denomitator
	det = 1.f / det;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			(*this).m_member[i][j] = result.m_member[i][j] * det;

	return *this;
}


Matrix4x4 Matrix4x4::Perspective(float fovy, float aspect, float zNear, float zFar)
{
	float tanHalfFovy = tan(fovy / static_cast<float>(2));

	Matrix4x4 Result(static_cast<float>(0));
	Result.m_member[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
	Result.m_member[1][1] = static_cast<float>(1) / (tanHalfFovy);
	Result.m_member[2][2] = -(zFar + zNear) / (zFar - zNear);
	Result.m_member[2][3] = -static_cast<float>(1);
	Result.m_member[3][2] = -(static_cast<float>(2) * zFar * zNear) / (zFar - zNear);
	return Result;
}


Matrix4x4 Matrix4x4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	//tmat4x4<T, defaultp> Result(1);
	//Result[0][0] = static_cast<T>(2) / (right - left);
	//Result[1][1] = static_cast<T>(2) / (top - bottom);
	//Result[2][2] = -static_cast<T>(2) / (zFar - zNear);
	//Result[3][0] = -(right + left) / (right - left);
	//Result[3][1] = -(top + bottom) / (top - bottom);
	//Result[3][2] = -(zFar + zNear) / (zFar - zNear);

	Matrix4x4 Result;
	Result.m_member[0][0] = 2.f / (right - left);
	Result.m_member[1][1] = 2.f / (top - bottom);
	Result.m_member[2][2] = -2.f / (zFar - zNear);
	Result.m_member[3][0] = (-(right + left)) / (right - left);
	Result.m_member[3][1] = (-(top + bottom)) / (top - bottom);
	Result.m_member[3][2] = (-(zFar + zNear)) / (zFar - zNear);
	Result.m_member[3][3] = 1.f;
	return Result;
}


Matrix4x4 Matrix4x4::LookAt(Vector3 eye, Vector3 center, Vector3 up)
{
	Vector3 temp = (center - eye);

	Vector3 f = temp.Normalize();
	Vector3 cal = f.CrossProduct(up);
	Vector3 s = cal * 1 / (sqrt(cal.DotProduct(cal)));
	//		return x * inversesqrt(dot(x, x));
	Vector3 u = s.CrossProduct(f);

	Matrix4x4 Result;
	Result.SetIdentity();

	Result.m_member[0][0] = s.x;
	Result.m_member[1][0] = s.y;
	Result.m_member[2][0] = s.z;

	Result.m_member[0][1] = u.x;
	Result.m_member[1][1] = u.y;
	Result.m_member[2][1] = u.z;

	Result.m_member[0][2] = -f.x;
	Result.m_member[1][2] = -f.y;
	Result.m_member[2][2] = -f.z;

	Result.m_member[3][0] = -(s.DotProduct(eye));
	Result.m_member[3][1] = -(u.DotProduct(eye));
	Result.m_member[3][2] = f.DotProduct(eye);

	return Result;
}


Matrix4x4 Matrix4x4::Translate(const Vector3& vec)
{
	Matrix4x4 Result;
	Result.SetIdentity();

	Result.m_member[3][0] = vec.x;
	Result.m_member[3][1] = vec.y;
	Result.m_member[3][2] = vec.z;

	return Result;
}


Matrix4x4 Matrix4x4::Scale(const Vector3& vec)
{
	Matrix4x4 Result;
	Result.SetIdentity();

	Result.m_member[0][0] = vec.x;
	Result.m_member[1][1] = vec.y;
	Result.m_member[2][2] = vec.z;

	return Result;
}


Matrix4x4 Matrix4x4::Rotation(float degree, Vector3& vec)
{
	Matrix4x4 Result;
	Result.SetIdentity();

	float c = cos(degree);
	float s = sin(degree);

	Vector3 a = vec.Normalize();
	Vector3 n = ((float(1) - c) * a);

	Result.m_member[0][0] = c + n.x + a.x;
	Result.m_member[0][1] = n.x * a.y + s * a.z;
	Result.m_member[0][2] = n.x * a.z - s * a.y;

	Result.m_member[1][0] = n.y * a.x - s * a.z;
	Result.m_member[1][1] = c + n.y * a.y;
	Result.m_member[1][2] = n.y * a.z + s * a.x;

	Result.m_member[2][0] = n.z * a.x + s * a.y;
	Result.m_member[2][1] = n.z * a.y - s * a.x;
	Result.m_member[2][2] = c + n.z * a.z;

	return Result;
}

NS_JE_END