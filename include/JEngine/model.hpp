#pragma once
#include <component.hpp>
#include <vector>
#include <string>

jeBegin

class Mesh;
class Shader;
class Object;

class Model : protected Component {

public:

private:

	/*  Functions   */
	Model(char* path) { load_model(path); }
	void draw(Shader* shader);

	/*  Model Data  */
	std::vector<Mesh*> meshes;
	std::string directory;
	/*  Functions   */
	void load_model(const std::string& path);
	void process_node(aiNode* node, const aiScene* scene);
	Mesh* process_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<unsigned> load_materialTextures(aiMaterial* mat, aiTextureType type,
		const std::string& typeName);
	unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma);

	Model(Object* owner);
	virtual ~Model() {};

	Model() = delete;

};

jeEnd