#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "ComponentManager.h"

#include "Vector3.h"

jeBegin

struct Telegram;
class Camera;

class CameraController : public CustomComponent
{
	template <class T>
	friend class MemoryAllocator;

	friend class ComponentManager;
	friend class CameraControllerBuilder;

public:

private:

	CameraController(Object* pObject);
	~CameraController() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;
	bool OnMessage(Telegram& msg) override { msg; return false; }

	Camera* m_camera;

	void EditorUpdate(float dt) override;
};

jeDeclareCustomComponentBuilder(CameraController);
jeEnd
