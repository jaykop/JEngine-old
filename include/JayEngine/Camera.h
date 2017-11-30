#pragma once
#include "Vector3.h"
#include "Component.h"
#include "ComponentBuilder.h"

JE_BEGIN

class CameraBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	CameraBuilder();
	~CameraBuilder() {};
	CameraBuilder(const CameraBuilder& /*_copy*/) {};
	void operator=(const CameraBuilder& /*_copy*/) {};

	Component* CreateComponent(Object* _pOwner) const override;

};


class Camera : public Component
{

	friend class GraphicSystem;
	friend class ComponentManager;
	friend class CameraBuilder;

public:

	vec3 m_position, m_up, m_target;

	void Register() override;

private:

	Camera(Object* _owner);
	~Camera() {};

	Camera() = delete;
	Camera(const Camera& /*_copy*/) = delete;
	void operator=(const Camera& /*_copy*/) = delete;

	void Load(CR_RJValue _data) override;

};

JE_END
