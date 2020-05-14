#pragma once
#include <macros.hpp>
#include <elements.hpp>

jeBegin

class Shader;
struct Texture;

class Mesh {

	friend class Renderer;
	friend class ObjLoader;

public:

private:
	
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;

	unsigned int vao_, vbo_, ebo_;

	Mesh(const std::vector<Vertex>& vertices, 
		const std::vector<unsigned int>& indices,
		const std::vector<Texture>& textures);
	void draw(Shader* shader);
	void setup_mesh();

};

jeEnd