#pragma once
#include "Macro.h"
#include "Component.h"
#include "Vector4.h"

JE_BEGIN

class Object;
class Sprite;
class Material : public Component {

	friend class GraphicSystem;
	friend class ComponentManager;

public:

	// old attribute of light...
	/*vec4 m_ambient, m_diffuse, m_specular;*/
	int m_diffuse, m_specular;
	float m_shininess;

private:

	Material(Object* _owner = nullptr);
	~Material() {};
	Material(const Material& /*_copy*/) {};
	void operator= (const Material& /*_copy*/) {};
};

JE_END