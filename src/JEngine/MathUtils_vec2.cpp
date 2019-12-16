#include "MathUtils.h"

jeBegin

namespace Math {

	/******************************************************************************/
	/*!
	\brief - Get Vector2's length
	\param vec
	\return sqrt(x*x + y*y)
	*/
	/******************************************************************************/
	float GetLength(CR_Vec2 vec)
	{
		return sqrt(GetLengthSq(vec));
	}

	float GetLengthSq(CR_Vec2 vec)
	{
		return vec.x * vec.x + vec.y * vec.y;
	}

	/******************************************************************************/
	/*!
	\brief - Get two Vector2s' DotProduct
	\param left - to be calculated
	\param right - to be calculated
	\return x * rhs.x + y * rhs.y;
	*/
	/******************************************************************************/
	float DotProduct(CR_Vec2 left, CR_Vec2 right)
	{
		return left.x * right.x + left.y * right.y;
	}

	/******************************************************************************/
	/*!
	\brief - Get two Vector2s' CrossProduct
	\param left - to be calculated
	\param right - to be calculated
	\return x * x * rhs.y - y * rhs.x;
	*/
	/******************************************************************************/
	Vector2 CrossProduct(CR_Vec2 left, CR_Vec2 right)
	{
		return left.x * right.y - left.y * right.x;
	}

	/******************************************************************************/
	/*!
	\brief - Get normalized vector2
	\param vec
	*/
	/******************************************************************************/
	void Normalize(vec2& vec)
	{
		vec = GetNormalize(vec);
	}

	Vector2 GetNormalize(CR_Vec2 vec)
	{
		Vector2 result(vec);

		// If this is not zero,
		if (!IsZero(vec))
			result = result / GetLength(vec);

		// Unless.
		else
			jeDebugPrint("!Vector2 - Cannot devide by 0.\n");

		return result;
	}

	void Absolute(vec2& vec)
	{
		vec = GetAbsolute(vec);
	}

	Vector2 GetAbsolute(CR_Vec2 vec)
	{
		Vector2 result(vec);

		if (result.x < 0)
			result.x = -result.x;
		if (result.y < 0)
			result.y = -result.y;

		return result;
	}

	float GetAngle(CR_Vec2 vec)
	{
		return atan2(vec.y, vec.x);
	}

	bool IsZero(CR_Vec2 vec)
	{
		// If both x y are not 0,
		if (vec.x || vec.y)
			return false;

		// Unless,
		return true;
	}

	bool IsOne(CR_Vec2 vec)
	{
		// If both x y are not 1,
		if (vec.x != 1.f || vec.y != 1.f)
			return false;

		// Unless,
		return true;
	}
}

jeEnd