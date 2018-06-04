#pragma once
#include "CustomComponent.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"
#include "Vector3.h"

jeBegin;

class Object;
class MovingEntity : public CustomComponent
{
    jeBaseFriends(MovingEntity);

public:

protected:

    MovingEntity(Object* pObject);
    ~MovingEntity() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(const float _dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*_dt*/) override {};
};

jeEnd;