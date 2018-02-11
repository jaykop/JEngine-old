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
	vec3 a(1, 0, 0);
	vec3 b(0, 0, -1);
	float aa = Math::RadToDeg(atan2(0, -1)); 
	float ab = Math::RadToDeg(atan2(-1, 0));
	//float d = Math::RadToDeg(a.DotProduct(b));
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