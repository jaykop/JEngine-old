/******************************************************************************/
/*!
\file   Vector4.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Vector4's template class and member function


*/
/******************************************************************************/

#include "MathUtils.h"

jeBegin

using namespace Math;

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
const Vector4 Vector4::ONE(1.f, 1.f, 1.f, 1.f);
const Vector4 Vector4::ZERO(0.f, 0.f, 0.f, 0.f);
const Vector4 Vector4::UNIT_X(1.f, 0.f, 0.f, 0.f);
const Vector4 Vector4::UNIT_Y(0.f, 1.f, 0.f, 0.f);
const Vector4 Vector4::UNIT_Z(0.f, 0.f, 1.f, 0.f);
const Vector4 Vector4::UNIT_W(0.f, 0.f, 0.f, 1.f);

/******************************************************************************/
/*!
\brief - Vector4 Constructor
\param _x - element x
\param _y - element y
\param _z - element z
\param _w - element w
*/
/******************************************************************************/
Vector4::Vector4(float _x, float _y, float _z, float _w)
	:x(_x), y(_y), z(_z), w(_w)
{}

Vector4::Vector4(const vec4& copy)
{
	if (this != &copy)
	{
		x = copy.x;
		y = copy.y;
		z = copy.z;
		w = copy.w;
	}
}

/******************************************************************************/
/*!
\brief - Vector4 unary - operator
\return Result
*/
/******************************************************************************/
Vector4 Vector4::operator-(void) const
{
	vec4 Result;

	Result.x = -x;
	Result.y = -y;
	Result.z = -z;
	Result.w = -w;

	return Result;
}

/******************************************************************************/
/*!
\brief - Vector4 + operator
\param rhs - number to be added
\return result
*/
/******************************************************************************/
bool Vector4::operator<(const vec4& rhs) const
{
	return GetLengthSq(*this) < GetLengthSq(rhs);
}

Vector4 Vector4::operator+(const vec4& rhs) const
{
	Vector4 result;

	result.x = x + rhs.x;
	result.y = y + rhs.y;
	result.z = z + rhs.z;
	result.w = w + rhs.w;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 + operator
\param constant - Vector4 to be added
\return result
*/
/******************************************************************************/
Vector4 Vector4::operator+(float constant) const
{
	Vector4 result;

	result.x = x + constant;
	result.y = y + constant;
	result.z = z + constant;
	result.w = w + constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 - operator
\param rhs - Vector4 to be subtracted
\return result
*/
/******************************************************************************/
Vector4 Vector4::operator-(const vec4& rhs) const
{
	Vector4 result;

	result.x = x - rhs.x;
	result.y = y - rhs.y;
	result.z = z - rhs.z;
	result.w = w - rhs.w;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 - operator
\param constant - number to be subtracted
\return result
*/
/******************************************************************************/
Vector4 Vector4::operator-(float constant) const
{
	Vector4 result;

	result.x = x - constant;
	result.y = y - constant;
	result.z = z - constant;
	result.w = w - constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 * operator
\param constant - number to be nultiplied
\return result
*/
/******************************************************************************/
Vector4 Vector4::operator*(float constant) const
{
	Vector4 result;

	result.x = x * constant;
	result.y = y * constant;
	result.z = z * constant;
	result.w = w * constant;

	return result;
}

Vector4 Vector4::operator*(const vec4& rhs) const
{
	Vector4 result;

	result.x = x * rhs.x;
	result.y = y * rhs.y;
	result.z = z * rhs.z;
	result.w = w * rhs.w;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 / operator
\param constant - number to be divided
\return result
*/
/******************************************************************************/

Vector4 Vector4::operator / (float constant) const
{
	Vector4 result;

	// If constant is not zero,
	if (constant) {
		result.x = x / constant;
		result.y = y / constant;
		result.z = z / constant;
		result.w = w / constant;
	}

	// Unless.
	else
		jeDebugPrint("!Vector4 - Cannot devide by 0.\n");

	return result;
}

Vector4& Vector4::operator=(const vec4& rhs)
{
	if (this != &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}

	return *this;
}

/******************************************************************************/
/*!
\brief - Friend function, + operator
\param constant - number to be added
\param rhs - *this
\return result
*/
/******************************************************************************/
Vector4 operator+(float constant, const vec4& rhs)
{
	Vector4 result;

	result = rhs + constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Friend function, * operator
\param constant - number to be multiplied
\param rhs - *this
\return result
*/
/******************************************************************************/
Vector4 operator*(float constant, const vec4& rhs)
{
	Vector4 result;

	result = rhs * constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Friend function, << operator
\param os - storage to put contents in
\param contents - *this
\return os
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& os, const vec4& contents)
{
	os << "Vector4[ " << contents.x << ", " << contents.y << ", " << contents.z << ", " << contents.w << " ]";
	return os;
}

/******************************************************************************/
/*!
\brief - Vector4 += operator
\param rhs - Vector4 to be added
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator+=(const vec4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector4 += operator
\param constant - number to be added
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator+=(float constant)
{
	x += constant;
	y += constant;
	z += constant;
	w += constant;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector4 -= operator
\param rhs - Vector4 to be subtracted
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator-=(const vec4& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector4 -= operator
\param constant - number to be subtracted
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator-=(float constant)
{
	x -= constant;
	y -= constant;
	z -= constant;
	w -= constant;

	return *this;
}

Vector4& Vector4::operator*=(const vec4& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector4 *= operator
\param constant - Vector4 to be multiplied
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator*=(float constant)
{
	x *= constant;
	y *= constant;
	z *= constant;
	w += constant;

	return *this;
}

Vector4& Vector4::operator/=(float constant)
{
	// If constant is not zero,
	if (constant) {
		x /= constant;
		y /= constant;
		y /= constant;
		w /= constant;
	}

	// Unless.
	else
		jeDebugPrint("!Vector4 - Cannot devide by 0.\n");

	return *this;
}

void Vector4::Set(const vec4& copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
	w = copy.w;
}

void Vector4::Set(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}


void Vector4::SetZero()
{
	x = y = z = w = 0.f;
}

void Vector4::SetOne()
{
	x = y = z = w =1.f;
}

void Vector4::SetUnitX()
{
	x = 1.f;
	y = 0.f;
	z = 0.f;
	w = 0.f;
}

void Vector4::SetUnitY()
{
	x = 0.f;
	y = 1.f;
	z = 0.f;
	w = 0.f;
}

void Vector4::SetUnitZ()
{
	x = 0.f;
	y = 0.f;
	z = 1.f;
	w = 0.f;
}

void Vector4::SetUnitW()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
	w = 1.f;
}

/******************************************************************************/
/*!
\brief - Compare == operator
\param rhs - to be compared
\return bool
*/
/******************************************************************************/
bool Vector4::operator==(const vec4& rhs) const
{
	if (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w)
		return false;

	return true;
}

/******************************************************************************/
/*!
\brief - Compare != operator
\param rhs - to be compared
\return bool
*/
/******************************************************************************/
bool Vector4::operator!=(const vec4& rhs) const
{
	if ((*this) == rhs)
		return false;

	return true;
}

jeEnd
