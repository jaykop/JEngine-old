#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

jeBegin

class Material : public Component {

    jeBaseFriends(Material);
    friend class GraphicSystem;

public:

    int diffuse_, specular_;
    float shininess_;

private:

    Material(Object* pOwner);
	virtual ~Material();
    void operator= (const Material& copy);

    Material() = delete;
    Material(const Material& /*copy*/) = delete;

    void Load(CR_RJValue data) override;
    void Register() override {};

    void EditorUpdate(float dt) override;
};

jeDeclareComponentBuilder(Material);
jeEnd
