#pragma once
#include "Model.h"

jeBegin
jeDeclareComponentBuilder(Light);

class Mesh;
class Light : public Model
{
    jeBaseFriends(Light);
    friend class GraphicSystem;

    enum LightType { NORMALLIGHT, DIRECTIONALLIGHT, SPOTLIGHT, POINTLIGHT };

public:

    LightType	m_type;

    vec3		direction;
    vec4		ambient, specular, diffuse;
    float		constant, linear, quadratic, cutOff, outerCutOff;
    unsigned	sfactor, dfactor;

    void Register() override;

private:
	
    Light(Object* pOwner);
    virtual ~Light();
    void operator=(const Light& copy);

    Light() = delete;
    Light(const Light& /*copy*/) = delete;

	void Load(CR_RJValue _data) override;
	
    void EditorUpdate(float dt) override;
};

jeEnd
