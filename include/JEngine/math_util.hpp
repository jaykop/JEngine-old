#pragma once
#include <limits>

//! Math Deifnitions
#define jeRounding(x, dig)	( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )

namespace Math
{
	static const float zero = 0.F;
	static const float one = 1.F;
	static const float perpendicular = 90.F;
	static const float colinear = 180.F;
	static const float round = 360.F;
	static const float radian = 0.01745329251994329576923690768489F;
	static const float radian_to_degree = 57.2958F;
	const static float sqrt_2 = 1.4142135623730950488016887242097f;
	const static float pi = 3.1415926535897932384626433832795f;
	const static float pi_2 = 1.5707963267948966192313216916397f;
	const static float max_f = (std::numeric_limits<float>::max)();
	const static float min_f = -max_f;
	const static float epsilon = std::numeric_limits<float>::epsilon(); // O.OO1F in some cases

	static float deg_to_rad(float deg)
	{ 
		return deg * static_cast<float>(radian); 
	}

	static float rad_to_deg(float rad)	
	{ 
		return rad * static_cast<float>(radian_to_degree); 
	}

	static inline float get_max(float a, float b)
	{
		if (a > b) return a;
		else return b;
	}

	static inline float get_min(float a, float b)
	{
		if (a < b) return a;
		else return b;
	}

	static inline bool float_equal(float lhs, float rhs)
	{
		float diff = lhs - rhs;
		return (diff < epsilon && -diff < epsilon);
	}
}