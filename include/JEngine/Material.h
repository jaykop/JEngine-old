#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

jeBegin

class Material : public Component {

    jeBaseFriends(Material);
    friend class GraphicSystem;

public:

    int diffuse, specular;
    float shininess;

private:

    Material(Object* pOwner);
	virtual ~Material();
    void operator= (const Material& copy);

    Material() = delete;
    Material(const Material& /*copy*/) = delete;

    void Load(CR_RJValue _data) override;
    void Register() override {};

    void EditorUpdate(float dt) override;
};

jeDeclareComponentBuilder(Material);
jeEnd
