#pragma once
#include "Macro.h"

JE_BEGIN

template <typename T>
class Rect
{
public:

	Rect();
	~Rect() {};

	T m_left = 0, m_top = 0, m_width = 0, m_height = 0;
};

class Glyph 
{
	using iRect = Rect<int>;
	using fRect = Rect<float>;

public:
	
	Glyph() : advance(0) {}
	~Glyph() {};

	float	advance = 0;
	iRect	m_iRect;
	fRect	m_fRect;
	
};

JE_END