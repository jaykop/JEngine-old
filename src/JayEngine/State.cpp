#include "State.h"
#include "StateManager.h"
#include "SystemManager.h"

//TODO Remove later
#include "InputHandler.h"
#include "Object.h"
#include "Transform.h"
#include "Sprite.h"
#include "ObjectManager.h"

NS_JE_BEGIN

State::State(const char* _name)
	:m_pLastStage(nullptr)
{
	m_name.assign(_name);
}

void State::Load()
{
	SystemManager::Load();
	JE_DEBUG_PRINT("Loading %s...\n", m_name.c_str());
}

void State::Init()
{
	SystemManager::Init();
	JE_DEBUG_PRINT("Initializing %s...\n", m_name.c_str());

	ObjectManager::CreateObject("test");
	ObjectManager::GetCreatedObject()->GetComponentMap()->AddComponent<Transform>();
	ObjectManager::GetCreatedObject()->GetComponentMap()->AddComponent<Sprite>();
	ObjectManager::AddCreatedObject();

	ObjectManager::GetObject("test")->GetComponentMap()->GetComponent<Transform>()->SetScale(vec3(10.f, 10.f, 0.f));
	ObjectManager::GetObject("test")->GetComponentMap()->GetComponent<Sprite>()->SetColor(vec4(1.f, 0.f, 0.f, 1.f));
}

void State::Update(float _dt)
{
	SystemManager::Update(_dt);
	JE_UNUSED_PARAM(_dt);

	//JE_DEBUG_PRINT("Updating %s...\n", m_name.c_str());

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
	SystemManager::Close();
	JE_DEBUG_PRINT("Closing %s...\n", m_name.c_str());
}

void State::Unload()
{
	SystemManager::Unload();
	JE_DEBUG_PRINT("Unloading %s...\n", m_name.c_str());
}

NS_JE_END

