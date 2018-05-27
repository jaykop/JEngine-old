#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

jeBegin

class Material : public Component {

    jeBaseFriends(Material);
    friend class GraphicSystem;

public:

    int m_diffuse, m_specular;
    float m_shininess;

private:

    Material(Object* _pOwner);
    ~Material() {};
    void operator= (const Material& _copy);

    Material() = delete;
    Material(const Material& /*_copy*/) = delete;

    void Load(CR_RJValue _data) override;
    void Register() override {};

    void EditorUpdate(const float _dt) override;
};

jeDeclareComponentBuilder(Material);
jeEnd
