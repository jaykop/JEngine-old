#pragma once
#include <component.hpp>
#include <mesh.hpp>

// assimp
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

jeBegin

unsigned texture_from_file(const char* path, const std::string& directory, bool gamma = false);

class Shader;
class Object;

class Model {

public:

private:
	/*  Model Data */
	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Texture> textures_loaded;	
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	/*  Functions   */
	Model(char* path) { load_model(path); }
	void draw(Shader* shader);

	/*  Functions   */
	void load_model(const std::string& path);
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> load_materialTextures(aiMaterial* mat, aiTextureType type,
		const std::string& typeName);

	Model(Object* owner);
	virtual ~Model() {};

	Model() = delete;

};

jeEnd