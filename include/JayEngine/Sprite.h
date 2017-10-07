#pragma once
#include "Macro.h"
#include "Vector4.h"
#include "Component.h"

NS_JE_BEGIN

class Object;
class Sprite : public Component
{

public:

	enum ProjectType {PERSPECTIVE, ORTHOGONAL};

	friend class GraphicSystem;

	Sprite(Object* _owner);
	~Sprite();

	vec4		m_color;
	ProjectType m_projection;

	// void SetColor(const vec4& _color);
	// const vec4& GetColor() const;

private:

	Sprite(const Sprite& /*_copy*/) {};
	void operator=(const Sprite& /*_copy*/) {};

};

NS_JE_END