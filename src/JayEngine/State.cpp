#include "State.h"
#include "StateManager.h"
#include "SystemManager.h"
#include "ObjectContainer.h"

//TODO Remove later
#include "Sprite.h"
#include "Object.h"
#include "Camera.h"
#include "Transform.h"
#include "InputHandler.h"
#include "ObjectFactory.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "GraphicSystem.h"
#include "Light.h"
#include "Material.h"

JE_BEGIN

State::State(const char* _name)
	:m_pLastStage(nullptr)
{
	m_name.assign(_name);
}

void State::Load()
{
	JE_DEBUG_PRINT("Loading %s...\n", m_name.c_str());
	m_objContainer = new ObjectContainer; 
	SystemManager::Load();
}

void State::Init()
{
	JE_DEBUG_PRINT("Initializing %s...\n", m_name.c_str());
	SystemManager::Init();

	// TODO
	/************************* Test Code... **************************/
	ObjectFactory::CreateObject("camera");
	ObjectFactory::GetCreatedObject()->AddComponent<Camera>();
#ifdef JE_SUPPORT_3D
	ObjectFactory::GetCreatedObject()->GetComponent<Camera>()->m_position.Set(50, 50, 50);
#else
	ObjectFactory::GetCreatedObject()->GetComponent<Camera>()->m_position.Set(0, 0, 50);
#endif
	SystemManager::GetGraphicSystem()->SetMainCamera(
		ObjectFactory::GetCreatedObject()->GetComponent<Camera>());
	ObjectFactory::AddCreatedObject(m_objContainer);

	ObjectFactory::CreateObject("light");
	ObjectFactory::GetCreatedObject()->AddComponent<Light>();
	ObjectFactory::GetCreatedObject()->GetComponent<Light>()->m_ambient.Set(.1f, .1f, .1f, 1.f);
	ObjectFactory::GetCreatedObject()->GetComponent<Light>()->m_specular.Set(.5f, .5f, .5f, 1.f);
	ObjectFactory::GetCreatedObject()->GetComponent<Light>()->m_position.Set(0, 0, 1);
	ObjectFactory::AddCreatedObject(m_objContainer);

	ObjectFactory::CreateObject("test1");
	ObjectFactory::GetCreatedObject()->AddComponent<Transform>();
	ObjectFactory::GetCreatedObject()->AddComponent<Sprite>();
	ObjectFactory::GetCreatedObject()->AddComponent<Material>();
	ObjectFactory::GetCreatedObject()->GetComponent<Material>()->m_ambient.Set(.1f, .1f, .1f, 1.f);
	ObjectFactory::GetCreatedObject()->GetComponent<Material>()->m_specular.Set(.5f, .5f, .5f, 1.f);
	ObjectFactory::GetCreatedObject()->GetComponent<Transform>()->m_position.Set(0, 0, 2);
	ObjectFactory::GetCreatedObject()->GetComponent<Sprite>()->m_color.Set(1.f, 1.f, 1.f, 1.f);

#ifdef JE_SUPPORT_3D

	ObjectFactory::GetCreatedObject()->GetComponent<Transform>()->m_scale.Set(30.f, 30.f, 30.f);
	ObjectFactory::GetCreatedObject()->GetComponent<Sprite>()->AddTexture("uvtemplate");

#else

	ObjectFactory::GetCreatedObject()->GetComponent<Transform>()->m_scale.Set(30.f, 30.f, 1.f);
	ObjectFactory::GetCreatedObject()->GetComponent<Sprite>()->AddTexture("testTexture");
	

	//ObjectFactory::CreateObject("test2");
	//ObjectFactory::GetCreatedObject()->AddComponent<Transform>();
	//ObjectFactory::GetCreatedObject()->AddComponent<Sprite>();
	//ObjectFactory::GetCreatedObject()->AddComponent<Material>();
	//ObjectFactory::GetCreatedObject()->GetComponent<Material>()->m_ambient.Set(.1f, .1f, .1f, 1.f);
	//ObjectFactory::GetCreatedObject()->GetComponent<Material>()->m_specular.Set(.5f, .5f, .5f, 1.f);
	//ObjectFactory::GetCreatedObject()->GetComponent<Transform>()->m_position.Set(10, 10, 2);
	//ObjectFactory::GetCreatedObject()->GetComponent<Sprite>()->m_color.Set(1.f, 1.f, 1.f, 1.f);
	//ObjectFactory::GetCreatedObject()->GetComponent<Transform>()->m_scale.Set(30.f, 30.f, 1.f);
	//ObjectFactory::GetCreatedObject()->GetComponent<Sprite>()->AddTexture("testAnimation");
	//ObjectFactory::GetCreatedObject()->GetComponent<Sprite>()->ActiveAnimation(true);
	//ObjectFactory::GetCreatedObject()->GetComponent<Sprite>()->SetAnimationFrame(8);
	//ObjectFactory::GetCreatedObject()->GetComponent<Sprite>()->SetAnimationSpeed(10.f);
	//ObjectFactory::AddCreatedObject(m_objContainer);
#endif
	ObjectFactory::AddCreatedObject(m_objContainer);
	SystemManager::GetGraphicSystem()->SetBackgroundColor(.3f, .3f, .3f, .3f);
}

void State::Update(float _dt)
{
	//JE_DEBUG_PRINT("Updating %s...\n", m_name.c_str());
	
	Light* light = m_objContainer->GetObject("light")->GetComponent<Light>();

	SystemManager::Update(_dt);
	float move = _dt * 0.1f;

	if (InputHandler::KeyPressed(JE_W))
		light->m_position.z += move;

	if (InputHandler::KeyPressed(JE_S))
		light->m_position.z -= move;
	
	if (InputHandler::KeyPressed(JE_D))
		light->m_position.x += move;
	
	if (InputHandler::KeyPressed(JE_A))
		light->m_position.x -= move;

	//std::cout << light->m_position << std::endl;

	/*************************** Temp state test key ******************************/
	if (InputHandler::KeyTriggered(JE_1))
		StateManager::SetNextState("testState1");

	if (InputHandler::KeyTriggered(JE_2))
		StateManager::SetNextState("testState2");

	if (InputHandler::KeyTriggered(JE_3))
		StateManager::SetNextState("testState3");

	if (InputHandler::KeyTriggered(JE_4))
		StateManager::Pause("PauseState");

	if (InputHandler::KeyTriggered(JE_5))
		StateManager::Resume();

	if (InputHandler::KeyTriggered(JE_6))
		StateManager::ResumeAndNext("testState3");

	if (InputHandler::KeyTriggered(JE_MOUSE_LEFT)) 
		JE_DEBUG_PRINT("Left Mouse\n");

	if (InputHandler::KeyTriggered(JE_MOUSE_RIGHT))
		JE_DEBUG_PRINT("Right Mouse\n");

	if (InputHandler::KeyTriggered(JE_MOUSE_MIDDLE))
		JE_DEBUG_PRINT("Middle Mouse\n");

	if (InputHandler::KeyPressed(JE_MOUSE_WHEEL_DOWN))
		JE_DEBUG_PRINT("Mouse Wheel Down\n");

	if (InputHandler::KeyPressed(JE_MOUSE_WHEEL_UP))
		JE_DEBUG_PRINT("Mouse Wheel Up\n");

	if (InputHandler::KeyPressed(JE_ESC)) {
		JE_DEBUG_PRINT("Quit\n");
		StateManager::Quit();
	}
}

void State::Close()
{
	JE_DEBUG_PRINT("Closing %s...\n", m_name.c_str());
	SystemManager::Close();
}

void State::Unload()
{
	JE_DEBUG_PRINT("Unloading %s...\n", m_name.c_str());
	ClearObjectContainer();
	SystemManager::Unload();
}

void State::ClearObjectContainer()
{
	if (m_objContainer) {
		delete m_objContainer;
		m_objContainer = nullptr;
	}
}

JE_END

