#pragma once
#include <vector>
#include "System.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#ifndef JE_SUPPORT_3D
#include "Vector3.h"
#endif

JE_BEGIN

class Transform;

class GraphicSystem : public System
{

	friend class Light;
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
	GraphicSystem(const GraphicSystem& /*_copy*/) {};
	void operator=(const GraphicSystem& /*_copy*/) {};

	void Load() override;
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

	void Pipeline(Light* _light);
	void Pipeline(Sprite* _sprite);
	void TransformPipeline(Sprite* _sprite);
	void MappingPipeline(Sprite* _sprite);
	void AnimationPipeline(Sprite* _sprite);

	void GLMousePosition();

	// Member variables
	Lights		m_lights;
	Sprites		m_sprites;
	Cameras		m_cameras;
	Camera*		m_pMainCamera;
	Transform*	m_pTransformStorage;
	
	int		m_width, m_height;
	bool	m_orthoFirst, m_inside, m_isLight;
	mat4	m_animation, m_perspective, m_orthogonal, m_viewport;
	vec4	m_backgroundColor;
	float	m_fovy, m_aspect, m_zNear, m_zFar;
	float	m_left, m_right, m_top, m_bottom;

	struct compareOrder {
		
		compareOrder(bool _orthoFirst) { m_orthoFirst = _orthoFirst; }
		bool operator()(Sprite * _leftSpt, Sprite * _rightSpt);
		
		private:
			bool m_orthoFirst;
	};

#ifdef JE_SUPPORT_3D
	friend class Model;
	typedef std::vector<Model*> Models;
	Models		m_models;

	void AddModel(Model* _model);
	void RemoveModel(Model* _model);
#else
	vec3 m_target2D;
#endif
};

JE_END
