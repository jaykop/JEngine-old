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

    LightType	type_;

    vec3		direction_;
    vec4		ambient_, specular_, diffuse_;
    float		constant_, linear_, quadratic_, cutOff_, outerCutOff_;
    unsigned	sfactor_, dfactor_;

    void Register() override;

private:
	
    Light(Object* pOwner);
    virtual ~Light();
    void operator=(const Light& copy);

    Light() = delete;
    Light(const Light& /*copy*/) = delete;

	void Load(CR_RJValue data) override;
	
    void EditorUpdate(float dt) override;
};

jeEnd
