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

JE_BEGIN

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
}

JE_END
