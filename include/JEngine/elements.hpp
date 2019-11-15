#pragma once
#include <macros.hpp>
#include <vector>
#include <String>
#include <vec3.hpp>
#include <vec2.hpp>

jeBegin

struct Texture {

	unsigned id;
	std::string type, path;
};

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
	vec3 tangent;
	vec3 bitangent;
};

jeEnd