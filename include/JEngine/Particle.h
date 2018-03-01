#pragma once
#include <vector>
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
	EmitterBuilder(const EmitterBuilder& /*_copy*/) = delete;
	void operator=(const EmitterBuilder& /*_copy*/) = delete;

	Component* CreateComponent(Object* _pOwner) const override;

};

class Emitter : public Sprite 
{

	friend class ComponentManager;
	friend class EmitterBuilder;
	friend class GraphicSystem;

	enum ParticleType			{ PARTICLE_NORMAL, PARTICLE_EXPLODE, PARTICLE_WIDE, PARTICLE_SMOG };
	enum ParticleRendertType	{ PARTICLERENDER_POINT, PARTICLERENDER_PLANE, PARTICLERENDER_3D };
	
	class Particle {

		friend class GraphicSystem;
		friend class Emitter;

	public:

		void	Refresh();

		bool	m_hidden, m_dead;
		vec3	m_color, m_position, m_direction, m_velocity;
		float	m_life, m_rotationSpeed, m_rotation;
		
	private:

		Particle(Emitter* _emitter);
		~Particle() {};
		void operator=(const Particle& _copy);

		Emitter* m_emitter;

		Particle() = delete;
		Particle(const Particle& /*_copy*/) = delete;

	};

	using Particles = std::vector<Particle*>;

public:

	void Register() override;

	void ManualRefresh();
	void SetQuantity(unsigned _quantity);
	void SetColors(const vec3& _start, const vec3& _end);

	bool				m_active, m_is2d;
	float				m_life, m_rotationSpeed, m_pointSize;
	unsigned			m_size;
	ParticleType		m_type;
	ParticleRendertType	m_renderType;
	vec3				m_direction, m_velocity, m_range;

private:

	Emitter(Object* _pOwner);
	~Emitter();
	void operator=(const Emitter& _copy);

	void Load(CR_RJValue _data) override;
	void Refresh(Particle* _particle);

	Particles	m_particles; 
	vec3		m_startColor, m_endColor, m_colorDiff;
	bool		m_changeColor;
	unsigned	m_deadCount;

	Emitter() = delete;
	Emitter(const Emitter& /*_copy*/) = delete;

	void EditorUpdate(const float _dt);

	const static unsigned m_maxSize;

};

JE_END