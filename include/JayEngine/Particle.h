#pragma once
#include "Sprite.h"
#include "Vector3.h"

JE_BEGIN

class EmitterBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	EmitterBuilder();
	~EmitterBuilder() {};
	EmitterBuilder(const EmitterBuilder& /*_copy*/) {};
	void operator=(const EmitterBuilder& /*_copy*/) {};

	Component* CreateComponent(Object* _pOwner) const override;

};


class Emitter : public Sprite 
{

	enum ParticleType {};

public:

	ParticleType	m_type;
	vec3			m_direction, m_velocity;

	void Register() override;

private:

	struct Particle {

	};

	Emitter(Object* _owner = nullptr);
	~Emitter() {};
	Emitter(const Emitter& /*_copy*/) {};
	void operator=(const Emitter& /*_copy*/) {};

	void Load(CR_RJValue _data) override;

};

JE_END