#pragma once
#include "Vector3.h"
#include "Component.h"

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

public:

	vec3 m_position, m_up, m_target;

private:

	Camera(Object* _owner = nullptr);
	~Camera() {};

	Camera(const Camera& /*_copy*/) {};
	void operator=(const Camera& /*_copy*/) {};

};

JE_END
