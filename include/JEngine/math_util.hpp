#pragma once
#include <macros.hpp>

jeBegin

//! Math Deifnitions
#define jeRounding(x, dig)	( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )

namespace Math
{
	static const float zero = 0.F;
	static const float one = 1.F;
	static const float perpendicular = 90.F;
	static const float colinear = 180.F;
	static const float round = 360.F;
	static const float pi = 3.1415926535897932F;
	static const float radian = 0.01745329251994329576923690768489F;
	static const float radian_to_degree = 57.2958F;

	float deg_to_rad(float degree)	{ return degree * static_cast<float>(radian); }

	float rad_to_deg(float radian)	{ return radian * static_cast<float>(radian_to_degree);	}
}

jeEnd