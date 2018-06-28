#pragma once
#include "Vector3.h"
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

jeBegin

class Transform : public Component
{
    jeBaseFriends(Transform);
    friend class PhysicsSystem;

public:

    vec3    position, scale, rotationAxis;
    float   rotation;

private:

    Transform(Object* _pOwner);
    ~Transform() {};
    void operator= (const Transform& _copy);

    void Load(CR_RJValue _data) override;
    void Register() override {};

    void EditorUpdate(float dt) override;

    Transform() = delete;
    Transform(const Transform& /*_copy*/) = delete;

};

jeDeclareComponentBuilder(Transform);
jeEnd
