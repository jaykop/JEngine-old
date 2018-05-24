#pragma once
#include "Vector3.h"
#include "Component.h"
#include "ComponentBuilder.h"

jeBegin

class Camera : public Component
{
	template <class T>
	friend class MemoryAllocator;

	friend class ComponentManager;
	friend class GraphicSystem;
	friend class CameraBuilder;

public:

	vec3 m_position, m_up, m_target;

	void Register() override;

private:

	Camera(Object* _pOwner);
	~Camera() {};
	void operator=(const Camera& _copy);

	Camera() = delete;
	Camera(const Camera& /*_copy*/) = delete;
	
	void Load(CR_RJValue _data) override;

	void EditorUpdate(const float _dt) override;
};

jeDeclareComponentBuilder(Camera);
jeEnd
