/******************************************************************************/
/*!
\file   Vector3.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains vector3's template class and member function


*/
/******************************************************************************/

#include <cmath>
#include "Vector3.h"
#include "MathUtils.h"

NS_JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
const Vector3 Vector3::ONE(1.f, 1.f, 1.f);
const Vector3 Vector3::ZERO(0.f, 0.f, 0.f);
const Vector3 Vector3::UNIT_X(1.f, 0.f, 0.f);
const Vector3 Vector3::UNIT_Y(0.f, 1.f, 0.f);
const Vector3 Vector3::UNIT_Z(0.f, 0.f, 1.f);

/******************************************************************************/
/*!
\brief - Vector3 Destructor
*/
/******************************************************************************/
Vector3::~Vector3(void)
{}

bool Vector3::operator<(const Vector3 & _rhs) const
{
	return this->LengthSqrt() < _rhs.LengthSqrt();
}

/******************************************************************************/
/*!
\brief - Vector3 Constructor
\param _x - element x
\param _y - element y
\param _z - element z
*/
/******************************************************************************/
Vector3::Vector3(float _x, float _y, float _z)
: x(_x), y(_y), z(_z)
{}

/******************************************************************************/
/*!
\brief - Vector3 Copy Constructor
\param _copy - Vector3 to be copied
*/
/******************************************************************************/
Vector3::Vector3(const Vector3& _copy)
{
	if (this != &_copy)
	{
		x = _copy.x;
		y = _copy.y;
		z = _copy.z;
	}
}

/******************************************************************************/
/*!
\brief - Vector3 = operator
\param _rhs - vector3 to be assigned
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator=(const Vector3& _rhs)
{
	if (this != &_rhs)
	{
		x = _rhs.x;
		y = _rhs.y;
		z = _rhs.z;
	}

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 unary - operator
\param _rhs - Vector3 to be assigned
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator-(void)
{
	x = -x;
	y = -y;
	z = -z;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 + operator
\param _rhs - number to be added
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator+(float _constant) const
{
	Vector3 result;

	result.x = x + _constant;
	result.y = y + _constant;
	result.z = z + _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector3 + operator
\param _rhs - Vector3 to be added
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator+(const Vector3& _rhs) const
{
	Vector3 result;

	result.x = x + _rhs.x;
	result.y = y + _rhs.y;
	result.z = z + _rhs.z;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector3 - operator
\param _rhs - number to be subtracted
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator-(float _constant) const
{
	Vector3 result;

	result.x = x - _constant;
	result.y = y - _constant;
	result.z = z - _constant;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector3 - operator
\param _rhs - Vector3 to be subtracted
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator-(const Vector3& _rhs) const
{
	Vector3 result;

	result.x = x - _rhs.x;
	result.y = y - _rhs.y;
	result.z = z - _rhs.z;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector3 * operator
\param _rhs - number to be nultiplied
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator*(float _constant) const
{
	Vector3 result;

	result.x = x * _constant;
	result.y = y * _constant;
	result.z = z * _constant;

	return result;
}

Vector3 Vector3::operator*(const Vector3 & _rhs) const
{
	Vector3 result;

	result.x = x * _rhs.x;
	result.y = y * _rhs.y;
	result.z = z * _rhs.z;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector3 / operator
\param _rhs - number to be divided
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator/(float _constant) const
{
	Vector3 result;

	// If _constant is not zero,
	if (!_constant) {
		result.x = x / _constant;
		result.y = y / _constant;
		result.z = z / _constant;
	}

	// Unless.
	else
		JE_DEBUG_PRINT("Cannot devide by 0");

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
Vector3 operator+(float _constant, const Vector3& _rhs)
{
	Vector3 result;

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
Vector3 operator*(float _constant, const Vector3& _rhs)
{
	Vector3 result;

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
std::ostream& operator<<(std::ostream& os, const Vector3& contents)
{
	os << "Vector3[ " << contents.x << ", " << contents.y << ", " << contents.z << " ]";
	return os;
}

/******************************************************************************/
/*!
\brief - Get two vector3s' DotProduct
\param _rhs - to be calculated
\return x * _rhs.x + y * _rhs.y;
*/
/******************************************************************************/
float Vector3::DotProduct(const Vector3& _rhs)
{
	return x * _rhs.x + y * _rhs.y + z * _rhs.z;
}

/******************************************************************************/
/*!
\brief - Get two vector3s' CrossProduct
\param _rhs - to be calculated
\return x * x * _rhs.y - y * _rhs.x;
*/
/******************************************************************************/
Vector3 Vector3::CrossProduct(const Vector3& _rhs)
{
	Vector3 result;

	result.x = y * _rhs.z - z * _rhs.y;
	result.y = z * _rhs.x - x * _rhs.z;
	result.z = x * _rhs.y - y * _rhs.x;

	return result;
}

/******************************************************************************/
/*!
\brief - Vector3 += operator
\param _rhs - Vector3 to be added
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator+=(const Vector3& _rhs)
{
	x += _rhs.x;
	y += _rhs.y;
	z += _rhs.z;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 += operator
\param _rhs - number to be added
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator+=(float _constant)
{
	x += _constant;
	y += _constant;
	z += _constant;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 -= operator
\param _rhs - Vector3 to be subtracted
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator-=(const Vector3& _rhs)
{
	x -= _rhs.x;
	y -= _rhs.y;
	z -= _rhs.z;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 -= operator
\param _rhs - number to be subtracted
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator-=(float _constant)
{
	x -= _constant;
	y -= _constant;
	z -= _constant;

	return *this;
}

Vector3& Vector3::operator*=(const Vector3 & _rhs)
{
	x *= _rhs.x;
	y *= _rhs.y;
	z *= _rhs.z;

	return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 *= operator
\param _rhs - Vector3 to be multiplied
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator*=(float _constant)
{
	x *= _constant;
	y *= _constant;
	z *= _constant;

	return *this;
}

Vector3& Vector3::operator/=(float _constant)
{
	// If _constant is not zero,
	if (!_constant) {
		x /= _constant;
		y /= _constant;
		y /= _constant;
	}

	// Unless.
	else
		JE_DEBUG_PRINT("Cannot devide by 0");

	return *this;
}

void Vector3::Set(const Vector3 & _copy)
{
	x = _copy.x;
	y = _copy.y;
	z = _copy.z;
}

void Vector3::Set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}


void Vector3::SetZero()
{
	x = y = z = 0.f;
}

void Vector3::SetOne()
{
	x = y = z = 1.f;
}

void Vector3::SetUnitX()
{
	x = 1.f;
	y = 0.f;
	z = 0.f;
}

void Vector3::SetUnitY()
{
	x = 0.f;
	y = 1.f;
	z = 0.f;
}

void Vector3::SetUnitZ()
{
	x = 0.f;
	y = 0.f;
	z = 1.f;
}

bool Vector3::IsZero() const
{
	// If both x y z are not 0,
	if (x || y || z)
		return false;

	// Unless,
	return true;
}

bool Vector3::IsOne() const
{
	// If both x y z are not 1,
	if (x != 1.f || y != 1.f || z != 1.f)
		return false;

	// Unless,
	return true;
}
/******************************************************************************/
/*!
\brief - Get normalized vector3
\return result
*/
/******************************************************************************/
Vector3& Vector3::Normalize(void)
{
		// If this is not zero,
		if (!IsZero())
			*this = *this / Length();

		// Unless.
		else
			JE_DEBUG_PRINT("Cannot devide by 0");

		return *this;
}

Vector3 Vector3::GetNormalize() const
{
	Vector3 result(*this);

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
\brief - Get vector3's length
\return sqrt(x*x + y*y)
*/
/******************************************************************************/
float Vector3::Length(void) const
{
	return sqrt(LengthSqrt());
}

float Vector3::LengthSqrt() const
{
	return x*x + y*y + z*z;
}

/******************************************************************************/
/*!
\brief - Calculate vector's absolute value
\return result
*/
/******************************************************************************/
Vector3& Vector3::Absolute()
{
	if (x < 0)
		x = -x;
	if (y < 0)
		y = -y;
	if (z < 0)
		z = -z;

	return *this;
}

Vector3 Vector3::GetAbsolute() const
{
	Vector3 result(x, y, z);

	if (result.x < 0)
		result.x = -result.x;
	if (result.y < 0)
		result.y = -result.y;
	if (result.z < 0)
		result.z = -result.z;

	return result;
}

/******************************************************************************/
/*!
\brief - Calculate vector's rotated position; Regard vector as a point
\param angle - degree to rotate
\param pivot - pivot point
\return point
*/
/******************************************************************************/
Vector3 Vector3::Rotation(float angle, const Vector3& pivot)
{
	Vector3 point(*this);

	float s = sinf(Math::DegToRad(angle));
	float c = cosf(Math::DegToRad(angle));

	point.x -= pivot.x;
	point.y -= pivot.y;

	float new_x = point.x * c - point.y * s;
	float new_y = point.x * s + point.y * c;

	point.x = new_x + pivot.x;
	point.y = new_y + pivot.y;

	return point;
}

/******************************************************************************/
/*!
\brief - Calculate vector's reflection
\param _rhs - vector to reflect
\return reflected
*/
/******************************************************************************/
Vector3 Vector3::Reflection(const Vector3&  _rhs)
{
	Vector3 reflected;
	Vector3 norm = _rhs.GetNormalize();
	
	reflected = (*this) - 2 * ((*this).DotProduct(norm)) * norm;

	return reflected;
}

/******************************************************************************/
/*!
\brief - Calculate 2 vectors' included angle
\param other - 2nd vector
\return Math::RadToDeg(radian)
*/
/******************************************************************************/
float Vector3::GetAngle(const Vector3& _other)
{
	float radian = atan2(x * _other.y - _other.x * y, (*this).DotProduct(_other));

	return Math::RadToDeg(radian);
}

/******************************************************************************/
/*!
\brief - Calculate rotated vector
\param angle - degree to rotate
\return result
*/
/******************************************************************************/
Vector3 Vector3::Rotation(float _angle)
{
	Vector3 result;

	result.x = x * cosf(Math::DegToRad(_angle)) - y * sinf(Math::DegToRad(_angle));
	result.y = x * sinf(Math::DegToRad(_angle)) + y * cosf(Math::DegToRad(_angle));

	return result;
}

/******************************************************************************/
/*!
\brief - Compare == operator
\param _rhs - to be compared
\return bool
*/
/******************************************************************************/
bool Vector3::operator==(const Vector3& _rhs) const
{
	if (x != _rhs.x || y != _rhs.y || z != _rhs.z)
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
bool Vector3::operator!=(const Vector3& _rhs) const
{
	if ((*this) == _rhs)
		return false;

	return true;
}

NS_JE_END
