#include "MathUtils.h"

jeBegin

namespace Math
{
	bool IsZero(CR_Vec3 vec)
	{
		// If both x y z are not 0,
		if (vec.x || vec.y || vec.z)
			return false;

		// Unless,
		return true;
	}

	bool IsOne(CR_Vec3 vec)
	{
		// If both x y z are not 1,
		if (vec.x != 1.f || vec.y != 1.f || vec.z != 1.f)
			return false;

		// Unless,
		return true;
	}

	/******************************************************************************/
	/*!
	\brief - Get two vector3s' DotProduct
	\param left - to be calculated
	\param right - to be calculated
	\return x * rhs.x + y * rhs.y;
	*/
	/******************************************************************************/
	float DotProduct(CR_Vec3 left, CR_Vec3 right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}

	/******************************************************************************/
	/*!
	\brief - Get two vector3s' CrossProduct
	\param left - to be calculated
	\param right - to be calculated
	\return x * x * rhs.y - y * rhs.x;
	*/
	/******************************************************************************/
	Vector3 CrossProduct(CR_Vec3 left, CR_Vec3 right)
	{
		Vector3 result;

		result.x = left.y * right.z - left.z * right.y;
		result.y = left.z * right.x - left.x * right.z;
		result.z = left.x * right.y - left.y * right.x;

		return result;
	}

	/******************************************************************************/
	/*!
	\brief - Get normalized vector3
	\return result
	*/
	/******************************************************************************/
	void Normalize(vec3& vec)
	{
		vec = GetNormalize(vec);
	}

	Vector3 GetNormalize(CR_Vec3 vec)
	{
		Vector3 result(vec);

		// If this is not zero,
		if (!IsZero(vec))
			result = result / GetLength(vec);

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
	float GetLength(CR_Vec3 vec)
	{
		return sqrt(GetLengthSq(vec));
	}

	float GetLengthSq(CR_Vec3 vec)
	{
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	}

	/******************************************************************************/
	/*!
	\brief - Calculate vector's absolute value
	\return result
	*/
	/******************************************************************************/
	void Absolute(vec3& vec)
	{
		vec = GetAbsolute(vec);
	}

	Vector3 GetAbsolute(CR_Vec3 vec)
	{
		Vector3 result(vec.x, vec.y, vec.z);

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
	\param angle - degree to rotate
	\param pivot - pivot point
	\return point
	*/
	/******************************************************************************/
	void Rotate(vec3& vec, float angle, CR_Vec3 pivot)
	{
		vec = GetRotated(vec, angle, pivot);
	}

	/******************************************************************************/
	/*!
	\brief - Get Rotated point around specific pivot point
	\param vec - point to be rotated
	\param angle - rotate degree
	\param pivot - pivot point
	\return new_point
	*/
	/******************************************************************************/
	Vector3 GetRotated(CR_Vec3 vec, float angle, CR_Vec3 pivot)
	{
		vec3 new_point(vec);
		float radian = Math::DegToRad(angle);

		float s = sinf(radian);
		float c = cosf(radian);

		new_point.x -= pivot.x;
		new_point.y -= pivot.y;

		float new_x = new_point.x * c - new_point.y * s;
		float new_y = new_point.x * s + new_point.y * c;

		new_point.x = new_x + pivot.x;
		new_point.y = new_y + pivot.y;

		return new_point;
	}


	Vector3	GetReflected(CR_Vec3 vec, CR_Vec3 pivot)
	{
		vec3 result(vec);

		Vector3 norm = GetNormalize(pivot);

		result = result - 2 * (DotProduct(result, norm)) * norm;

		return result;
	}

	/******************************************************************************/
	/*!
	\brief - Calculate vector's reflection
	\param vec - vector to be reflected
	\param pivot - pivot point
	\return reflected
	*/
	/******************************************************************************/
	void Reflection(vec3& vec, CR_Vec3 pivot)
	{
		vec = GetReflected(vec, pivot);
	}

	/******************************************************************************/
	/*!
	\brief - Calculate 2 vectors' included angle
	\param vec - 1st vector
	\param pivot - 2nd vector
	\return Math::RadToDeg(radian)
	*/
	/******************************************************************************/
	float GetAngle(CR_Vec3 vec, CR_Vec3 pivot)
	{
		float radian = atan2(vec.x * pivot.y - pivot.x * vec.y, DotProduct(vec, pivot));

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
	//float DistanceToLine(CR_Vec3 vec, CR_Vec3 _lineStart, CR_Vec3 _lineEnd)
	//{
	//	// segment is nit a segment; a point
	//	float length = GetLength(_lineStart - _lineEnd);
	//	if (!length)
	//		return  GetLength(vec - _lineStart);

	//	// Unless...
	//	float projection = ((vec.x - _lineStart.x) * (_lineEnd.x - _lineStart.x) +
	//		(vec.y - _lineStart.y) * (_lineEnd.y - _lineStart.y)) / length;

	//	//
	//	//			1st case	2nd case	3rd case
	//	//			*			*			*
	//	//		    A						B
	//	//			=========================

	//	// 1st case
	//	if (projection < 0)
	//		return GetLength(vec - _lineStart);
	//	// 3rd case
	//	else if (projection > length)
	//		return GetLength(vec - _lineEnd);
	//	// 2nd case
	//	else return abs((vec.y - _lineStart.y) * (_lineEnd.x - _lineStart.x)
	//		- (vec.x - _lineStart.x) * (_lineEnd.y - _lineStart.y)) / length;
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