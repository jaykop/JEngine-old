/******************************************************************************/
/*!
\file   Vector3.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains Vector3's template class


*/
/******************************************************************************/

#pragma once
#include "Macro.h"
#include <iostream>

JE_BEGIN

// Vector3 class
class Vector3
{
public:

	// Constructor and destructor
	Vector3(float _x = 0, float _y = 0, float _z = 0);
	Vector3(const Vector3& _copy);
	~Vector3(void);

	// Operators
	bool		operator<(const Vector3& _rhs) const;
	bool		operator==(const Vector3& _rhs) const;
	bool		operator!=(const Vector3& _rhs) const;
	Vector3		operator+(float _constant) const;
	Vector3		operator+(const Vector3& _rhs) const;
	Vector3		operator-(float _constant) const;
	Vector3		operator-(const Vector3& _rhs) const;
	Vector3		operator*(float _constant) const;
	Vector3		operator*(const Vector3& _rhs) const;
	Vector3		operator/(float _constant) const;
	Vector3&	operator=(const Vector3& _rhs);
	Vector3&	operator-(void);
	Vector3&	operator+=(const Vector3& _rhs);
	Vector3&	operator+=(float _constant);
	Vector3&	operator-=(const Vector3& _rhs);
	Vector3&	operator-=(float _constant);
	Vector3&	operator*=(const Vector3& _rhs);
	Vector3&	operator*=(float _constant);
	Vector3&	operator/=(float _constant);

	// Transforming functions
	void		Set(const Vector3& _copy);
	void		Set(float _x, float _y, float _z);
	void		SetZero();
	void		SetOne();
	void		SetUnitX();
	void		SetUnitY();
	void		SetUnitZ();
	bool		IsZero() const;
	bool		IsOne() const;
	float		Length() const;
	float		LengthSqrt() const;
	float		DotProduct(const Vector3& _rhs);
	Vector3		CrossProduct(const Vector3& _rhs);
	void		Normalize();
	Vector3		GetNormalize() const;
	void		Reflection(const Vector3& _rhs);
	void		Absolute();
	Vector3		GetAbsolute() const;
	void		Rotation(float _angle, const Vector3& _pivot);
	void		Rotation(float _angle);
	float		GetAngle(const Vector3& _other);
	Vector3		GetRotated(float _angle, const Vector3& _pivot);
	float		GetDistance(const Vector3& _rhs);
	float		DistanceToLine(const Vector3& _lineStart, const Vector3& _lineEnd);

	// Friend Functions
	friend Vector3 operator+(float _constant, const Vector3& _rhs);
	friend Vector3 operator*(float _constant, const Vector3& _rhs);
	friend std::ostream& operator<<(std::ostream& _os, const Vector3& _constents);

	// Static variables
	static const Vector3 ZERO;
	static const Vector3 ONE;
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;

	// Static functions
	static Vector3  GetIntersection(
		const Vector3& line1_start, const Vector3& line1_end,
		const Vector3& line2_start, const Vector3& line2_end);

	static bool IsIntersected(
		const Vector3& line1_start, const Vector3& line1_end,
		const Vector3& line2_start, const Vector3& line2_end);

	// Memver variables
	float x, y, z;

private:
	
};

using vec3 = Vector3;

JE_END
