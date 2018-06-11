#include "MathUtils.h"

jeBegin

namespace Math {

	/******************************************************************************/
	/*!
	\brief - Get Vector2's length
	\param _vec2
	\return sqrt(x*x + y*y)
	*/
	/******************************************************************************/
	float GetLength(CR_Vec2 _vec2)
	{
		return sqrt(GetLengthSq(_vec2));
	}

	float GetLengthSq(CR_Vec2 _vec2)
	{
		return _vec2.x * _vec2.x + _vec2.y * _vec2.y;
	}

	/******************************************************************************/
	/*!
	\brief - Get two Vector2s' DotProduct
	\param _left - to be calculated
	\param _right - to be calculated
	\return x * _rhs.x + y * _rhs.y;
	*/
	/******************************************************************************/
	float DotProduct(CR_Vec2 _left, CR_Vec2 _right)
	{
		return _left.x * _right.x + _left.y * _right.y;
	}

	/******************************************************************************/
	/*!
	\brief - Get two Vector2s' CrossProduct
	\param _left - to be calculated
	\param _right - to be calculated
	\return x * x * _rhs.y - y * _rhs.x;
	*/
	/******************************************************************************/
	Vector2 CrossProduct(CR_Vec2 _left, CR_Vec2 _right)
	{
		return _left.x * _right.y - _left.y * _right.x;
	}

	/******************************************************************************/
	/*!
	\brief - Get normalized vector2
	\param _vec2
	*/
	/******************************************************************************/
	void Normalize(vec2& _vec2)
	{
		_vec2 = GetNormalize(_vec2);
	}

	Vector2 GetNormalize(CR_Vec2 _vec2)
	{
		Vector2 result(_vec2);

		// If this is not zero,
		if (!IsZero(_vec2))
			result = result / GetLength(_vec2);

		// Unless.
		else
			jeDebugPrint("!Vector2 - Cannot devide by 0.\n");

		return result;
	}

	void Absolute(vec2& _vec2)
	{
		_vec2 = GetAbsolute(_vec2);
	}

	Vector2 GetAbsolute(CR_Vec2 _vec2)
	{
		Vector2 result(_vec2);

		if (result.x < 0)
			result.x = -result.x;
		if (result.y < 0)
			result.y = -result.y;

		return result;
	}

	float GetAngle(CR_Vec2 _vec2)
	{
		return atan2(_vec2.y, _vec2.x);
	}

	bool IsZero(CR_Vec2 _vec2)
	{
		// If both x y are not 0,
		if (_vec2.x || _vec2.y)
			return false;

		// Unless,
		return true;
	}

	bool IsOne(CR_Vec2 _vec2)
	{
		// If both x y are not 1,
		if (_vec2.x != 1.f || _vec2.y != 1.f)
			return false;

		// Unless,
		return true;
	}
}

jeEnd