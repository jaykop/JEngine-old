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

    vec3    position_, scale_, rotationAxis_;
    float   rotation_;

private:

    Transform(Object* pOwner);
	virtual ~Transform() {};
    void operator= (const Transform& copy);

    void Load(CR_RJValue data) override;
    void Register() override {};

    void EditorUpdate(float dt) override;

    Transform() = delete;
    Transform(const Transform& /*copy*/) = delete;

};

jeDeclareComponentBuilder(Transform);
jeEnd
