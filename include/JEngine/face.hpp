#pragma once
#include <macros.hpp>

jeBegin

class HalfEdge;
struct Face
{
	Face(void) : edge(nullptr) {}
	HalfEdge* edge;
};

jeEnd