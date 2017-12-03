#pragma once
#include <vector>
#include "System.h"
#include "Matrix4x4.h"

JE_BEGIN

class Material;
class Transform;
class Animation;

class GraphicSystem : public System
{

	friend class Light;
	friend class Model;
	friend class Sprite;
	friend class Camera;
	friend class SystemManager;

	typedef std::vector<Light*> Lights;
	typedef std::vector<Sprite*> Sprites;
	typedef std::vector<Camera*> Cameras;

public:

	void	SetMainCamera(Camera* _camera);
	Camera* GetMainCamera();

	const vec4& GetBackgroundColor() const;
	void		SetBackgroundColor(const vec4& _color);
	void		SetBackgroundColor(float _r, float _g, float _b, float _a);

private:

	GraphicSystem();
	~GraphicSystem() {};
	GraphicSystem(const GraphicSystem& _copy) = delete;
	void operator=(const GraphicSystem& _copy) = delete;

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

	void LightingPipeline();
	void LightPipeline();

	void SpritePipeline();
	void TransformPipeline(Sprite* _sprite);
	void AnimationPipeline(Sprite* _sprite);
	void MappingPipeline(Sprite* _sprite, Material* _material);

	void EffectsPipeline(Sprite* _sprite);

	void GLMousePosition();

	// Member variables
	Lights		m_lights;
	Sprites		m_sprites;
	Cameras		m_cameras;
	Camera*		m_pMainCamera;
	Transform*	m_pTransformStorage;
	
	int		m_width, m_height;
	bool	m_orthoFirst, m_inside, m_isLight, m_hasAnimation;
	mat4	m_animation, m_perspective, m_orthogonal, m_viewport;
	vec4	m_backgroundColor;
	vec3	m_aniScale, m_aniTranslate;
	float	m_fovy, m_aspect, m_zNear, m_zFar;
	float	m_left, m_right, m_top, m_bottom;

#ifndef JE_SUPPORT_3D
	vec3 m_target2D;
#endif

	struct compareOrder {
		
		compareOrder(bool _orthoFirst) { m_orthoFirst = _orthoFirst; }
		bool operator()(Sprite * _leftSpt, Sprite * _rightSpt);
		
		private:
			bool m_orthoFirst;
	};

};

JE_END
