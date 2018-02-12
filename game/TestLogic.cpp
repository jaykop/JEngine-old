#include <thread>
#include "TestLogic.h"
#include "GameLogicHeader.h"

JE_BEGIN

TestLogic::TestLogic(Object* _pObject)
	:UserComponent(_pObject)
{}

void TestLogic::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void TestLogic::Load(CR_RJValue /*_data*/)
{}

void TestLogic::Init()
{

}

void TestLogic::Update(const float /*_dt*/)
{

}

void TestLogic::Close()
{}

void TestLogic::Unload()
{}

TestLogicBuilder::TestLogicBuilder()
	:ComponentBuilder()
{}

UserComponent* TestLogicBuilder::CreateComponent(Object* _pOwner) const
{
	return new TestLogic(_pOwner);
}

JE_END