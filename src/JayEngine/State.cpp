#include "State.h"
#include "StateManager.h"
#include "SystemManager.h"

//TODO Remove later
#include "Sprite.h"
#include "Object.h"
#include "Transform.h"
#include "InputHandler.h"
#include "ObjectManager.h"
#include "Matrix4x4.h"
#include "GraphicSystem.h"


NS_JE_BEGIN

State::State(const char* _name)
	:m_pLastStage(nullptr)
{
	m_name.assign(_name);
}

void State::Load()
{
	JE_DEBUG_PRINT("Loading %s...\n", m_name.c_str());
	SystemManager::Load();
}

void State::Init()
{
	JE_DEBUG_PRINT("Initializing %s...\n", m_name.c_str());
	SystemManager::Init();

	ObjectManager::CreateObject("test");
	ObjectManager::GetCreatedObject()->AddComponent<Transform>();
	ObjectManager::GetCreatedObject()->AddComponent<Sprite>();
	ObjectManager::AddCreatedObject();

	ObjectManager::GetObject("test")->GetComponent<Transform>()->m_position.Set(10, 10, 0);
	ObjectManager::GetObject("test")->GetComponent<Transform>()->m_rotation = 0;
	ObjectManager::GetObject("test")->GetComponent<Transform>()->m_scale.Set(30.f, 30.f, 0.f);
	ObjectManager::GetObject("test")->GetComponent<Sprite>()->m_color.Set(1.f, 1.f, 0.f, 1.f);
	ObjectManager::GetObject("test")->GetComponent<Sprite>()->AddTexture("testTexture");

	ObjectManager::CreateObject("test2");
	ObjectManager::GetCreatedObject()->AddComponent<Transform>();
	ObjectManager::GetCreatedObject()->AddComponent<Sprite>();
	ObjectManager::GetCreatedObject()->GetComponent<Transform>()->m_position.Set(-10, -10, 0);
	ObjectManager::GetCreatedObject()->GetComponent<Transform>()->m_rotation = 0;
	ObjectManager::GetCreatedObject()->GetComponent<Transform>()->m_scale.Set(30.f, 30.f, 0.f);
	ObjectManager::GetCreatedObject()->GetComponent<Sprite>()->m_color.Set(1.f, 1.f, 0.f, 1.f);
	ObjectManager::GetCreatedObject()->GetComponent<Sprite>()->AddTexture("testAnimation");
	ObjectManager::AddCreatedObject();

}

void State::Update(float _dt)
{
	//JE_DEBUG_PRINT("Updating %s...\n", m_name.c_str());
	SystemManager::Update(_dt);

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

	/*************************** Temp state test key ******************************/

}

void State::Close()
{
	JE_DEBUG_PRINT("Closing %s...\n", m_name.c_str());
	ObjectManager::ClearObjectContainer();
	SystemManager::Close();
}

void State::Unload()
{
	SystemManager::Unload();
	JE_DEBUG_PRINT("Unloading %s...\n", m_name.c_str());
}

NS_JE_END

