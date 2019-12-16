#pragma once
#include <limits>

// Make them static function
namespace Math {

	const static float sqrt_2 = 1.4142135623730950488016887242097f;
	const static float pi = 3.1415926535897932384626433832795f;
	const static float pi_2 = 1.5707963267948966192313216916397f;
	const static float deg_to_rad = pi / 180.0f;
	const static float rad_to_deg = 180.0f / pi;
	const static float max_f = (std::numeric_limits<float>::max)();
	const static float min_f = -max_f;
	const static float epsilon = std::numeric_limits<float>::epsilon(); // O.OO1F in some cases

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