#include "Model.h"

JE_BEGIN

Model::Model(Object* _owner)
	:Sprite(_owner)
{
	m_isModel = true;
}

ModelBuilder::ModelBuilder()
	:ComponentBuilder()
{}

Component* ModelBuilder::CreateComponent(Object* _pOwner) const
{
	return new Model(_pOwner);
}

JE_END

