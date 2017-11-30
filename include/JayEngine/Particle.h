#pragma once
#include "Sprite.h"
#include "Vector3.h"

JE_BEGIN

class Particle;
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

using Particles = std::vector<Particle*>;

class Emitter : public Sprite 
{

	enum ParticleType {PT_NORMAL};

public:

	ParticleType	m_type;
	vec3			m_direction, m_velocity;

	void Register() override;

private:

	struct Particle {
		vec4 m_color;

	};

	Emitter(Object* _owner);
	~Emitter() {};
	void Load(CR_RJValue _data) override;

	Emitter() = delete;
	Emitter(const Emitter& /*_copy*/) = delete;
	void operator=(const Emitter& /*_copy*/) = delete;

	Particles m_particles;
};

JE_END