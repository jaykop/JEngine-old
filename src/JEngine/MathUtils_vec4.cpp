#include "MathUtils.h"

jeBegin

namespace Math
{
	bool IsZero(CR_Vec4 vec)
	{
		// If both x y z are not 0,
		if (vec.x || vec.y || vec.z || vec.w)
			return false;

		// Unless,
		return true;
	}

	bool IsOne(CR_Vec4 vec)
	{
		// If both x y z are not 1,
		if (vec.x != 1.f || vec.y != 1.f || vec.z != 1.f || vec.w != 1.f)
			return false;

		// Unless,
		return true;
	}

	/******************************************************************************/
	/*!
	\brief - Get normalized Vector4
	\return result
	*/
	/******************************************************************************/
	void Normalize(vec4& vec)
	{
		vec = GetNormalize(vec);
	}

	Vector4 GetNormalize(CR_Vec4 vec)
	{
		Vector4 result(vec);

		// If this is not zero,
		if (!IsZero(vec))
			result = result / GetLength(vec);

		// Unless.
		else
			jeDebugPrint("!Vector4 - Cannot devide by 0.\n");

		return result;
	}

	/******************************************************************************/
	/*!
	\brief - Calculate vector's absolute value
	\return result
	*/
	/******************************************************************************/
	void Absolute(vec4& vec)
	{
		vec = GetAbsolute(vec);
	}

	Vector4 GetAbsolute(CR_Vec4 vec)
	{
		Vector4 result(vec);

		if (result.x < 0)
			result.x = -result.x;
		if (result.y < 0)
			result.y = -result.y;
		if (result.z < 0)
			result.z = -result.z;
		if (result.w < 0)
			result.w = -result.w;

		return result;
	}

	/******************************************************************************/
	/*!
	\brief - Get Vector4's length
	\return sqrt(x*x + y*y)
	*/
	/******************************************************************************/
	float GetLength(CR_Vec4 vec)
	{
		return sqrt(GetLengthSq(vec));
	}

	float GetLengthSq(CR_Vec4 vec)
	{
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
	}


	/******************************************************************************/
	/*!
	\brief - Get two Vector4s' DotProduct
	\param left - to be calculated
	\param right - to be calculated
	\return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	*/
	/******************************************************************************/
	float DotProduct(CR_Vec4 left, CR_Vec4 right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
	}
}

jeEnd