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
#include "MathUtils.h"

jeBegin

using namespace Math;

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
const Vector3 Vector3::ONE(1.f, 1.f, 1.f);
const Vector3 Vector3::ZERO(0.f, 0.f, 0.f);
const Vector3 Vector3::UNIT_X(1.f, 0.f, 0.f);
const Vector3 Vector3::UNIT_Y(0.f, 1.f, 0.f);
const Vector3 Vector3::UNIT_Z(0.f, 0.f, 1.f);

bool Vector3::operator<(const vec3& _rhs) const
{
    return GetLengthSq(*this) < GetLengthSq(_rhs);
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
\param copy - Vector3 to be copied
*/
/******************************************************************************/
Vector3::Vector3(const vec3& copy)
{
    if (this != &copy)
    {
        x = copy.x;
        y = copy.y;
        z = copy.z;
    }
}

/******************************************************************************/
/*!
\brief - Vector3 = operator
\param _rhs - vector3 to be assigned
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator=(const vec3& _rhs)
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
\return Result.
*/
/******************************************************************************/
Vector3 Vector3::operator-(void) const
{
    vec3 Result;

    Result.x = -x;
    Result.y = -y;
    Result.z = -z;

    return Result;
}

/******************************************************************************/
/*!
\brief - Vector3 + operator
\param _constant - number to be added
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
Vector3 Vector3::operator+(const vec3& _rhs) const
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
\param _constant - number to be subtracted
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
Vector3 Vector3::operator-(const vec3& _rhs) const
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
\param _constant - number to be nultiplied
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

Vector3 Vector3::operator*(const vec3& _rhs) const
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
\param _constant - number to be divided
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
        jeDebugPrint("!Vector3 - Cannot devide by 0.\n");

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
Vector3 operator+(float _constant, const vec3& _rhs)
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
Vector3 operator*(float _constant, const vec3& _rhs)
{
    Vector3 result;

    result = _rhs * _constant;

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
std::ostream& operator<<(std::ostream& os, const vec3& contents)
{
    os << "Vector3[ " << contents.x << ", " << contents.y << ", " << contents.z << " ]";
    return os;
}

/******************************************************************************/
/*!
\brief - Vector3 += operator
\param _rhs - Vector3 to be added
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator+=(const vec3& _rhs)
{
    x += _rhs.x;
    y += _rhs.y;
    z += _rhs.z;

    return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 += operator
\param _constant - number to be added
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
Vector3& Vector3::operator-=(const vec3& _rhs)
{
    x -= _rhs.x;
    y -= _rhs.y;
    z -= _rhs.z;

    return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 -= operator
\param _constant - number to be subtracted
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

Vector3& Vector3::operator*=(const vec3& _rhs)
{
    x *= _rhs.x;
    y *= _rhs.y;
    z *= _rhs.z;

    return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 *= operator
\param _constant - Vector3 to be multiplied
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
        jeDebugPrint("!Vector3 - Cannot devide by 0.\n");

    return *this;
}

void Vector3::Set(const vec3& copy)
{
    x = copy.x;
    y = copy.y;
    z = copy.z;
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

/******************************************************************************/
/*!
\brief - Compare == operator
\param _rhs - to be compared
\return bool
*/
/******************************************************************************/
bool Vector3::operator==(const vec3& _rhs) const
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
bool Vector3::operator!=(const vec3& _rhs) const
{
    if ((*this) == _rhs)
        return false;

    return true;
}

jeEnd
