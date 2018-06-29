#pragma once
//#include "Model.h"

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

	unsigned	m_drawMode;
    LightType	m_type;
    ProjectType	projection;

    vec3		position, direction, scale;
    vec4		ambient, specular, diffuse, color;
    float		constant, linear, quadratic, cutOff, outerCutOff;
    unsigned	sfactor, dfactor;

    void Register() override;

	void		AddMesh(Mesh* pMesh);
	void		RemoveMesh(unsigned index);
	Mesh*		GetMesh(unsigned index) const;
	unsigned	GetMeshCount() const;

private:
	
	std::vector<Mesh*> meshes_;

    Light(Object* _pOwner);
    ~Light();
    void operator=(const Light& _copy);

    Light() = delete;
    Light(const Light& /*_copy*/) = delete;

	void Load(CR_RJValue _data) override;
	
    void EditorUpdate(float dt) override;
};

jeEnd
