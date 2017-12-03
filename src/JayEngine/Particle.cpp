#include "Particle.h"

JE_BEGIN

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

void Emitter::NormalUpdate(float /*_dt*/)
{
	for (auto particle : m_particles) {
		particle;
	}
}

Component* EmitterBuilder::CreateComponent(Object * _pOwner) const
{
	return new Emitter(_pOwner);
}


JE_END