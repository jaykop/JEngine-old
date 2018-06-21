#include "MovingEntity.h"

jeBegin

MovingEntity::MovingEntity(Object* _pObject)
    :CustomComponent(_pObject)
{}

void MovingEntity::Register()
{}

void MovingEntity::Load(CR_RJValue /*_data*/)
{}

void MovingEntity::Init()
{
}

void MovingEntity::Update(const float /*_dt*/)
{
    steering
}

void MovingEntity::Close()
{}

bool MovingEntity::OnMessage(Telegram& /*msg*/)
{
    return false;
}

jeEnd