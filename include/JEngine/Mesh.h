#pragma once
#include "Macro.h"
#include <vector>
#include "glew.h"
#include "Vector2.h"
#include "Vector3.h"
#include <unordered_map>

jeBegin

class Mesh
{
	friend class Text;
	friend class Light;
	friend class Model;
	friend class Emitter;
	friend class GLManager;
	friend class AssetManager;
	friend class GraphicSystem;

	using TextureMap = std::unordered_map<std::string, unsigned>;

	struct jeIndex { unsigned a, b, c; };
	struct jeVertex {
		vec3 position;
		vec2 uv;
		vec3 normal;
	};

public:

	static Mesh* CreatePoint();
	static Mesh* CreateRect();
	static Mesh* CreateCrossRect();
	static Mesh* CreateCube();
	static Mesh* CreateTetrahedron();
	static Mesh* CreateWireframeBox();

	void		AddTexture(const char* key);
	void		RemoveTexture(const char* key);
	void		SetCurrentTexutre(const char* key);
	unsigned	GetCurrentTexutre() const;
	unsigned	GetTexutre(const char* key);

	void AddPoint(CR_Vec3 point);
	void AddTextureUV(CR_Vec2 uv);
	void AddNormal(CR_Vec3 normal);
	void AddIndice(jeIndex indice);

	vec3		GetPoint(unsigned index) const;
	vec2		GetUV(unsigned index) const;
	vec3		GetNormal(unsigned index) const;
	jeIndex		GetIndice(unsigned index) const;

	std::size_t GetPointCount() const;
	std::size_t GetIndiceCount() const;

	const std::vector<vec3>&	GetNormals() const;
	const std::vector<jeIndex>& GetIndices() const;

	void ClearPoints();
	void ClearNormals();
	void ClearUVs();
	void ClearIndices();

	void ClearVertexes();

private:

	unsigned	mainTexture_, drawMode_;;
	TextureMap	textureMap_;

	std::vector<jeIndex>	indices_;
	std::vector<vec3>		points_;
	std::vector<vec2>		textureUVs_;
	std::vector<vec3>		normals_;

	GLuint vao_, vbo_, ebo_;

	bool builtIn_;

	Mesh();
	~Mesh();
};

jeEnd