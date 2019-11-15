#pragma once
#include <macros.hpp>
#include <elements.hpp>

jeBegin

class Shader;
struct Texture;

class Mesh {

	friend class Model;

public:

private:
	
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;

	/*  Functions  */
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
		const std::vector<Texture>& textures);
	void draw(Shader* shader);

	/*  Render data  */
	unsigned int vao_, vbo_, ebo_;

	/*  Functions    */
	void setup_mesh();
};

jeEnd