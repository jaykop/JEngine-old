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

NS_JE_BEGIN

//! Math Deifnitions
#define ROUNDING(x, dig)    ( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )

namespace Math
{
	static const float PI = 3.1415926535897932F;
	static const float RADIAN = 0.01745329251994329576923690768489F;
	static const float RADIAN_DEGREE = 57.2958F;

	float DegToRad(float degree);
	float RadToDeg(float radian);

}

NS_JE_END