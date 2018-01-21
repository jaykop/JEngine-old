#pragma once
#include <vector>
#include "System.h"
#include "Matrix4x4.h"

JE_BEGIN

class Shader;
class Material;
class Transform;
class Animation;

enum ProjectType { PERSPECTIVE, ORTHOGONAL };

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

public:

	int		GetWidth() const;
	int		GetHeight() const;
	void	SetMainCamera(Camera* _camera);
	Camera* GetMainCamera();

	CR_vec4	GetBackgroundColor() const;
	void	SetBackgroundColor(CR_vec4 _color);
	void	SetBackgroundColor(float _r, float _g, float _b, float _a);

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

	void LightSourcePipeline();
	void NormalPipeline(const float _dt);

	void SpritePipeline(Sprite * _sprite);
	void MappingPipeline(Sprite* _sprite);
	void LightingEffectPipeline(Material* _material);

	void ParticlePipeline(Emitter* _emitter, const float _dt);
	void EffectsPipeline(Sprite* _sprite);

	void Render(const unsigned _vao, const unsigned _vbo, const unsigned _ebo,
		const float _vertices[], const unsigned _indices[],
		const int _verticesSize, const int _indicesSize, const int _elementSize);
	void RenderParticle(const int _particleSize, float *_positionData, float *_colorData);

	void GLMousePosition();

	// Member variables
	Lights		m_lights;
	Sprites		m_sprites;
	Cameras		m_cameras;
	Camera*		m_pMainCamera;
	
	int		m_width, m_height;
	bool	m_orthoFirst, m_inside, m_isLight;
	mat4	m_perspective, m_orthogonal, m_viewport;
	vec4	m_backgroundColor;
	vec3	m_aniScale, m_aniTranslate;
	float	m_fovy, m_aspect, m_zNear, m_zFar;
	float	m_left, m_right, m_top, m_bottom;

	struct compareOrder {
		
		compareOrder(bool _orthoFirst) { m_orthoFirst = _orthoFirst; }
		bool operator()(Sprite * _leftSpt, Sprite * _rightSpt);
		
		private:
			bool m_orthoFirst;
	};

	GraphicSystem(const GraphicSystem& /*_copy*/) = delete;
	void operator=(const GraphicSystem& /*_copy*/) = delete;
};

JE_END
