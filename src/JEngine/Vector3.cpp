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

bool Vector3::operator<(const vec3& rhs) const
{
    return GetLengthSq(*this) < GetLengthSq(rhs);
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
\param rhs - vector3 to be assigned
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator=(const vec3& rhs)
{
    if (this != &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
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
\param constant - number to be added
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator+(float constant) const
{
    Vector3 result;

    result.x = x + constant;
    result.y = y + constant;
    result.z = z + constant;

    return result;
}

/******************************************************************************/
/*!
\brief - Vector3 + operator
\param rhs - Vector3 to be added
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator+(const vec3& rhs) const
{
    Vector3 result;

    result.x = x + rhs.x;
    result.y = y + rhs.y;
    result.z = z + rhs.z;

    return result;
}

/******************************************************************************/
/*!
\brief - Vector3 - operator
\param constant - number to be subtracted
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator-(float constant) const
{
    Vector3 result;

    result.x = x - constant;
    result.y = y - constant;
    result.z = z - constant;

    return result;
}

/******************************************************************************/
/*!
\brief - Vector3 - operator
\param rhs - Vector3 to be subtracted
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator-(const vec3& rhs) const
{
    Vector3 result;

    result.x = x - rhs.x;
    result.y = y - rhs.y;
    result.z = z - rhs.z;

    return result;
}

/******************************************************************************/
/*!
\brief - Vector3 * operator
\param constant - number to be nultiplied
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator*(float constant) const
{
    Vector3 result;

    result.x = x * constant;
    result.y = y * constant;
    result.z = z * constant;

    return result;
}

Vector3 Vector3::operator*(const vec3& rhs) const
{
    Vector3 result;

    result.x = x * rhs.x;
    result.y = y * rhs.y;
    result.z = z * rhs.z;

    return result;
}

/******************************************************************************/
/*!
\brief - Vector3 / operator
\param constant - number to be divided
\return result
*/
/******************************************************************************/
Vector3 Vector3::operator/(float constant) const
{
    Vector3 result;

    // If constant is not zero,
    if (constant) {
        result.x = x / constant;
        result.y = y / constant;
        result.z = z / constant;
    }

    // Unless.
    else
        jeDebugPrint("!Vector3 - Cannot devide by 0.\n");

    return result;
}

/******************************************************************************/
/*!
\brief - Friend function, + operator
\param constant - number to be added
\param rhs - *this
\return result
*/
/******************************************************************************/
Vector3 operator+(float constant, const vec3& rhs)
{
    Vector3 result;

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
Vector3 operator*(float constant, const vec3& rhs)
{
    Vector3 result;

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
std::ostream& operator<<(std::ostream& os, const vec3& contents)
{
    os << "Vector3[ " << contents.x << ", " << contents.y << ", " << contents.z << " ]";
    return os;
}

/******************************************************************************/
/*!
\brief - Vector3 += operator
\param rhs - Vector3 to be added
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator+=(const vec3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 += operator
\param constant - number to be added
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator+=(float constant)
{
    x += constant;
    y += constant;
    z += constant;

    return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 -= operator
\param rhs - Vector3 to be subtracted
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator-=(const vec3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 -= operator
\param constant - number to be subtracted
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator-=(float constant)
{
    x -= constant;
    y -= constant;
    z -= constant;

    return *this;
}

Vector3& Vector3::operator*=(const vec3& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;

    return *this;
}

/******************************************************************************/
/*!
\brief - Vector3 *= operator
\param constant - Vector3 to be multiplied
\return *this
*/
/******************************************************************************/
Vector3& Vector3::operator*=(float constant)
{
    x *= constant;
    y *= constant;
    z *= constant;

    return *this;
}

Vector3& Vector3::operator/=(float constant)
{
    // If constant is not zero,
    if (constant) {
        x /= constant;
        y /= constant;
        y /= constant;
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
\param rhs - to be compared
\return bool
*/
/******************************************************************************/
bool Vector3::operator==(const vec3& rhs) const
{
    if (x != rhs.x || y != rhs.y || z != rhs.z)
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
bool Vector3::operator!=(const vec3& rhs) const
{
    if ((*this) == rhs)
        return false;

    return true;
}

jeEnd
