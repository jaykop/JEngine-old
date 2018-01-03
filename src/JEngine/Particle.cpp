#include "Particle.h"
#include "AssetManager.h"
#include "Transform.h"
#include "GraphicSystem.h"
#include "SystemManager.h"
#include "Random.h"

JE_BEGIN

Emitter::Particle::Particle(Emitter* _emitter)
	: m_emitter(_emitter), m_direction(vec3::ZERO)
{
	m_life		= Random::GetRandomFloat(0.f, m_emitter->m_life);
	m_position	= m_emitter->m_pOwner->GetComponent<Transform>()->m_position;
	m_direction = Random::GetRandVec3(m_emitter->m_direction.x,
		m_emitter->m_direction.y);
	m_direction.z = 0.f;
	m_color.Set(m_emitter->m_startColor);
}

void Emitter::Particle::Refresh()
{
	m_direction = Random::GetRandVec3(
		m_emitter->m_direction.x,
		m_emitter->m_direction.y); 
	m_direction.z = 0.f;
	m_position	= m_emitter->m_transform->m_position;
	m_life		= Random::GetRandomFloat(0.f, m_emitter->m_life);
	m_color.Set(m_emitter->m_startColor);
}

Emitter::Emitter(Object* _owner)
	:Sprite(_owner), m_startColor(vec3::ONE),
	m_endColor(vec3::ZERO), m_life(1.f), m_type(PT_NORMAL),
	m_direction(vec3::ZERO), m_velocity(vec3::ZERO), m_active(true)
{
	m_isEmitter = true;
}

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
	SystemManager::GetGraphicSystem()->AddSprite(this);
	if (m_pOwner->HasComponent<Transform>())
		m_transform = m_pOwner->GetComponent<Transform>();
}

void Emitter::Load(CR_RJValue _data)
{
	if (_data.HasMember("Active"))
		m_active = _data["Active"].GetBool();

	if (_data.HasMember("Projection")) {
		CR_RJValue projection = _data["Projection"];

		if (!strcmp("Perspective", projection.GetString())) {
			m_projection = PERSPECTIVE;
		}

		else if (!strcmp("Orhtogonal", projection.GetString())) {
			m_projection = ORTHOGONAL;
		}
	}

	if (_data.HasMember("Texture")) {
		CR_RJValue texture = _data["Texture"];
		AddTexture(texture.GetString());
	}

	if (_data.HasMember("Life")) 
		m_life = _data["Life"].GetFloat();

	if (_data.HasMember("StartColor")) {
		CR_RJValue startColor = _data["StartColor"];
		m_startColor.Set(startColor[0].GetFloat(),
			startColor[1].GetFloat(),
			startColor[2].GetFloat());
	}

	if (_data.HasMember("EndColor")) {
		CR_RJValue endColor = _data["EndColor"];
		m_endColor.Set(endColor[0].GetFloat(),
			endColor[1].GetFloat(),
			endColor[2].GetFloat());
	}

	if (_data.HasMember("Type")) {
		CR_RJValue type = _data["Type"];
		
		if (!strcmp(type.GetString(), "Normal"))
			m_type = PT_NORMAL;
	}

	if (_data.HasMember("Direction")) {
		CR_RJValue direction = _data["Direction"];
		m_direction.Set(direction[0].GetFloat(),
			direction[1].GetFloat(),
			direction[2].GetFloat());
	}
	
	if (_data.HasMember("Velocity")) {
		CR_RJValue velocity = _data["Velocity"];
		m_velocity.Set(velocity[0].GetFloat(),
			velocity[1].GetFloat(),
			velocity[2].GetFloat());
	}

	if (_data.HasMember("Quantity"))
		SetQuantity(_data["Quantity"].GetUint());
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
	_particle->m_position = m_transform->m_position;
	_particle->m_color = m_startColor;

}

EmitterBuilder::EmitterBuilder()
	:ComponentBuilder()
{}

Component* EmitterBuilder::CreateComponent(Object * _pOwner) const
{
	return new Emitter(_pOwner);
}


JE_END