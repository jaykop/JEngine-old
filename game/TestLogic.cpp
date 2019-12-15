#include "TestLogic.h"
#include "CustomLogicHeader.h"
#include "Application.h"

jeBegin
jeDefineCustomComponentBuilder(TestLogic);

int TestLogic::a = 0;

TestLogic::TestLogic(Object* pObject)
    :CustomComponent(pObject)
{}

void TestLogic::Register()
{
    SYSTEM::pBehavior_->AddBehavior(this);
}

void TestLogic::Load(CR_RJValue /*_data*/)
{}

void TestLogic::Init()
{}

void TestLogic::Update(float dt)
{
	dt;

	// it's on defaultly
	if (INPUT::KeyPressed(JE_SPACE)) {
		static bool on = false;
		APP::ActivateVSync(on);
		on = !on;
	}
}

void TestLogic::Close()
{}

void TestLogic::Unload()
{}

void TestLogic::EditorUpdate(const float /*dt*/)
{
    // TODO
}

jeEnd
