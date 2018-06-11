/******************************************************************************/
/*!
\file   Vector2.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Vector2's class and member function

*/
/******************************************************************************/

#include "MathUtils.h"

jeBegin

using namespace Math;

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
const Vector2 Vector2::ONE(1.f, 1.f);
const Vector2 Vector2::ZERO(0.f, 0.f);
const Vector2 Vector2::UNIT_X(1.f, 0.f);
const Vector2 Vector2::UNIT_Y(0.f, 1.f);

void Vector2::Set(const vec2& _copy)
{
	x = _copy.x;
	y = _copy.y;
}

void Vector2::Set(float _x, float _y)
{
	x = _x;
	y = _y;
}

void Vector2::SetZero()
{
	x = y = 0.f;
}

void Vector2::SetOne()
{
	x = y = 1.f;
}

void Vector2::SetUnitX()
{
	x = 1.f; 
	y = 0.f;
}

void Vector2::SetUnitY()
{
	x = 0.f;
	y = 1.f;
}

/******************************************************************************/
/*!
\brief - Vector2 Constructor
\param _x - element x
\param _y - element y
*/
/******************************************************************************/
Vector2::Vector2(float _x, float _y)
:x(_x), y(_y)
{}

/******************************************************************************/
/*!
\brief - Vector2 Copy Constructor
\param _copy - vector2 to be copied
*/
/******************************************************************************/
Vector2::Vector2(const vec2& _copy)
{
	if (this !=&_copy)
	{
		x = _copy.x;
		y = _copy.y;
	}
}

/******************************************************************************/
/*!
\brief - Vector2 < operator
\param _rhs - vector2 to be compared
\return bool
*/
/******************************************************************************/
bool Vector2::operator<(const vec2& _rhs) const
{
	return GetLengthSq(*this) < GetLengthSq(_rhs);
}

/******************************************************************************/
/*!
\brief - Vector2 == operator
\param _rhs - vector2 to be compared
\return bool
*/
/******************************************************************************/
bool Vector2::operator==(const vec2& _rhs) const
{
	return (x == _rhs.x && y == _rhs.y);
}

/******************************************************************************/
/*!
\brief - Vector2 != operator
\param _rhs - vector2 to be compared
\return bool
*/
/******************************************************************************/
bool Vector2::operator!=(const vec2& _rhs) const
{
	return (x != _rhs.x || y != _rhs.y);
}

/******************************************************************************/
/*!
\brief - Vector2 = operator
\param _rhs - vector2 to be assigned
\return *this
*/
/******************************************************************************/
Vector2& Vector2::operator=(const vec2& _rhs)
{
	if (this !=&_rhs)
	{
		x = _rhs.x;
		y = _rhs.y;
	}

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector2 unary - operator
\param _rhs - vector2 to be assigned
\return Result
*/
/******************************************************************************/
Vector2 Vector2::operator-(void)
{
	vec2 Result;

	Result.x = -x;
	Result.y = -y;

	return Result;
}

/******************************************************************************/
/*!
\brief - Vector2 += operator
\param _rhs - vector2 to be added
\return *this
*/
/******************************************************************************/
Vector2& Vector2::operator+=(const vec2& _rhs)
{
	x += _rhs.x;
	y += _rhs.y;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector2 += operator
\param _rhs - number to be added
\return *this
*/
/******************************************************************************/
Vector2& Vector2::operator+=(float _constant)
{
	x += _constant;
	y += _constant;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector2 -= operator
\param _rhs - vector2 to be subtracted
\return *this
*/
/******************************************************************************/
Vector2& Vector2::operator-=(const vec2& _rhs)
{
	x -= _rhs.x;
	y -= _rhs.y;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector2 -= operator
\param _rhs - number to be subtracted
\return *this
*/
/******************************************************************************/
Vector2& Vector2::operator-=(float _constant)
{
	x -= _constant;
	y -= _constant;

	return *this;
}

Vector2& Vector2::operator*=(const vec2& _rhs)
{
	x *= _rhs.x;
	y *= _rhs.y;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector2 *= operator
\param _rhs - vector2 to be multiplied
\return *this
*/
/******************************************************************************/
Vector2& Vector2::operator*=(float _constant)
{
	x *= _constant;
	y *= _constant;

	return *this;
}

Vector2& Vector2::operator/=(float _constant)
{
	// If _constant is not zero,
	if (_constant) {
		x /= _constant;
		y /= _constant;
	}

	// Unless.
	else
		jeDebugPrint("!Vector2 - Cannot devide by 0.\n");

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector2 + operator
\param _rhs - number to be added
\return result
*/
/******************************************************************************/
Vector2 Vector2::operator+(float _constant) const
{
	Vector2 result;

	result.x = x + _constant;
	result.y = y + _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector2 + operator
\param _rhs - vector2 to be added
\return result
*/
/******************************************************************************/
Vector2 Vector2::operator+(const vec2& _rhs) const
{
	Vector2 result;

	result.x = x + _rhs.x;
	result.y = y + _rhs.y;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector2 - operator
\param _rhs - number to be subtracted
\return result
*/
/******************************************************************************/
Vector2 Vector2::operator-(float _constant) const
{
	Vector2 result;

	result.x = x - _constant;
	result.y = y - _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector2 - operator
\param _rhs - vector2 to be subtracted
\return result
*/
/******************************************************************************/
Vector2 Vector2::operator-(const vec2& _rhs) const
{
	Vector2 result;

	result.x = x - _rhs.x;
	result.y = y - _rhs.y;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector2 * operator
\param _rhs - number to be nultiplied
\return result
*/
/******************************************************************************/
Vector2 Vector2::operator*(float _constant) const
{
	Vector2 result;

	result.x = x * _constant;
	result.y = y * _constant;

	return result;
}

Vector2 Vector2::operator*(const vec2& _rhs) const
{
	Vector2 result;

	result.x = x * _rhs.x;
	result.y = y * _rhs.y;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector2 / operator
\param _rhs - number to be divided
\return result
*/
/******************************************************************************/
Vector2 Vector2::operator/(float _constant) const
{
	Vector2 result;

	// If _constant is not zero,
	if (_constant) {
		result.x = x / _constant;
		result.y = y / _constant;
	}

	// Unless.
	else
		jeDebugPrint("!Vector2 - Cannot devide by 0.\n");

	return result;
}

/******************************************************************************/
/*!
\brief - Friend function, + operator
\param _constant - number to be added 
\param _rhs - *this
\return result
*/
/******************************************************************************/
Vector2 operator+(float _constant, const vec2& _rhs)
{
	Vector2 result;

	result = _rhs + _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Friend function, * operator
\param _constant - number to be multiplied
\param _rhs - *this
\return result
*/
/******************************************************************************/
Vector2 operator*(float _constant, const vec2& _rhs)
{
	Vector2 result;

	result = _rhs * _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Friend function, << operator
\param _constents - storage to put contents in
\param _constents - *this
\return _os
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& _os, const vec2& _constents)
{
	_os << "Vector2[ " << _constents.x << ", " << _constents.y << " ]";
	return _os;
}

jeEnd
