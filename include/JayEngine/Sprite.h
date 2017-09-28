#pragma once
#include "Macro.h"
#include "Vector4.h"
#include "Component.h"

NS_JE_BEGIN

class Object;
class Sprite : public Component
{

public:

	friend class GraphicSystem;

	Sprite(Object* _owner);
	~Sprite();

	void SetColor(const vec4& _color);
	const vec4& GetColor() const;

private:

	Sprite(const Sprite& /*_copy*/) {};
	void operator=(const Sprite& /*_copy*/) {};

	vec4 m_color;

};

NS_JE_END