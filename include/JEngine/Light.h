#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

// For enum ProjectType
#include "GraphicSystem.h"

jeBegin
jeDeclareComponentBuilder(Light);

class Light : public Component
{

    jeBaseFriends(Light);
    friend class GraphicSystem;

    enum LightType { NORMALLIGHT, DIRECTIONALLIGHT, SPOTLIGHT, POINTLIGHT };


public:

    LightType	m_type;
    ProjectType	m_projection;

    vec3	m_position, m_direction;
    vec4	m_ambient, m_specular, m_diffuse, m_color;
    float	m_constant, m_linear, m_quadratic, m_cutOff, m_outerCutOff;
    unsigned	m_sfactor, m_dfactor;

    void Register() override;

private:

    Light(Object* _pOwner);
    ~Light();
    void operator=(const Light& _copy);

    Light() = delete;
    Light(const Light& /*_copy*/) = delete;

    void Load(CR_RJValue _data) override;

    void EditorUpdate(const float _dt) override;
};

jeEnd
