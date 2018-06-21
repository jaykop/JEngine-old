#include "MathUtils.h"

jeBegin

namespace Math
{
	bool IsZero(CR_Vec3 _vec3)
	{
		// If both x y z are not 0,
		if (_vec3.x || _vec3.y || _vec3.z)
			return false;

		// Unless,
		return true;
	}

	bool IsOne(CR_Vec3 _vec3)
	{
		// If both x y z are not 1,
		if (_vec3.x != 1.f || _vec3.y != 1.f || _vec3.z != 1.f)
			return false;

		// Unless,
		return true;
	}

	/******************************************************************************/
	/*!
	\brief - Get two vector3s' DotProduct
	\param _left - to be calculated
	\param _right - to be calculated
	\return x * _rhs.x + y * _rhs.y;
	*/
	/******************************************************************************/
	float DotProduct(CR_Vec3 _left, CR_Vec3 _right)
	{
		return _left.x * _right.x + _left.y * _right.y + _left.z * _right.z;
	}

	/******************************************************************************/
	/*!
	\brief - Get two vector3s' CrossProduct
	\param _left - to be calculated
	\param _right - to be calculated
	\return x * x * _rhs.y - y * _rhs.x;
	*/
	/******************************************************************************/
	Vector3 CrossProduct(CR_Vec3 _left, CR_Vec3 _right)
	{
		Vector3 result;

		result.x = _left.y * _right.z - _left.z * _right.y;
		result.y = _left.z * _right.x - _left.x * _right.z;
		result.z = _left.x * _right.y - _left.y * _right.x;

		return result;
	}

	/******************************************************************************/
	/*!
	\brief - Get normalized vector3
	\return result
	*/
	/******************************************************************************/
	void Normalize(vec3& _vec3)
	{
		_vec3 = GetNormalize(_vec3);
	}

	Vector3 GetNormalize(CR_Vec3 _vec3)
	{
		Vector3 result(_vec3);

		// If this is not zero,
		if (!IsZero(_vec3))
			result = result / GetLength(_vec3);

		// Unless.
		else
			jeDebugPrint("!Vector3 - Cannot devide by 0.\n");

		return result;
	}

	/******************************************************************************/
	/*!
	\brief - Get vector3's length
	\return sqrt(x*x + y*y)
	*/
	/******************************************************************************/
	float GetLength(CR_Vec3 _vec3)
	{
		return sqrt(GetLengthSq(_vec3));
	}

	float GetLengthSq(CR_Vec3 _vec3)
	{
		return _vec3.x * _vec3.x + _vec3.y * _vec3.y + _vec3.z * _vec3.z;
	}

	/******************************************************************************/
	/*!
	\brief - Calculate vector's absolute value
	\return result
	*/
	/******************************************************************************/
	void Absolute(vec3& _vec3)
	{
		_vec3 = GetAbsolute(_vec3);
	}

	Vector3 GetAbsolute(CR_Vec3 _vec3)
	{
		Vector3 result(_vec3.x, _vec3.y, _vec3.z);

		if (result.x < 0)
			result.x = -result.x;
		if (result.y < 0)
			result.y = -result.y;
		if (result.z < 0)
			result.z = -result.z;

		return result;
	}

	/******************************************************************************/
	/*!
	\brief - Calculate vector's rotated position; Regard vector as a point
	\param _angle - degree to rotate
	\param _pivot - pivot point
	\return point
	*/
	/******************************************************************************/
	void Rotate(vec3& _vec3, float _angle, CR_Vec3 _pivot)
	{
		_vec3 = GetRotated(_vec3, _angle, _pivot);
	}

	/******************************************************************************/
	/*!
	\brief - Get Rotated point around specific pivot point
	\param _vec3 - point to be rotated
	\param _angle - rotate degree
	\param _pivot - pivot point
	\return new_point
	*/
	/******************************************************************************/
	Vector3 GetRotated(CR_Vec3 _vec3, float _angle, CR_Vec3 _pivot)
	{
		vec3 new_point(_vec3);
		float radian = Math::DegToRad(_angle);

		float s = sinf(radian);
		float c = cosf(radian);

		new_point.x -= _pivot.x;
		new_point.y -= _pivot.y;

		float new_x = new_point.x * c - new_point.y * s;
		float new_y = new_point.x * s + new_point.y * c;

		new_point.x = new_x + _pivot.x;
		new_point.y = new_y + _pivot.y;

		return new_point;
	}


	Vector3	GetReflected(CR_Vec3 _vec3, CR_Vec3 _pivot)
	{
		vec3 result(_vec3);

		Vector3 norm = GetNormalize(_pivot);

		result = result - 2 * (DotProduct(result, norm)) * norm;

		return result;
	}

	/******************************************************************************/
	/*!
	\brief - Calculate vector's reflection
	\param _vec3 - vector to be reflected
	\param _pivot - pivot point
	\return reflected
	*/
	/******************************************************************************/
	void Reflection(vec3& _vec3, CR_Vec3 _pivot)
	{
		_vec3 = GetReflected(_vec3, _pivot);
	}

	/******************************************************************************/
	/*!
	\brief - Calculate 2 vectors' included angle
	\param _vec3 - 1st vector
	\param _pivot - 2nd vector
	\return Math::RadToDeg(radian)
	*/
	/******************************************************************************/
	float GetAngle(CR_Vec3 _vec3, CR_Vec3 _pivot)
	{
		float radian = atan2(_vec3.x * _pivot.y - _pivot.x * _vec3.y, DotProduct(_vec3, _pivot));

		return Math::RadToDeg(radian);
	}

	///******************************************************************************/
	///*!
	//\brief - Calculate the distance of point and segment.
	//\param _lineStart
	//\param _lineEnd
	//\return distance
	//*/
	///******************************************************************************/
	//float DistanceToLine(CR_Vec3 _vec3, CR_Vec3 _lineStart, CR_Vec3 _lineEnd)
	//{
	//	// segment is nit a segment; a point
	//	float length = GetLength(_lineStart - _lineEnd);
	//	if (!length)
	//		return  GetLength(_vec3 - _lineStart);

	//	// Unless...
	//	float projection = ((_vec3.x - _lineStart.x) * (_lineEnd.x - _lineStart.x) +
	//		(_vec3.y - _lineStart.y) * (_lineEnd.y - _lineStart.y)) / length;

	//	//
	//	//			1st case	2nd case	3rd case
	//	//			*			*			*
	//	//		    A						B
	//	//			=========================

	//	// 1st case
	//	if (projection < 0)
	//		return GetLength(_vec3 - _lineStart);
	//	// 3rd case
	//	else if (projection > length)
	//		return GetLength(_vec3 - _lineEnd);
	//	// 2nd case
	//	else return abs((_vec3.y - _lineStart.y) * (_lineEnd.x - _lineStart.x)
	//		- (_vec3.x - _lineStart.x) * (_lineEnd.y - _lineStart.y)) / length;
	//}

	///******************************************************************************/
	///*!
	//\brief - Get a intersection point by two lines
	//\param line1_start - 1st line's staring point(vector)
	//\param line1_end - 2nd line's staring point(vector)
	//\param line2_start - 1st line's ending point(vector)
	//\param line2_end - 2nd line's ending point(vector)
	//\return Vector3
	//*/
	///******************************************************************************/
	//Vector3  GetSegmentIntersection(
	//	CR_Vec3 line1_start, CR_Vec3 line1_end,
	//	CR_Vec3 line2_start, CR_Vec3 line2_end)
	//{
	//	//Get Coefficients
	//	float a2 = line2_end.y - line2_start.y;
	//	float b2 = line2_start.x - line2_end.x;
	//	float c2 = line2_end.x * line2_start.y - line2_start.x * line2_end.y;

	//	float a1 = line1_end.y - line1_start.y;
	//	float b1 = line1_start.x - line1_end.x;
	//	float c1 = line1_end.x * line1_start.y - line1_start.x * line1_end.y;

	//	//Check if they are parallel
	//	float D = a1 * b2 - a2 * b1;

	//	if (!D)
	//		jeDebugPrint("!Vector3 - Determine is 0.\n");

	//	return Vector3((b1*c2 - b2 * c1) / D, (a2*c1 - a1 * c2) / D);
	//}

	///******************************************************************************/
	///*!
	//\brief - Check if two line is intersected or not; for vectors.
	//\param line1_start - 1st line's staring point(vector)
	//\param line1_end - 2nd line's staring point(vector)
	//\param line2_start - 1st line's ending point(vector)
	//\param line2_end - 2nd line's ending point(vector)
	//\return bool
	//*/
	///******************************************************************************/
	//bool IsSegmentIntersection(
	//	CR_Vec3 line1_start, CR_Vec3 line1_end,
	//	CR_Vec3 line2_start, CR_Vec3 line2_end)
	//{
	//	float denominator = ((line1_end.x - line1_start.x) * (line2_end.y - line2_start.y)) - ((line1_end.y - line1_start.y) * (line2_end.x - line2_start.x));
	//	float numerator1 = ((line1_start.y - line2_start.y) * (line2_end.x - line2_start.x)) - ((line1_start.x - line2_start.x) * (line2_end.y - line2_start.y));
	//	float numerator2 = ((line1_start.y - line2_start.y) * (line1_end.x - line1_start.x)) - ((line1_start.x - line2_start.x) * (line1_end.y - line1_start.y));

	//	if (!denominator) return numerator1 == 0 && numerator2 == 0;
	//	float r = numerator1 / denominator;
	//	float s = numerator2 / denominator;

	//	return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
	//}
}

jeEnd