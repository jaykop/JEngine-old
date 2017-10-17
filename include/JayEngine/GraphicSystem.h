#pragma once
#include <vector>
#include "System.h"
#include "Vector4.h"
#include "Matrix4x4.h"

NS_JE_BEGIN

class Camera;
class Sprite;
class Texture;
class Transform;

class GraphicSystem : public System
{
	friend class Sprite;
	friend class SystemManager;

	typedef std::vector<Sprite*> Sprites;
	typedef std::vector<Camera*> Cameras;
	
	enum GraphicMode { MODE_2D, MODE_3D };

public:

	void	SetMainCamera(Camera* _camera);
	Camera* GetMainCamera();

	void		SetBackgroundColor(const vec4& _color);
	const vec4& GetBackgroundColor() const;

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
	void InitCamera();

	void AddSprite(Sprite* _sprite);
	void RemoveSprite(Sprite* _sprite);

	void AddCamera(Camera* _camera);
	void RemoveCamera(Camera* _camera);

	void Pipeline(Sprite* _sprite);
	void TransformPipeline(Sprite* _sprite);
	void MappingPipeline(Sprite* _sprite);
	void AnimationPipeline(Sprite* _sprite);

	void GLMousePosition();

	// Member variables
	GraphicMode m_mode;
	Sprites		m_sprites;
	Cameras		m_cameras;
	Camera*		m_pMainCamera;
	Transform*	m_pTransformStorage;
	
	int		m_width, m_height;
	bool	m_orthoFirst, m_inside;
	mat4	m_animation, m_perspective, m_orthogonal, m_viewport;
	vec4	m_colorStorage, m_backgroundColor;
	float	m_fovy, m_aspect, m_zNear, m_zFar;
	float	m_left, m_right, m_top, m_bottom;

	struct compareOrder {
		
		compareOrder(bool _orthoFirst) { m_orthoFirst = _orthoFirst; }
		bool operator()(Sprite * _leftSpt, Sprite * _rightSpt);
		
		private:
			bool m_orthoFirst;
	};
};

NS_JE_END