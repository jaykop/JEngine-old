#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

// For enum ProjectType
#include "GraphicSystem.h"

jeBegin
jeDeclareComponentBuilder(Light);

class Mesh;
class Light : public Component
{

    jeBaseFriends(Light);
    friend class GraphicSystem;

    enum LightType { NORMALLIGHT, DIRECTIONALLIGHT, SPOTLIGHT, POINTLIGHT };


public:

    LightType	m_type;
    ProjectType	projection;

    vec3		position, direction;
    vec4		ambient, specular, diffuse, color;
    float		constant, linear, quadratic, cutOff, outerCutOff;
    unsigned	sfactor, dfactor;

    void Register() override;

private:

	Mesh *m_pMeshes = nullptr;

    Light(Object* _pOwner);
    ~Light();
    void operator=(const Light& _copy);

    Light() = delete;
    Light(const Light& /*_copy*/) = delete;

    void Load(CR_RJValue _data) override;

    void EditorUpdate(const float _dt) override;
};

jeEnd
