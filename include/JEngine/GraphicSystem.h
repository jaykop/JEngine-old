#pragma once
#include <vector>
//#include <functional>
#include "System.h"
#include "Matrix4x4.h"

JE_BEGIN

class Shader;
class Material;
class Transform;
class Animation;

enum ProjectType  { PROJECTION_PERSPECTIVE, PROJECTION_ORTHOGONAL };

class GraphicSystem : public System
{

	friend class Light;
	friend class Model;
	friend class Sprite;
	friend class Camera;
	friend class Emitter;
	friend class SystemManager;

	using Lights = std::vector<Light*>;
	using Sprites = std::vector<Sprite*>;
	using Cameras = std::vector<Camera*>;

	enum Alias  {ALIAS_ALIASED, ALIAS_ANTIALIASED, ALIAS_MULTISAMPLE};

public:

	int		GetWidth() const;
	int		GetHeight() const;
	void	SetMainCamera(Camera* _camera);
	Camera* GetMainCamera();

	CR_vec4	GetBackgroundColor() const;
	void	SetBackgroundColor(CR_vec4 _color);
	void	SetBackgroundColor(float _r, float _g, float _b, float _a);

	bool	m_orthoComesFirst;
	Alias	m_aliasMode;

private:

	GraphicSystem();
	~GraphicSystem() {};

	void Load(CR_RJDoc _data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

	// Helper functions
	void AddSprite(Sprite* _sprite);
	void RemoveSprite(Sprite* _sprite);

	void AddCamera(Camera* _camera);
	void RemoveCamera(Camera* _camera);

	void AddLight(Light* _light);
	void RemoveLight(Light* _light);

	void UpdatePipelines(const float _dt);
	void LightSourcePipeline();
	void SpritePipeline(Sprite * _sprite);
	void MappingPipeline(Sprite* _sprite);
	void LightingEffectPipeline(Material* _material);

	void ParticlePipeline(Emitter* _emitter, const float _dt);
	void EffectsPipeline(Sprite* _sprite);

	void Render(const unsigned &_vao, const int _elementSize, unsigned _mode = 0x0004 /*GL_TRIANGLES*/);
	void SortSprites();
	void GLMousePosition();

	// Member variables
	Lights		m_lights;
	Sprites		m_sprites;
	Cameras		m_cameras;
	Camera*		m_pMainCamera;
	
	int		m_width, m_height;
	bool	m_inside, m_isLight;
	mat4	m_perspective, m_orthogonal, m_viewport;
	vec4	m_backgroundColor;
	vec3	m_aniScale, m_aniTranslate;
	float	m_fovy, m_aspect, m_zNear, m_zFar;
	float	m_left, m_right, m_top, m_bottom;

	//std::function<bool(Sprite*, Sprite*)> m_sorter;

	GraphicSystem(const GraphicSystem& /*_copy*/) = delete;
	void operator=(const GraphicSystem& /*_copy*/) = delete;
};

JE_END
