/******************************************************************************/
/*!
\file   MathUtils.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains MathUtils's class 

*/
/******************************************************************************/

#pragma once
#include "Macro.h"
//#include "Matrix2x2.h"
//#include "Matrix4x4.h"
//#include "Vector2.h"
//#include "Vector3.h"
//#include "Vector4.h"

NS_JE_BEGIN

//! Math Deifnitions
#define ROUNDING(x, dig)    ( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )

//! Type definitions
typedef Matrix4x4 mat44;
typedef Matrix2x2 mat22;

namespace Math
{
	static const float PI = 3.1415926535897932F;
	static const float RADIAN = 0.01745329251994329576923690768489F;
	static const float RADIAN_DEGREE = 57.2958F;

	//! Point struct
	struct Pointf
	{
		float x;
		float y;
	};

	//! Line struct
	struct Linef
	{
		Pointf p1;
		Pointf p2;
	};

	float DegToRad(float degree);
	float RadToDeg(float radian);

	vec3 Rotation(const vec3& point, float angle, const vec3& pivot);
	Pointf Rotation(const Pointf& point, float angle, const Pointf& pivot);
	Pointf Intersect_ln(const Linef& line1, const Linef& line2);
	bool Intersect_Bool(const Linef& line1, const Linef& line2);

	vec3  Intersect_ln(
		const vec3& line1_start, const vec3& line1_end,
		const vec3& line2_start, const vec3& line2_end);

	bool Intersect_Bool(
		const vec3& line1_start, const vec3& line1_end,
		const vec3& line2_start, const vec3& line2_end);

	float Distance_pt(const vec3& a, const vec3& b);
	float Distance_pt(const Pointf& a, const Pointf& b);

	float Degree_pt(const vec3& a, const vec3& b);
	float Degree_pt(const Pointf& a, const Pointf& b);

	float Distance_st(const vec3& point, const vec3& line_start, const vec3& line_end);
	float Distance_st(const Pointf& point, const Linef& line);

}

NS_JE_END