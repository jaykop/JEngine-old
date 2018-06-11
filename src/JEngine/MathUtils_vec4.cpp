#include "MathUtils.h"

jeBegin

namespace Math
{
	bool IsZero(CR_Vec4 _vec4)
	{
		// If both x y z are not 0,
		if (_vec4.x || _vec4.y || _vec4.z || _vec4.w)
			return false;

		// Unless,
		return true;
	}

	bool IsOne(CR_Vec4 _vec4)
	{
		// If both x y z are not 1,
		if (_vec4.x != 1.f || _vec4.y != 1.f || _vec4.z != 1.f || _vec4.w != 1.f)
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
	void Normalize(vec4& _vec4)
	{
		_vec4 = GetNormalize(_vec4);
	}

	Vector4 GetNormalize(CR_Vec4 _vec4)
	{
		Vector4 result(_vec4);

		// If this is not zero,
		if (!IsZero(_vec4))
			result = result / GetLength(_vec4);

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
	void Absolute(vec4& _vec4)
	{
		_vec4 = GetAbsolute(_vec4);
	}

	Vector4 GetAbsolute(CR_Vec4 _vec4)
	{
		Vector4 result(_vec4);

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
	float GetLength(CR_Vec4 _vec4)
	{
		return sqrt(GetLengthSq(_vec4));
	}

	float GetLengthSq(CR_Vec4 _vec4)
	{
		return _vec4.x * _vec4.x + _vec4.y * _vec4.y + _vec4.z * _vec4.z + _vec4.w * _vec4.w;
	}


	/******************************************************************************/
	/*!
	\brief - Get two Vector4s' DotProduct
	\param _left - to be calculated
	\param _right - to be calculated
	\return x * _rhs.x + y * _rhs.y + z * _rhs.z + w * _rhs.w;
	*/
	/******************************************************************************/
	float DotProduct(CR_Vec4 _left, CR_Vec4 _right)
	{
		return _left.x * _right.x + _left.y * _right.y + _left.z * _right.z + _left.w * _right.w;
	}
}

jeEnd