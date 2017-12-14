#include "Particle.h"
#include "Random.h"
#include "AssetManager.h"

JE_BEGIN

Emitter::Particle::Particle(Emitter* _emitter)
	: m_emitter(_emitter), m_color(_emitter->m_color),
	m_texture(0), m_position(_emitter->m_position)
{}

void Emitter::Particle::SetTexture(const char* _key) 
{
	m_texture = AssetManager::GetTexture(_key);
}

unsigned Emitter::Particle::GetTexture() 
{
	return m_texture;
}

Emitter::Emitter(Object* _owner)
	:Sprite(_owner), m_startColor(vec4::ONE),
	m_endColor(vec4::ZERO), m_life(1.f), m_type(PT_NORMAL),
	m_position(vec3::ZERO), m_direction(vec3::ZERO), m_velocity(vec3::ZERO)
{}

Emitter::~Emitter() 
{
	// Clear particles
	for (auto particle : m_particles) {
		delete particle;
		particle = nullptr;
	}
}

void Emitter::Register()
{
}

void Emitter::Load(CR_RJValue /*_data*/)
{
}

void Emitter::Update(float _dt)
{
	switch (m_type) {
	
	case PT_NORMAL:
	default :
		NormalUpdate(_dt);
		break;
	}
}

void Emitter::NormalUpdate(float _dt)
{
	for (auto particle : m_particles) {
		//Random::;
		//particle->m_position += _dt * m_velocity * ;
		//particle->m_color += _dt * m_life;
	}
}

void Emitter::SetQuantity(unsigned _quantity)
{
	if (m_particles.empty())
		for (unsigned i = 0; i < _quantity; ++i)
			m_particles.push_back(new Particle(this));

	else
		JE_DEBUG_PRINT("Already allocated.\n");
}

void Emitter::Refresh(Particle *_particle)
{
	_particle->m_position = m_position;
	_particle->m_color = m_startColor;

}

Component* EmitterBuilder::CreateComponent(Object * _pOwner) const
{
	return new Emitter(_pOwner);
}


JE_END