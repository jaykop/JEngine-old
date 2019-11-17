#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <mesh.hpp>
#include <string>

// assimp
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

jeBegin

unsigned texture_from_file(const char* path, const std::string& directory, bool gamma = false);

class Shader;
class Object;

class Model : public Component {

	jeBaseFriends(Model);
	friend class GraphicSystem;

public:

	void load_model(const std::string& path);

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

private:
	/*  Model Data */
	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Texture> textures_loaded;	
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	/*  Functions   */
	Model(Object* owner);
	virtual ~Model() {}

	void draw(Shader* shader);

	/*  Functions   */
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> load_materialTextures(aiMaterial* mat, aiTextureType type,
		const std::string& typeName);

	Model() = delete;

};

jeDeclareComponentBuilder(Model);

jeEnd