#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"

#include "Vector3.h"

JE_BEGIN

class CameraControllerBuilder : public ComponentBuilder
{

	friend class Core;

public:

private:

	CameraControllerBuilder();
	~CameraControllerBuilder() {};
	CameraControllerBuilder(const CameraControllerBuilder& _copy) = delete;
	void operator=(const CameraControllerBuilder& _copy) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class Camera;
class CameraController : public CustomComponent
{

	friend class ComponentManager;
	friend class CameraControllerBuilder;

public:

private:

	CameraController(Object* pObject);
	~CameraController() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override;

	Camera* m_camera;
	vec3 m_position, m_target;

	void EditorUpdate(const float _dt);
};

JE_END