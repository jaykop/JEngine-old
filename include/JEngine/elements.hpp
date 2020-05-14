#pragma once
#include <macros.hpp>
#include <vector>
#include <string>
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
};

struct Face;
struct HalfEdgeVertex;

struct HalfEdge
{
	// ctor
	HalfEdge(void);

	// public members
	HalfEdgeVertex* vertex;
	Face* face;
	HalfEdge* next;
	HalfEdge* twin;

	// public methods
	HalfEdge* Prev(void) const;

}; // struct half_edge

struct Face
{
	Face(void) : edge(nullptr) {}
	HalfEdge* edge;

};

jeEnd