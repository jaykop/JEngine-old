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

	friend class EmitterBuilder;
	friend class GraphicSystem;

	enum ParticleType {PT_NORMAL};

	struct Particle {
		vec4 m_color;
		vec3 m_position;
	};

public:

	ParticleType	m_type;
	vec3			m_direction, m_velocity, m_position;

	void Register() override;

private:

	Emitter(Object* _owner);
	~Emitter() {};
	void Load(CR_RJValue _data) override;
	void Update(float _dt);
	void NormalUpdate(float _dt);

	Particles m_particles;

	Emitter() = delete;
	Emitter(const Emitter& /*_copy*/) = delete;
	void operator=(const Emitter& /*_copy*/) = delete;

};

JE_END