#include "Model.h"

JE_BEGIN

Model::Model(Object* _pOwner)
	:Sprite(_pOwner)
{}

ModelBuilder::ModelBuilder()
	:ComponentBuilder()
{}

Component* ModelBuilder::CreateComponent(Object* _pOwner) const
{
	return new Model(_pOwner);
}

JE_END

