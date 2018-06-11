#pragma once
#include "Macro.h"
#include <vector>
#include "Vector2.h"
#include "Vector3.h"

jeBegin

class Mesh
{
	std::vector<vec3>	Points;
	std::vector<vec2>	TextureCoordinates;
	std::vector<vec3>	Normals;
};

jeEnd