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
	: m_emitter(_emitter), m_direction(vec3::ZERO), 
	m_rotation(0.f), m_rotateDiff(0.f), m_standBy(true)
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
	m_standBy = false;

	m_direction = Random::GetRandVec3(
		m_emitter->m_direction.x,
		m_emitter->m_direction.y); 
	
	//m_direction.z = 0.f; // For2d

	m_position	= m_emitter->m_transform->m_position;
	m_life		= Random::GetRandomFloat(0.f, m_emitter->m_life);
	m_color.Set(m_emitter->m_startColor);
}

void Emitter::Particle::RainRefresh()
{
	m_standBy = false;

	//m_direction = Random::GetRandVec3(
	//	m_emitter->m_direction.x,
	//	m_emitter->m_direction.y);
	m_direction.y = -1;// Random::GetRandomFloat(0.f, -1.f);
	//m_direction.z = 0.f;
	static vec3 pos, range;
	static float rotate;
	
	range		= m_emitter->m_range;
	pos			= m_emitter->m_transform->m_position;
	rotate		= m_emitter->m_transform->m_rotation;
	m_rotateDiff = Random::GetRandomFloat(-rotate, rotate);
	m_position.x = Random::GetRandomFloat(pos.x - range.x, pos.x + range.x);
	m_position.y = Random::GetRandomFloat(pos.y - range.y, pos.y + range.y);
	m_position.z = Random::GetRandomFloat(pos.z - range.z, pos.z + range.z);

	//m_position = m_emitter->m_transform->m_position;
	m_life = Random::GetRandomFloat(0.f, m_emitter->m_life);
	m_color.Set(m_emitter->m_startColor);
}

Emitter::Emitter(Object* _owner)
	:Sprite(_owner), m_startColor(vec3::ONE), m_changeColor(true),
	m_endColor(vec3::ZERO), m_life(1.f), m_type(PT_NORMAL),
	m_direction(vec3::ZERO), m_velocity(vec3::ZERO), m_active(true),
	m_range(vec3::ZERO), m_count(0), m_size(0), m_colorDiff(vec3::ZERO),
	m_colorData(nullptr), m_positionData(nullptr)
{
	m_isEmitter = true;
}

Emitter::~Emitter() 
{

	delete[] m_colorData; 
	delete[] m_positionData;

	// Clear particles
	for (auto particle : m_particles) {
		delete particle;
		particle = nullptr;
	}

}

void Emitter::Register()
{
	SystemManager::GetGraphicSystem()->AddSprite(this);
	if (m_pOwner->HasComponent<Transform>()) {
		m_transform = m_pOwner->GetComponent<Transform>();

		SetEmitter();
	}
}

void Emitter::ManualRefresh()
{
	m_count = 0;
	for (auto particle : m_particles)
		particle->Refresh();
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

	if (_data.HasMember("Type")) {
		CR_RJValue type = _data["Type"];
		
		if (!strcmp(type.GetString(), "Normal"))
			m_type = PT_NORMAL;
		else if (!strcmp(type.GetString(), "Rain"))
			m_type = PT_RAIN;
		else if (!strcmp(type.GetString(), "Explosion"))
			m_type = PT_EXPLODE;
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
}

void Emitter::SetQuantity(unsigned _quantity)
{
	if (m_particles.empty()) {

		if (m_maxSize < _quantity) {
			_quantity = m_maxSize;
			JE_DEBUG_PRINT("*Emitter: The quantity of particle must be less than 1000.\n");
		}

		for (unsigned i = 0; i < _quantity; ++i)
			m_particles.push_back(new Particle(this));
		m_size = _quantity;
	}

	else
		JE_DEBUG_PRINT("*Emitter: Already allocated.\n");
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

void Emitter::SetEmitter()
{
	m_positionData = new float[3 * m_size];
	m_colorData = new float[4 * m_size];

	glGenVertexArrays(1, &m_vao);

	glGenBuffers(1, &m_center);
	glBindBuffer(GL_ARRAY_BUFFER, m_center);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLM::m_verticesParticle), GLM::m_verticesParticle, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &m_position);
	glBindBuffer(GL_ARRAY_BUFFER, m_position);
	glBufferData(GL_ARRAY_BUFFER, m_size * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &m_color);
	glBindBuffer(GL_ARRAY_BUFFER, m_color);
	glBufferData(GL_ARRAY_BUFFER, m_size * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

}

EmitterBuilder::EmitterBuilder()
	:ComponentBuilder()
{}

Component* EmitterBuilder::CreateComponent(Object * _pOwner) const
{
	return new Emitter(_pOwner);
}

JE_END