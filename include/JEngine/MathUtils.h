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
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

jeBegin

//! Math Deifnitions
#define jeRounding(x, dig)	( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )

namespace Math
{
	static const float ZERO				= 0.F;
	static const float ONE				= 1.F;
	static const float PREPENDICULAR	= 90.F;
	static const float COLINEAR			= 180.F;
	static const float ROUND			= 360.F;
	static const float PI				= 3.1415926535897932F;
	static const float RADIAN			= 0.01745329251994329576923690768489F;
	static const float RADIAN_TO_DEGREE	= 57.2958F;

	float DegToRad(float degree);
	float RadToDeg(float radian);

	//////////////////////////////////////////////////////////////////////////
	// Matrix 4x4
	//////////////////////////////////////////////////////////////////////////
	mat4	GetTranspose(CR_Mat4 mat);
	void	Transpose(mat4& mat);
	mat4	GetInverse(CR_Mat4 mat);
	void	Inverse(mat4& mat);
	void	SetIdentity(mat4& mat, float diagonal = 1.f);

	mat4 Translate(CR_Vec3 vec);
	mat4 Scale(CR_Vec3 vec);
	mat4 Rotate(float radian, CR_Vec3 vec);
	mat4 RotateX(float radian);
	mat4 RotateY(float radian);
	mat4 RotateZ(float radian);
	mat4 Perspective(float fovy, float aspect, float zNear, float zFar);
	mat4 Orthogonal(float left, float right, float bottom, float top, float zNear, float zFar);
	mat4 Orthogonal(float left, float right, float bottom, float top);
	mat4 LookAt(CR_Vec3 eye, CR_Vec3 target, CR_Vec3 up);

	//////////////////////////////////////////////////////////////////////////
	// Vector 2
	//////////////////////////////////////////////////////////////////////////
	bool	IsZero(CR_Vec2 vec2);
	bool	IsOne(CR_Vec2 vec2);
	float	GetLength(CR_Vec2 vec2);
	float	GetLengthSq(CR_Vec2 vec2);
	float	DotProduct(CR_Vec2 rhs);
	Vector2	CrossProduct(CR_Vec2 rhs);
	void	Normalize(vec2& vec2);
	Vector2	GetNormalize(CR_Vec2 vec2);
	void	Absolute(vec2& vec2);
	Vector2	GetAbsolute(CR_Vec2 vec2);
	float	GetAngle(CR_Vec2 vec2);

	//////////////////////////////////////////////////////////////////////////
	// Vector 3
	//////////////////////////////////////////////////////////////////////////
	bool	IsZero(CR_Vec3 vec3);
	bool	IsOne(CR_Vec3 vec3);
	float	GetLength(CR_Vec3 vec3);
	float	GetLengthSq(CR_Vec3 vec3);
	float	DotProduct(CR_Vec3 left, CR_Vec3 right);
	Vector3	CrossProduct(CR_Vec3 left, CR_Vec3 right);
	void	Normalize(vec3& vec3);
	Vector3	GetNormalize(CR_Vec3 vec3);
	void	Absolute(vec3& vec3);
	Vector3	GetAbsolute(CR_Vec3 vec3);
	void	Rotate(vec3& vec3, float angle, CR_Vec3 pivot = vec3::ZERO);
	Vector3	GetRotated(CR_Vec3 vec3, float angle, CR_Vec3 pivot = vec3::ZERO);
	void	Reflected(vec3& vec3, CR_Vec3 pivot);
	Vector3	GetReflected(CR_Vec3 vec3, CR_Vec3 pivot);
	float	GetAngle(CR_Vec3 vec3, CR_Vec3 pivot);

	//float	DistanceToLine(CR_Vec3 vec3, CR_Vec3 _lineStart, CR_Vec3 _lineEnd);

	//Vector3 GetSegmentIntersection(
	//	CR_Vec3 line1_start, CR_Vec3 line1_end,
	//	CR_Vec3 line2_start, CR_Vec3 line2_end);

	//bool IsSegmentIntersection(
	//	CR_Vec3 line1_start, CR_Vec3 line1_end,
	//	CR_Vec3 line2_start, CR_Vec3 line2_end);

	//////////////////////////////////////////////////////////////////////////
	// Vector 4
	//////////////////////////////////////////////////////////////////////////
	bool		IsZero(CR_Vec4 vec4);
	bool		IsOne(CR_Vec4 vec4);
	float		GetLength(CR_Vec4 vec4);
	float		GetLengthSq(CR_Vec4 vec4);
	float		DotProduct(CR_Vec4 left, CR_Vec4 right);
	void		Normalize(vec4& vec4);
	Vector4		GetNormalize(CR_Vec4 vec4);
	void		Absolute(vec4& vec4);
	Vector4		GetAbsolute(CR_Vec4 vec4);
}

jeEnd
