/******************************************************************************/
/*!
\file   Matrix2x2.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Matrix2x2's class and member function
Custom Matrix looks like this...

		   0  1  
	0	[ 00 01 ]
	1	[ 10 11 ]

*/
/******************************************************************************/

#include "Matrix2x2.h"

JE_BEGIN

/******************************************************************************/
/*!
\brief - Matrix2x2 Destructor
*/
/******************************************************************************/
Matrix2x2::~Matrix2x2(void)
{}

/******************************************************************************/
/*!
\brief - Matrix2x2 Constructor
\param _element - to be element(all 4 elements)
*/
/******************************************************************************/
Matrix2x2::Matrix2x2(float _element)
{
	for (int i = 0; i < 2; ++i)
	for (int j = 0; j < 2; ++j)
		m_member[i][j] = _element;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 Constructor
\param member1 - member[0][0]
\param member2 - member[0][1]
\param member3 - member[1][0]
\param member4 - member[1][1]
*/
/******************************************************************************/
Matrix2x2::Matrix2x2(float _member1, float _member2,
	float _member3, float _member4)
{
	m_member[0][0] = _member1;
	m_member[0][1] = _member2;
	m_member[1][0] = _member3;
	m_member[1][1] = _member4;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 Copy Constructor
\param _rhs - to be copied
*/
/******************************************************************************/
Matrix2x2::Matrix2x2(const Matrix2x2& _rhs)
{
	if (this != &_rhs)
	{
		for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 2; ++j)
			m_member[i][j] = _rhs.m_member[i][j];
	}
}

/******************************************************************************/
/*!
\brief - Matrix2x2 = operator
\param _rhs - to be assigned
\return *this
*/
/******************************************************************************/
Matrix2x2& Matrix2x2::operator=(const Matrix2x2& _rhs)
{
	if (this != &_rhs)
	{
		for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 2; ++j)
			m_member[i][j] = _rhs.m_member[i][j];
	}

	return *this;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 unary - operator
\return *this
*/
/******************************************************************************/
Matrix2x2& Matrix2x2::operator - (void)
{
	for (int i = 0; i < 2; ++i)
	for (int j = 0; j < 2; ++j)
		m_member[i][j] = -m_member[i][j];

	return *this;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 + operator
\param _rhs - matrix to add
\return result
*/
/******************************************************************************/
Matrix2x2 Matrix2x2::operator+(const Matrix2x2& _rhs) const
{
	Matrix2x2 result;

	for (int i = 0; i < 2; ++i)
	for (int j = 0; j < 2; ++j)
		result.m_member[i][j] = m_member[i][j] + _rhs.m_member[i][j];

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 * operator
\param _rhs - matrix to multiply
\return result
*/
/******************************************************************************/
Matrix2x2 Matrix2x2::operator*(const Matrix2x2& _rhs) const
{
	Matrix2x2 result;

	result.m_member[0][0] = m_member[0][0] * _rhs.m_member[0][0] + m_member[1][0] * _rhs.m_member[0][1];
	result.m_member[0][1] = m_member[0][0] * _rhs.m_member[0][1] + m_member[0][1] * _rhs.m_member[1][1];
	result.m_member[1][0] = m_member[1][0] * _rhs.m_member[0][0] + m_member[1][1] * _rhs.m_member[1][0];
	result.m_member[1][1] = m_member[1][0] * _rhs.m_member[0][1] + m_member[1][1] * _rhs.m_member[1][1];

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 * operator with vector2
\param _rhs - vector2 to multiply 
\return result
*/
/******************************************************************************/
Vector2 Matrix2x2::operator*(const Vector2& _rhs) const
{
	Vector2 result;

	result.x = m_member[0][0] * _rhs.x + m_member[0][1] * _rhs.y ;
	result.y = m_member[1][0] * _rhs.x + m_member[1][1] * _rhs.y ;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 - operator 
\param _rhs - matrix to subtract
\return result
*/
/******************************************************************************/
Matrix2x2 Matrix2x2::operator-(const Matrix2x2& _rhs) const
{
	Matrix2x2 result;

	for (int i = 0; i < 2; ++i)
	for (int j = 0; j < 2; ++j)
		result.m_member[i][j] = m_member[i][j] - _rhs.m_member[i][j];

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 + operator
\param _rhs - number to add
\return result
*/
/******************************************************************************/
Matrix2x2 Matrix2x2::operator+(float _constant) const
{
	Matrix2x2 result;

	for (int i = 0; i < 2; ++i)
	for (int j = 0; j < 2; ++j)
		result.m_member[i][j] = m_member[i][j] + _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 - operator
\param _rhs - number to subtract
\return result
*/
/******************************************************************************/
Matrix2x2 Matrix2x2::operator-(float _constant) const
{
	Matrix2x2 result;

	for (int i = 0; i < 2; ++i)
	for (int j = 0; j < 2; ++j)
		result.m_member[i][j] = m_member[i][j] - _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 * operator
\param _rhs - number to multiply
\return result
*/
/******************************************************************************/
Matrix2x2 Matrix2x2::operator*(float _constant) const
{
	Matrix2x2 result;

	for (int i = 0; i < 2; ++i)
	for (int j = 0; j < 2; ++j)
		result.m_member[i][j] = m_member[i][j] * _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Matrix2x2 / operator
\param _rhs - number to divide
\return result
*/
/******************************************************************************/
Matrix2x2 Matrix2x2::operator/(float _constant) const
{
	Matrix2x2 result;

	for (int i = 0; i < 2; ++i)
	for (int j = 0; j < 2; ++j)
		result.m_member[i][j] = m_member[i][j] / _constant;

	return result;
}

Matrix2x2 Matrix2x2::GetTranspose(void) const
{
	Matrix2x2 result(*this);

	float temp = result.m_member[0][1];
	result.m_member[0][1] = result.m_member[1][0];
	result.m_member[1][0] = temp;

	return result;
}

/******************************************************************************/
/*!
\brief - Transpose matrix
\return result
*/
/******************************************************************************/
Matrix2x2& Matrix2x2::Transpose(void)
{
	float temp = m_member[0][1];
	m_member[0][1] = m_member[1][0];
	m_member[1][0] = temp;

	return *this;
}

Matrix2x2 Matrix2x2::GetInverse() const
{
	Matrix2x2 result(*this);

	// Get ad - bc
	float deter = result.m_member[0][0] * result.m_member[1][1]
		- result.m_member[1][0]* result.m_member[0][1];

	// If determine is not 0,
	if (deter) {
		float temp = result.m_member[1][1];
		result.m_member[1][1] = result.m_member[0][0];
		result.m_member[0][0] = temp;
		result.m_member[0][1] = -result.m_member[0][1];
		result.m_member[1][0] = -result.m_member[1][0];
		result = result / deter;

	}

	else
		JE_DEBUG_PRINT("Determine is 0\n");

	return result;

}

Matrix2x2& Matrix2x2::Inverse()
{
	// Get ad - bc
	float deter = m_member[0][0] * m_member[1][1]
		- m_member[1][0] * m_member[0][1];

	// If determine is not 0,
	if (deter) {
		float temp = m_member[1][1];
		m_member[1][1] = m_member[0][0];
		m_member[0][0] = temp;
		m_member[0][1] = -m_member[0][1];
		m_member[1][0] = -m_member[1][0];
		(*this) = (*this) / deter;

	}

	else
		JE_DEBUG_PRINT("Determine is 0\n");

	return (*this);
}

/******************************************************************************/
/*!
\brief - Set identity matrix
*/
/******************************************************************************/
void Matrix2x2::SetIdentity(void) 
{
	m_member[0][0] = 1;
	m_member[0][1] = 0;
	m_member[1][0] = 0;
	m_member[1][1] = 1;
}

void Matrix2x2::Set(float _member1, float _member2, float _member3, float _member4)
{
	m_member[0][0] = _member1;
	m_member[0][1] = _member2;
	m_member[1][0] = _member3;
	m_member[1][1] = _member4;
}

void Matrix2x2::SetZero()
{
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 2; ++j)
			m_member[i][j] = 0.f;
}

/******************************************************************************/
/*!
\brief - Friend function, + operator 
\param _constant - number to add
\param _rhs - *this 
\return result
*/
/******************************************************************************/
Matrix2x2 operator+(float _constant, const Matrix2x2& _rhs)
{
	Matrix2x2 result;
	result = _rhs + _constant;
	return result;
}

/******************************************************************************/
/*!
\brief - Friend function, * operator
\param _constant - number to multiply
\param _rhs - *this
\return result
*/
/******************************************************************************/
Matrix2x2 operator*(float _constant, const Matrix2x2& _rhs)
{
	Matrix2x2 result;
	result = _rhs * _constant;
	return result;
}

/******************************************************************************/
/*!
\brief - Friend function, << operator
\param os - storage to put contents
\param _rhs - contents to print out
\return os
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& _os, const Matrix2x2& _contents)
{
	for (int i = 0; i < 2; ++i)
	{
		_os << "[ ";
		for (int j = 0; j < 2; ++j)
		{
			_os.setf(std::ios_base::showpoint);
			_os << _contents.m_member[i][j] << " ";
		}
		_os << "]\n";
	}

	return _os;
}

JE_END
