#pragma once
#include <vector>
#include "glew.h"
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

	friend class EmitterBuilder;
	friend class GraphicSystem;

	enum ParticleType {PT_NORMAL, PT_EXPLODE, PT_WIDE, PT_SMOG};

	class Particle {

		friend class GraphicSystem;
		friend class Emitter;

	public:

		float	m_life, m_rotation, m_rotateDiff;
		vec3	m_color, m_position, m_direction;
		bool	m_standBy, m_dead;
		
		void	Refresh();
		
	private:

		Particle(Emitter* _emitter);
		~Particle() {};
		Emitter* m_emitter;

		Particle() = delete;
		Particle(const Particle& /*_copy*/) = delete;
		void operator=(const Particle& /*_copy*/) = delete;

	};

	using Particles = std::vector<Particle*>;

public:

	bool			m_active;
	ParticleType	m_type;
	vec3			m_direction, m_velocity, m_range;
	unsigned		m_size;
	float			m_life, m_rotation;

	void Register() override;

	void ManualRefresh();
	void SetQuantity(unsigned _quantity);
	void SetColors(const vec3& _start, const vec3& _end);

private:

	Emitter(Object* _pOwner);
	~Emitter();

	void Load(CR_RJValue _data) override;
	void Refresh(Particle* _particle);

	Particles	m_particles; 
	vec3		m_startColor, m_endColor, m_colorDiff;
	bool		m_changeColor;
	unsigned	m_deadCount;

	Emitter() = delete;
	Emitter(const Emitter& /*_copy*/) = delete;
	void operator=(const Emitter& /*_copy*/) = delete;

	const static unsigned m_maxSize;

};

JE_END