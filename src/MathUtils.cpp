/******************************************************************************/
/*!
\file   MathUtils.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/06/19(yy/mm/dd)

\description
Contains MathUtils's class and member function

*/
/******************************************************************************/

#include "MathUtils.h"
#include <cmath>
#include <iomanip>

NS_JE_BEGIN

namespace Math
{
	/******************************************************************************/
	/*!
	\brief - Convert degree to radian 
	\param degree - to be converted
	\return result
	*/
	/******************************************************************************/
	float DegToRad(float degree)
	{
		return degree * static_cast<float>(RADIAN);
	}

	/******************************************************************************/
	/*!
	\brief - Convert radian to degree
	\param radian - to be converted
	\return result
	*/
	/******************************************************************************/
	float RadToDeg(float radian)
	{
		return radian * static_cast<float>(RADIAN_DEGREE);
	}

	/******************************************************************************/
	/*!
	\brief - Get Rotated point around specific pivot point
	\param point - point to be rotated
	\param angle - rotate degree
	\param pivot - pivot point
	\return new_point
	*/
	/******************************************************************************/
	Pointf Rotation(const Pointf& point, float angle, const Pointf& pivot)
	{
		Pointf new_point(point);

		float s = sinf(Math::DegToRad(angle));
		float c = cosf(Math::DegToRad(angle));

		new_point.x -= pivot.x;
		new_point.y -= pivot.y;

		float new_x = point.x * c - point.y * s;
		float new_y = point.x * s + point.y * c;

		new_point.x = new_x + pivot.x;
		new_point.y = new_y + pivot.y;

		return new_point;
	}

	/******************************************************************************/
	/*!
	\brief - Get Rotated point around specific pivot point
	\param point - point to be rotated
	\param angle - rotate degree
	\param pivot - pivot point
	\return new_point
	*/
	/******************************************************************************/
	vec3 Rotation(const vec3& point, float angle, const vec3& pivot)
	{
		vec3 new_point(point);

		float s = sinf(Math::DegToRad(angle));
		float c = cosf(Math::DegToRad(angle));

		new_point.x -= pivot.x;
		new_point.y -= pivot.y;

		float new_x = new_point.x * c - new_point.y * s;
		float new_y = new_point.x * s + new_point.y * c;

		new_point.x = new_x + pivot.x;
		new_point.y = new_y + pivot.y;

		return new_point;
	}

	/******************************************************************************/
	/*!
	\brief - Get a intersection point by two lines
	\param line1 - 1st line
	\param line2 - 2nd line
	\return inter_point
	*/
	/******************************************************************************/
	Pointf  Intersect_ln(const Linef& line1, const Linef& line2)
	{
		//Get Coefficients
		float a2 = line2.p2.y - line2.p1.y;
		float b2 = line2.p1.x - line2.p2.x;
		float c2 = line2.p2.x * line2.p1.y - line2.p1.x * line2.p2.y;

		float a1 = line1.p2.y - line1.p1.y;
		float b1 = line1.p1.x - line1.p2.x;
		float c1 = line1.p2.x * line1.p1.y - line1.p1.x * line1.p2.y;

		//Check if they are parallel
		float D = a1 * b2 - a2 * b1;
		try {
			if (!D)
				throw D;
		}
		catch (float exception) {
			std::cout << exception << "is invalid.\nCannot calculate properly...\n";
		}

		Pointf inter_point{ (b1*c2 - b2*c1) / D, (a2*c1 - a1*c2) / D };

		return inter_point;
	}

	/******************************************************************************/
	/*!
	\brief - Check if two line is intersected or not; for lines.
	\param line1 - 1st line
	\param line2 - 2nd line
	\return bool
	*/
	/******************************************************************************/
	bool Intersect_Bool(const Linef& line1, const Linef& line2)
	{
		float denominator = ((line1.p2.x - line1.p1.x) * (line2.p2.y - line2.p1.y)) - ((line1.p2.y - line1.p1.y) * (line2.p2.x - line2.p1.x));
		float numerator1 = ((line1.p1.y - line2.p1.y) * (line2.p2.x - line2.p1.x)) - ((line1.p1.x - line2.p1.x) * (line2.p2.y - line2.p1.y));
		float numerator2 = ((line1.p1.y - line2.p1.y) * (line1.p2.x - line1.p1.x)) - ((line1.p1.x - line2.p1.x) * (line1.p2.y - line1.p1.y));

		if (!denominator) return numerator1 == 0 && numerator2 == 0;
		float r = numerator1 / denominator;
		float s = numerator2 / denominator;

		return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);

	}

	/******************************************************************************/
	/*!
	\brief - Get a intersection point by two lines
	\param line1 - 1st line's staring point(vector)
	\param line2 - 2nd line's staring point(vector)
	\param line1 - 1st line's ending point(vector)
	\param line2 - 2nd line's ending point(vector)
	\return vec3
	*/
	/******************************************************************************/
	vec3  Intersect_ln(
		const vec3& line1_start, const vec3& line1_end,
		const vec3& line2_start, const vec3& line2_end)
	{
		//Get Coefficients
		float a2 = line2_end.y - line2_start.y;
		float b2 = line2_start.x - line2_end.x;
		float c2 = line2_end.x * line2_start.y - line2_start.x * line2_end.y;

		float a1 = line1_end.y - line1_start.y;
		float b1 = line1_start.x - line1_end.x;
		float c1 = line1_end.x * line1_start.y - line1_start.x * line1_end.y;

		//Check if they are parallel
		float D = a1 * b2 - a2 * b1;
		try {
			if (!D)
				throw D;
		}
		catch (float exception) {
			std::cout << exception << "is invalid.\nCannot calculate properly...\n";
		}

		return vec3((b1*c2 - b2*c1) / D, (a2*c1 - a1*c2) / D);
	}

	/******************************************************************************/
	/*!
	\brief - Check if two line is intersected or not; for vectors.
	\param line1 - 1st line's staring point(vector)
	\param line2 - 2nd line's staring point(vector)
	\param line1 - 1st line's ending point(vector)
	\param line2 - 2nd line's ending point(vector)
	\return bool
	*/
	/******************************************************************************/
	bool Intersect_Bool(
		const vec3& line1_start, const vec3& line1_end,
		const vec3& line2_start, const vec3& line2_end)
	{
		//float s1_x, s1_y, s2_x, s2_y;
		//s1_x = line1_end.x - line1_start.x;     s1_y = line1_end.y - line1_start.y;
		//s2_x = line2_end.x - line2_start.x;     s2_y = line2_end.y - line2_start.y;

		//float s, t;
		//s = (-s1_y * (line1_start.x - line2_start.x) + s1_x * (line1_start.y - line2_start.y)) / (-s2_x * s1_y + s1_x * s2_y);
		//t = (s2_x * (line1_start.y - line2_start.y) - s2_y * (line1_start.x - line2_start.x)) / (-s2_x * s1_y + s1_x * s2_y);

		//if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		//	return true;
		//

		//return false;

		float denominator = ((line1_end.x - line1_start.x) * (line2_end.y - line2_start.y)) - ((line1_end.y - line1_start.y) * (line2_end.x - line2_start.x));
		float numerator1 = ((line1_start.y - line2_start.y) * (line2_end.x - line2_start.x)) - ((line1_start.x - line2_start.x) * (line2_end.y - line2_start.y));
		float numerator2 = ((line1_start.y - line2_start.y) * (line1_end.x - line1_start.x)) - ((line1_start.x - line2_start.x) * (line1_end.y - line1_start.y));

		if (!denominator) return numerator1 == 0 && numerator2 == 0;
		float r = numerator1 / denominator;
		float s = numerator2 / denominator;

		return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
	}

	/******************************************************************************/
	/*!
	\brief - Calculate the distance of two vector(point).
	\param a - 1st point(vector)
	\param b - 2nd point(vector)
	\return distance
	*/
	/******************************************************************************/
	float Distance_pt(const vec3& a, const vec3& b)
	{
		float x_sqr = (b.x - a.x);
		float y_sqr = (b.y - a.y);

		return sqrt(x_sqr * x_sqr + y_sqr * y_sqr);
	}

	/******************************************************************************/
	/*!
	\brief - Calculate the distance of two point.
	\param a - 1st point
	\param b - 2nd point
	\return distance
	*/
	/******************************************************************************/
	float Distance_pt(const Pointf& a, const Pointf& b)
	{
		vec3 square((b.x - a.x) * (b.x - a.x), (b.y - a.y) * (b.y - a.y), 0);
		return sqrt(square.x + square.y);
	}

	/******************************************************************************/
	/*!
	\brief - Calculate the degree of two point.
	\param a - 1st point
	\param b - 2nd point
	\return distance
	*/
	/******************************************************************************/
	float Degree_pt(const vec3 & a, const vec3 & b)
	{
		float result = atan2(b.y - a.y, b.x - a.x) * RADIAN_DEGREE;;
		return result < 0 ? 360 + result : result;
	}

	/******************************************************************************/
	/*!
	\brief - Calculate the degree of two point.
	\param a - 1st point
	\param b - 2nd point
	\return distance
	*/
	/******************************************************************************/
	float Degree_pt(const Pointf & a, const Pointf & b)
	{
		return atan2(b.y - a.y, b.x - a.x) * RADIAN_DEGREE;
	}

	/******************************************************************************/
	/*!
	\brief - Calculate the distance of point and segment.
	\param point
	\param line_start
	\param line_end
	\return distance
	*/
	/******************************************************************************/
	float Distance_st(const vec3& point, const vec3& line_start, const vec3& line_end)
	{
		// segment is nit a segment; a point
		float length = Distance_pt(line_start, line_end);
		if (!length)
			return Distance_pt(line_start, point);

		// Unless...
		float projection = ((point.x - line_start.x) * (line_end.x - line_start.x) + 
			(point.y - line_start.y) * (line_end.y - line_start.y)) / length;

		//
		//	1st case		2nd case		3rd case
		//		*				*				*
		//		   A						 B
		//			=========================
		
		// 1st case
		if (projection < 0) return Distance_pt(line_start, point);
		// 3rd case
		else if (projection > length) return Distance_pt(line_end, point);
		// 2nd case
		else return abs((point.y - line_start.y) * (line_end.x - line_start.x)
			- (point.x - line_start.x) * (line_end.y - line_start.y)) / length;
	}
	
	/******************************************************************************/
	/*!
	\brief - Calculate the distance of point and segment.
	\param point
	\param line
	\return distance
	*/
	/******************************************************************************/
	float Distance_st(const Pointf& point, const Linef& line)
	{
		// segment is nit a segment; a point
		float length = Distance_pt(line.p1, line.p2);
		if (!length)
			return Distance_pt(line.p1, point);

		// Unless...
		float projection = ((point.x - line.p1.x) * (line.p2.x - line.p1.x) +
			(point.y - line.p1.y) * (line.p2.y - line.p1.y)) / length;

		//
		//	1st case		2nd case		3rd case
		//		*				*				*
		//		   A						 B
		//			=========================

		// 1st case
		if (projection < 0) return Distance_pt(line.p1, point);
		// 3rd case
		else if (projection > length) return Distance_pt(line.p2, point);
		// 2nd case
		else return abs((point.x - line.p1.x) * (line.p2.y - line.p1.y) +
			(point.y - line.p1.y) * (line.p2.x - line.p1.x)) / length;
	}
}

NS_JE_END