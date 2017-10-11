#pragma once
#include <vector>
#include <unordered_map>
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

	typedef std::vector<Sprite*>						Sprites;
	typedef std::unordered_map<std::string, Camera*>	CameraMap;
	
	enum GraphicMode { MODE_2D, MODE_3D };

public:

	void	AddCamera(const char* _camaraName);
	void	RemoveCamera(const char* _camaraName);
	void	ClearCameraMap();
	Camera* GetCamera(const char* _camaraName);
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

	void Pipeline(Sprite* _sprite);

	// Member variables
	Sprites		m_sprites; 
	Transform*	m_pTransformStorage;
	
	mat4	m_animation;
	vec4	m_colorStorage, m_backgroundColor;
	float	m_fovy, m_aspect, m_zNear, m_zFar;
	float	m_left, m_right, m_top, m_bottom;

	Camera*		m_pMainCamera;
	CameraMap	m_cameraMap;

	GraphicMode m_mode;
};

NS_JE_END