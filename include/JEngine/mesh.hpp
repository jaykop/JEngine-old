#pragma once
#include <macros.hpp>
#include <vec3.hpp>
#include <vec2.hpp>
#include <vector>

jeBegin

class Shader;

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
};

class Mesh {

	friend class Model;

public:

private:
	
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices, textures;

	/*  Functions  */
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
		const std::vector<unsigned>& textures);
	void draw(Shader* shader);

	/*  Render data  */
	unsigned int vao_, vbo_, ebo_;

	/*  Functions    */
	void setup_mesh();
};

jeEnd