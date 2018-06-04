#pragma once

#include "MovingEntity.h"

jeBegin;

class Object;
class Steering : public MovingEntity
{
    jeBaseFriends(Steering);

public:

    vec3 Seek(const vec3& _targetPos);
    bool AccumulateForce(const vec3& forceToAdd);
    vec3 Calculate();

    Object* m_target = nullptr;

    vec3 velocity;
    vec3 heading;
    vec3 side;

    float mass;
    float maxSpeed;
    float maxForce;
    float maxTurnRate;



private:

    Steering(Object* pObject);
    ~Steering() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(const float _dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*_dt*/) override {};
};

jeDeclareCustomComponentBuilder(Steering);

jeEnd;