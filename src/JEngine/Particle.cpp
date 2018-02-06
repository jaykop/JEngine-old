#include "Particle.h"
#include "GLManager.h"
#include "AssetManager.h"
#include "Transform.h"
#include "GraphicSystem.h"
#include "SystemManager.h"
#include "Random.h"

JE_BEGIN

const unsigned Emitter::m_maxSize = 1000;

Emitter::Particle::Particle(Emitter* _emitter)
	: m_emitter(_emitter), m_dead(false)
{
	static Transform* s_pTransform;
	s_pTransform = m_emitter->m_pOwner->GetComponent<Transform>();
	
	m_life		= Random::GetRandomFloat(0.f, m_emitter->m_life);
	m_position	= s_pTransform->m_position;
	m_rotation	= Random::GetRandomFloat(0.f, 360.f);
	m_direction	= Random::GetRandVec3(m_emitter->m_direction.x, m_emitter->m_direction.y);
	m_color.Set(m_emitter->m_startColor);
	
	static float s_rotationSpeed = m_emitter->m_rotationSpeed;

	if (m_emitter->m_type == PARTICLE_EXPLODE)
		m_hidden = false;
	else
		m_hidden = true;

	if (s_rotationSpeed)
		m_rotationSpeed = Random::GetRandomFloat(0., s_rotationSpeed);

	if (m_emitter->m_is2d)
		m_direction.z = 0.f;
}

void Emitter::Particle::Refresh()
{

	if (m_emitter->m_type == PARTICLE_EXPLODE) {
		if (m_emitter->m_size == m_emitter->m_deadCount)
			m_emitter->m_active = false;

		else if (!m_dead) {
			m_dead = true;
			m_emitter->m_deadCount++;
		}
	}

	else {

		static Transform* s_pTransform;
		s_pTransform = m_emitter->m_pOwner->GetComponent<Transform>();

		m_rotation = Random::GetRandomFloat(0.f, 360.f);
		m_rotationSpeed = Random::GetRandomFloat(0., m_emitter->m_rotationSpeed);

		m_hidden = false;
		m_life = Random::GetRandomFloat(0.f, m_emitter->m_life);
		m_color.Set(m_emitter->m_startColor);

		if (m_emitter->m_type == Emitter::PARTICLE_NORMAL) {

			m_position = s_pTransform->m_position;
			m_direction = Random::GetRandVec3(
				m_emitter->m_direction.x,
				m_emitter->m_direction.y);
		}

		else if (m_emitter->m_type == Emitter::PARTICLE_WIDE) {

			m_direction.y = -1;

			static vec3 pos, range;

			range = m_emitter->m_range;
			pos = s_pTransform->m_position;
			m_position.x = Random::GetRandomFloat(pos.x - range.x, pos.x + range.x);
			m_position.y = Random::GetRandomFloat(pos.y - range.y, pos.y + range.y);
			m_position.z = Random::GetRandomFloat(pos.z - range.z, pos.z + range.z);

			m_life = Random::GetRandomFloat(0.f, m_emitter->m_life);
			m_color.Set(m_emitter->m_startColor);

		}

		if (m_emitter->m_is2d)
			m_direction.z = 0.f;
	}
}

Emitter::Emitter(Object* _pOwner)
	:Sprite(_pOwner), m_startColor(vec3::ONE), m_changeColor(true),
	m_endColor(vec3::ZERO), m_life(1.f), m_type(PARTICLE_NORMAL), m_is2d(false),
	m_direction(vec3::ZERO), m_velocity(vec3::ZERO), m_active(true),
	m_deadCount(0), m_renderType(PARTICLERENDER_3D), m_pointSize(0.f),
	m_range(vec3::ZERO), m_size(0), m_colorDiff(vec3::ZERO), m_rotationSpeed(0.f)
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

void Emitter::ManualRefresh()
{
	m_deadCount = 0;
	for (auto particle : m_particles)
		particle->Refresh();
}

void Emitter::Load(CR_RJValue _data)
{
	if (_data.HasMember("Active"))
		m_active = _data["Active"].GetBool();

	if (_data.HasMember("Bilboard"))
		m_bilboard = _data["Bilboard"].GetBool();

	if (_data.HasMember("Projection")) {
		CR_RJValue projection = _data["Projection"];

		if (!strcmp("Perspective", projection.GetString())) {
			m_projection = PROJECTION_PERSPECTIVE;
		}

		else if (!strcmp("Orhtogonal", projection.GetString())) {
			m_projection = PROJECTION_PERSPECTIVE;
		}
	}

	if (_data.HasMember("Texture")) {
		CR_RJValue texture = _data["Texture"];
		AddTexture(texture.GetString());
	}

	if (_data.HasMember("Life")) 
		m_life = _data["Life"].GetFloat();

	if (_data.HasMember("StartColor") 
		&& _data.HasMember("EndColor")) {

		CR_RJValue startColor = _data["StartColor"],
			endColor = _data["EndColor"];

		m_startColor.Set(startColor[0].GetFloat(),
			startColor[1].GetFloat(),
			startColor[2].GetFloat());

		m_endColor.Set(endColor[0].GetFloat(),
			endColor[1].GetFloat(),
			endColor[2].GetFloat());

		SetColors(m_startColor, m_endColor);
	}

	if (_data.HasMember("RenderType")) {
		CR_RJValue RenderType = _data["RenderType"];

		if (!strcmp("Point", RenderType.GetString())) {
			m_renderType = PARTICLERENDER_POINT;
			m_vao = &GLM::m_vao[GLM::SHAPE_POINT];
			m_elementSize = GLM::m_elementSize[GLM::SHAPE_POINT];
		}
		else if (!strcmp("Plane", RenderType.GetString())) {
			m_renderType = PARTICLERENDER_PLANE;
			m_vao = &GLM::m_vao[GLM::SHAPE_PLANE];
			m_elementSize = GLM::m_elementSize[GLM::SHAPE_PLANE];
		}
		else if (!strcmp("Plane3D", RenderType.GetString())) {
			m_renderType = PARTICLERENDER_3D;
			m_vao = &GLM::m_vao[GLM::SHAPE_PLANE3D];
			m_elementSize = GLM::m_elementSize[GLM::SHAPE_PLANE3D];
		}
	}

	if (_data.HasMember("Type")) {
		CR_RJValue type = _data["Type"];
		
		if (!strcmp(type.GetString(), "Normal"))
			m_type = PARTICLE_NORMAL;
		else if (!strcmp(type.GetString(), "Wide"))
			m_type = PARTICLE_WIDE;
		else if (!strcmp(type.GetString(), "Explosion"))
			m_type = PARTICLE_EXPLODE;
	}

	if (_data.HasMember("Range")) {
		CR_RJValue range = _data["Range"];
		m_range.Set(range[0].GetFloat(),
			range[1].GetFloat(),
			range[2].GetFloat());
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

	if (_data.HasMember("RotationSpeed"))
		m_rotationSpeed = _data["RotationSpeed"].GetFloat();

	if (_data.HasMember("PointSize"))
		m_pointSize = _data["PointSize"].GetFloat();
}

void Emitter::SetQuantity(unsigned _quantity)
{
	if (m_particles.empty()) {

		if (m_maxSize < _quantity) {
			_quantity = m_maxSize;
			JE_DEBUG_PRINT("!Emitter - The quantity of particle must be less than 1000.\n");
		}

		for (unsigned i = 0; i < _quantity; ++i)
			m_particles.push_back(new Particle(this));
		m_size = _quantity;
	}

	else
		JE_DEBUG_PRINT("!Emitter - Already allocated.\n");
}

void Emitter::SetColors(const vec3& _start, const vec3& _end)
{
	m_startColor = _start, m_endColor = _end;
	m_colorDiff = (m_endColor - m_startColor) / m_life;

	// If the idff is zero, no need to add diff
	if (m_colorDiff == vec3::ZERO)
		m_changeColor = false;

	else
		m_changeColor = true;
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