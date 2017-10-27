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
	SYSTEM::Load();
}

void State::Init()
{
	JE_DEBUG_PRINT("Initializing %s...\n", m_name.c_str());
	SYSTEM::Init();

	// TODO
	/************************* Test Code... **************************/
	FACTORY::CreateObject("camera");
	FACTORY::GetCreatedObject()->AddComponent<Camera>();
#ifdef JE_SUPPORT_3D
	FACTORY::GetCreatedObject()->GetComponent<Camera>()->m_position.Set(0, 0, 50);
#else
	FACTORY::GetCreatedObject()->GetComponent<Camera>()->m_position.Set(0, 0, 50);
#endif
	SYSTEM::GetGraphicSystem()->SetMainCamera(
		FACTORY::GetCreatedObject()->GetComponent<Camera>());
	FACTORY::AddCreatedObject(m_objContainer);

	FACTORY::CreateObject("light");
	FACTORY::GetCreatedObject()->AddComponent<Light>();
	FACTORY::GetCreatedObject()->GetComponent<Light>()->m_ambient.Set(.2f, .2f, .2f, 1.f);
	FACTORY::GetCreatedObject()->GetComponent<Light>()->m_specular.Set(1.f, 1.f, 1.f, 1.f);
	FACTORY::GetCreatedObject()->GetComponent<Light>()->m_diffuse.Set(.5f, .5f, .5f, 1.f);
	FACTORY::GetCreatedObject()->GetComponent<Light>()->m_position.Set(0, 0, 25 );
	FACTORY::AddCreatedObject(m_objContainer);

	FACTORY::CreateObject("test1");
	FACTORY::GetCreatedObject()->AddComponent<Transform>();
	FACTORY::GetCreatedObject()->AddComponent<Sprite>();
	FACTORY::GetCreatedObject()->AddComponent<Material>();
	FACTORY::GetCreatedObject()->GetComponent<Material>()->m_diffuse = 0;
	FACTORY::GetCreatedObject()->GetComponent<Transform>()->m_position.Set(30, 0, -50);
	FACTORY::GetCreatedObject()->GetComponent<Sprite>()->m_color.Set(1.f, 1.f, 1.f, 1.f);
	FACTORY::GetCreatedObject()->GetComponent<Transform>()->m_scale.Set(30.f, 30.f, 30.f);
	FACTORY::GetCreatedObject()->GetComponent<Sprite>()->AddTexture("uvtemplate");
	FACTORY::AddCreatedObject(m_objContainer);

	FACTORY::CreateObject("test2");
	FACTORY::GetCreatedObject()->AddComponent<Transform>();
	FACTORY::GetCreatedObject()->AddComponent<Sprite>();
	FACTORY::GetCreatedObject()->AddComponent<Material>();
	FACTORY::GetCreatedObject()->GetComponent<Material>()->m_diffuse = 100;
	FACTORY::GetCreatedObject()->GetComponent<Transform>()->m_position.Set(-30, 0, -50);
	FACTORY::GetCreatedObject()->GetComponent<Sprite>()->m_color.Set(1.f, 1.f, 1.f, 1.f);
	FACTORY::GetCreatedObject()->GetComponent<Transform>()->m_scale.Set(30.f, 30.f, 30.f);
	FACTORY::GetCreatedObject()->GetComponent<Sprite>()->AddTexture("uvtemplate");
	FACTORY::AddCreatedObject(m_objContainer);

	SYSTEM::GetGraphicSystem()->SetBackgroundColor(.3f, .3f, .3f, .3f);
}

void State::Update(float _dt)
{
	//JE_DEBUG_PRINT("Updating %s...\n", m_name.c_str());
	
	Light* light = m_objContainer->GetObject("light")->GetComponent<Light>();

	SYSTEM::Update(_dt);
	float move = _dt * 0.5f;

	if (INPUT::KeyPressed(JE_W))
		light->m_position.z -= move;

	if (INPUT::KeyPressed(JE_S))
		light->m_position.z += move;
	
	if (INPUT::KeyPressed(JE_D))
		light->m_position.x += move;
	
	if (INPUT::KeyPressed(JE_A))
		light->m_position.x -= move;

	/*************************** Temp state test key ******************************/
	if (INPUT::KeyTriggered(JE_1))
		STATE::SetNextState("testState1");

	if (INPUT::KeyTriggered(JE_2))
		STATE::SetNextState("testState2");

	if (INPUT::KeyTriggered(JE_3))
		STATE::SetNextState("testState3");

	if (INPUT::KeyTriggered(JE_4))
		STATE::Pause("PauseState");

	if (INPUT::KeyTriggered(JE_5))
		STATE::Resume();

	if (INPUT::KeyTriggered(JE_6))
		STATE::ResumeAndNext("testState3");

	if (INPUT::KeyTriggered(JE_MOUSE_LEFT)) 
		JE_DEBUG_PRINT("Left Mouse\n");

	if (INPUT::KeyTriggered(JE_MOUSE_RIGHT))
		JE_DEBUG_PRINT("Right Mouse\n");

	if (INPUT::KeyTriggered(JE_MOUSE_MIDDLE))
		JE_DEBUG_PRINT("Middle Mouse\n");

	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_DOWN))
		JE_DEBUG_PRINT("Mouse Wheel Down\n");

	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_UP))
		JE_DEBUG_PRINT("Mouse Wheel Up\n");

	if (INPUT::KeyPressed(JE_ESC)) {
		JE_DEBUG_PRINT("Quit\n");
		STATE::Quit();
	}
}

void State::Close()
{
	JE_DEBUG_PRINT("Closing %s...\n", m_name.c_str());
	SYSTEM::Close();
}

void State::Unload()
{
	JE_DEBUG_PRINT("Unloading %s...\n", m_name.c_str());
	ClearObjectContainer();
	SYSTEM::Unload();
}

void State::ClearObjectContainer()
{
	if (m_objContainer) {
		delete m_objContainer;
		m_objContainer = nullptr;
	}
}

JE_END

