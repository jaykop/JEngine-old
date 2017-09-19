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
#include "Vector4.h"

NS_JE_BEGIN

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
\brief - Vector4 Destructor
*/
/******************************************************************************/
Vector4::~Vector4(void)
{}

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

Vector4::Vector4(const Vector4& _copy)
{
	if (this != &_copy)
	{
		x = _copy.x;
		y = _copy.y;
		z = _copy.z;
		w = _copy.w;
	}
}

/******************************************************************************/
/*!
\brief - Vector4 unary - operator
\param _rhs - Vector4 to be assigned
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator-(void)
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector4 + operator
\param _rhs - number to be added
\return result
*/
/******************************************************************************/
bool Vector4::operator<(const Vector4& _rhs) const
{
	return this->LengthSqrt() < _rhs.LengthSqrt();
}

Vector4 Vector4::operator+(const Vector4& _rhs) const
{
	Vector4 result;

	result.x = x + _rhs.x;
	result.y = y + _rhs.y;
	result.z = z + _rhs.z;
	result.w = w + _rhs.w;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 + operator
\param _rhs - Vector4 to be added
\return result
*/
/******************************************************************************/
Vector4 Vector4::operator+(float _constant) const
{
	Vector4 result;

	result.x = x + _constant;
	result.y = y + _constant;
	result.z = z + _constant;
	result.w = w + _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 - operator
\param _rhs - Vector4 to be subtracted
\return result
*/
/******************************************************************************/
Vector4 Vector4::operator-(const Vector4& _rhs) const
{
	Vector4 result;

	result.x = x - _rhs.x;
	result.y = y - _rhs.y;
	result.z = z - _rhs.z;
	result.w = w - _rhs.w;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 - operator
\param _rhs - number to be subtracted
\return result
*/
/******************************************************************************/
Vector4 Vector4::operator-(float _constant) const
{
	Vector4 result;

	result.x = x - _constant;
	result.y = y - _constant;
	result.z = z - _constant;
	result.w = w - _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 * operator
\param _rhs - number to be nultiplied
\return result
*/
/******************************************************************************/
Vector4 Vector4::operator*(float _constant) const
{
	Vector4 result;

	result.x = x * _constant;
	result.y = y * _constant;
	result.z = z * _constant;
	result.w = w * _constant;

	return result;
}

Vector4 Vector4::operator*(const Vector4& _rhs) const
{
	Vector4 result;

	result.x = x * _rhs.x;
	result.y = y * _rhs.y;
	result.z = z * _rhs.z;
	result.w = w * _rhs.w;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector4 / operator
\param _rhs - number to be divided
\return result
*/
/******************************************************************************/

Vector4 Vector4::operator / (float _constant) const
{
	Vector4 result;

	// If _constant is not zero,
	if (!_constant) {
		result.x = x / _constant;
		result.y = y / _constant;
		result.z = z / _constant;
		result.w = w / _constant;
	}

	// Unless.
	else
		JE_DEBUG_PRINT("Cannot devide by 0");

	return result;
}

Vector4& Vector4::operator=(const Vector4& _rhs)
{
	if (this != &_rhs)
	{
		x = _rhs.x;
		y = _rhs.y;
		z = _rhs.z;
		w = _rhs.w;
	}

	return *this;
}

/******************************************************************************/
/*!
\brief - Friend function, + operator
\param _constant - number to be added
\param _rhs - *this
\return result
*/
/******************************************************************************/
Vector4 operator+(float _constant, const Vector4& _rhs)
{
	Vector4 result;

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
Vector4 operator*(float _constant, const Vector4& _rhs)
{
	Vector4 result;

	result = _rhs * _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Friend function, << operator
\param _constant - storage to put contents in
\param _rhs - *this
\return os
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& os, const Vector4& contents)
{
	os << "Vector4[ " << contents.x << ", " << contents.y << ", " << contents.z << ", " << contents.w << " ]";
	return os;
}

/******************************************************************************/
/*!
\brief - Get two Vector4s' DotProduct
\param _rhs - to be calculated
\return x * _rhs.x + y * _rhs.y + z * _rhs.z + w * _rhs.w;
*/
/******************************************************************************/
float Vector4::DotProduct(const Vector4& _rhs)
{
	return x * _rhs.x + y * _rhs.y + z * _rhs.z + w * _rhs.w;
}

/******************************************************************************/
/*!
\brief - Vector4 += operator
\param _rhs - Vector4 to be added
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator+=(const Vector4& _rhs)
{
	x += _rhs.x;
	y += _rhs.y;
	z += _rhs.z;
	w += _rhs.w;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector4 += operator
\param _rhs - number to be added
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator+=(float _constant)
{
	x += _constant;
	y += _constant;
	z += _constant;
	w += _constant;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector4 -= operator
\param _rhs - Vector4 to be subtracted
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator-=(const Vector4& _rhs)
{
	x -= _rhs.x;
	y -= _rhs.y;
	z -= _rhs.z;
	w -= _rhs.w;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector4 -= operator
\param _rhs - number to be subtracted
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator-=(float _constant)
{
	x -= _constant;
	y -= _constant;
	z -= _constant;
	w -= _constant;

	return *this;
}

Vector4& Vector4::operator*=(const Vector4& _rhs)
{
	x *= _rhs.x;
	y *= _rhs.y;
	z *= _rhs.z;
	w *= _rhs.w;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector4 *= operator
\param _rhs - Vector4 to be multiplied
\return *this
*/
/******************************************************************************/
Vector4& Vector4::operator*=(float _constant)
{
	x *= _constant;
	y *= _constant;
	z *= _constant;
	w += _constant;

	return *this;
}

Vector4& Vector4::operator/=(float _constant)
{
	// If _constant is not zero,
	if (!_constant) {
		x /= _constant;
		y /= _constant;
		y /= _constant;
		w /= _constant;
	}

	// Unless.
	else
		JE_DEBUG_PRINT("Cannot devide by 0");

	return *this;
}

void Vector4::Set(const Vector4 & _copy)
{
	x = _copy.x;
	y = _copy.y;
	z = _copy.z;
	w = _copy.w;
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

bool Vector4::IsZero() const
{
	// If both x y z are not 0,
	if (x || y || z || w)
		return false;

	// Unless,
	return true;
}

bool Vector4::IsOne() const
{
	// If both x y z are not 1,
	if (x != 1.f || y != 1.f || z != 1.f || w != 1.f)
		return false;

	// Unless,
	return true;
}

/******************************************************************************/
/*!
\brief - Get normalized Vector4
\return result
*/
/******************************************************************************/
Vector4& Vector4::Normalize(void)
{
	// If this is not zero,
	if (!IsZero())
		*this = *this / Length();

	// Unless.
	else
		JE_DEBUG_PRINT("Cannot devide by 0");

	return *this;
}

Vector4 Vector4::GetNormalize() const
{
	Vector4 result(*this);

	// If this is not zero,
	if (!IsZero())
		result = result / Length();

	// Unless.
	else
		JE_DEBUG_PRINT("Cannot devide by 0");

	return result;
}

/******************************************************************************/
/*!
\brief - Calculate vector's absolute value
\return result
*/
/******************************************************************************/
Vector4& Vector4::Absolute()
{
	if (x < 0)
		x = -x;
	if (y < 0)
		y = -y;
	if (z < 0)
		z = -z;
	if (w < 0)
		w = -w;

	return *this;
}

Vector4 Vector4::GetAbsolute() const
{
	Vector4 result(x, y, z, w);

	if (result.x < 0)
		result.x = -result.x;
	if (result.y < 0)
		result.y = -result.y;
	if (result.z < 0)
		result.z = -result.z;
	if (result.w < 0)
		result.w = -result.w;

	return result;
}

/******************************************************************************/
/*!
\brief - Get Vector4's length
\return sqrt(x*x + y*y)
*/
/******************************************************************************/
float Vector4::Length(void) const
{
	return sqrt(LengthSqrt());
}

float Vector4::LengthSqrt() const
{
	return x*x + y*y + z*z + w*w;
}

/******************************************************************************/
/*!
\brief - Compare == operator
\param _rhs - to be compared
\return bool
*/
/******************************************************************************/
bool Vector4::operator==(const Vector4& _rhs) const
{
	if (x != _rhs.x || y != _rhs.y || z != _rhs.z || w != _rhs.w)
		return false;

	return true;
}

/******************************************************************************/
/*!
\brief - Compare != operator
\param _rhs - to be compared
\return bool
*/
/******************************************************************************/
bool Vector4::operator!=(const Vector4& _rhs) const
{
	if ((*this) == _rhs)
		return false;

	return true;
}

NS_JE_END