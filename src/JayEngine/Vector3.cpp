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
	if (_constant) {
		result.x = x / _constant;
		result.y = y / _constant;
		result.z = z / _constant;
	}

	// Unless.
	else
		JE_DEBUG_PRINT("Cannot devide by 0\n");

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
	if (_constant) {
		x /= _constant;
		y /= _constant;
		y /= _constant;
	}

	// Unless.
	else
		JE_DEBUG_PRINT("Cannot devide by 0\n");

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
			JE_DEBUG_PRINT("Cannot devide by 0\n");

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
		JE_DEBUG_PRINT("Cannot devide by 0\n");

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

float Vector3::GetDistance(const Vector3 & a, const Vector3 & b)
{
	return (a-b).LengthSqrt();
}

/******************************************************************************/
/*!
\brief - Calculate the distance of point and segment.
\param point
\param line_start
\param line_end
\return distance
*/
/******************************************************************************/
float Vector3::DistanceToLine(const Vector3 & point, const Vector3 & line_start, const Vector3 & line_end)
{
	// segment is nit a segment; a point
	float length = GetDistance(line_start, line_end);
	if (!length)
		return GetDistance(line_start, point);

	// Unless...
	float projection = ((point.x - line_start.x) * (line_end.x - line_start.x) +
		(point.y - line_start.y) * (line_end.y - line_start.y)) / length;

	//
	//	1st case		2nd case		3rd case
	//		*				*				*
	//		   A						 B
	//			=========================

	// 1st case
	if (projection < 0)
		return GetDistance(line_start, point);
	// 3rd case
	else if (projection > length)
		return GetDistance(line_end, point);
	// 2nd case
	else return abs((point.y - line_start.y) * (line_end.x - line_start.x)
		- (point.x - line_start.x) * (line_end.y - line_start.y)) / length;
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

/******************************************************************************/
/*!
\brief - Get Rotated point around specific pivot point
\param point - point to be rotated
\param angle - rotate degree
\param pivot - pivot point
\return new_point
*/
/******************************************************************************/
Vector3 GetRotated(const Vector3& point, float angle, const Vector3& pivot)
{
	Vector3 new_point(point);

	float s = sinf(Math::DegToRad(angle));
	float c = cosf(Math::DegToRad(angle));

	new_point.x -= pivot.x;
	new_point.y -= pivot.y;

	float new_x = new_point.x * c - new_point.y * s;
	float new_y = new_point.x * s + new_point.y * c;

	new_point.x = new_x + pivot.x;
	new_point.y = new_y + pivot.y;

	return new_point;
}

/******************************************************************************/
/*!
\brief - Get a intersection point by two lines
\param line1 - 1st line's staring point(vector)
\param line2 - 2nd line's staring point(vector)
\param line1 - 1st line's ending point(vector)
\param line2 - 2nd line's ending point(vector)
\return Vector3
*/
/******************************************************************************/
Vector3  GetIntersection(
	const Vector3& line1_start, const Vector3& line1_end,
	const Vector3& line2_start, const Vector3& line2_end)
{
	//Get Coefficients
	float a2 = line2_end.y - line2_start.y;
	float b2 = line2_start.x - line2_end.x;
	float c2 = line2_end.x * line2_start.y - line2_start.x * line2_end.y;

	float a1 = line1_end.y - line1_start.y;
	float b1 = line1_start.x - line1_end.x;
	float c1 = line1_end.x * line1_start.y - line1_start.x * line1_end.y;

	//Check if they are parallel
	float D = a1 * b2 - a2 * b1;
	
	if (!D)
		JE_DEBUG_PRINT("is invalid.\nCannot calculate properly...\n");

	return Vector3((b1*c2 - b2*c1) / D, (a2*c1 - a1*c2) / D);
}

/******************************************************************************/
/*!
\brief - Check if two line is intersected or not; for vectors.
\param line1 - 1st line's staring point(vector)
\param line2 - 2nd line's staring point(vector)
\param line1 - 1st line's ending point(vector)
\param line2 - 2nd line's ending point(vector)
\return bool
*/
/******************************************************************************/
bool IsIntersected(
	const Vector3& line1_start, const Vector3& line1_end,
	const Vector3& line2_start, const Vector3& line2_end)
{
	//float s1_x, s1_y, s2_x, s2_y;
	//s1_x = line1_end.x - line1_start.x;     s1_y = line1_end.y - line1_start.y;
	//s2_x = line2_end.x - line2_start.x;     s2_y = line2_end.y - line2_start.y;

	//float s, t;
	//s = (-s1_y * (line1_start.x - line2_start.x) + s1_x * (line1_start.y - line2_start.y)) / (-s2_x * s1_y + s1_x * s2_y);
	//t = (s2_x * (line1_start.y - line2_start.y) - s2_y * (line1_start.x - line2_start.x)) / (-s2_x * s1_y + s1_x * s2_y);

	//if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	//	return true;
	//

	//return false;

	float denominator = ((line1_end.x - line1_start.x) * (line2_end.y - line2_start.y)) - ((line1_end.y - line1_start.y) * (line2_end.x - line2_start.x));
	float numerator1 = ((line1_start.y - line2_start.y) * (line2_end.x - line2_start.x)) - ((line1_start.x - line2_start.x) * (line2_end.y - line2_start.y));
	float numerator2 = ((line1_start.y - line2_start.y) * (line1_end.x - line1_start.x)) - ((line1_start.x - line2_start.x) * (line1_end.y - line1_start.y));

	if (!denominator) return numerator1 == 0 && numerator2 == 0;
	float r = numerator1 / denominator;
	float s = numerator2 / denominator;

	return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

/******************************************************************************/
/*!
\brief - Calculate the degree of two point.
\param a - 1st point
\param b - 2nd point
\return distance
*/
/******************************************************************************/
float GetDegree(const Vector3 & a, const Vector3 & b)
{
	float result = atan2(b.y - a.y, b.x - a.x) * Math::RADIAN_DEGREE;;
	return result < 0 ? 360 + result : result;
}

NS_JE_END
