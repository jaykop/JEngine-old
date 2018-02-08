#include "Model.h"
#include "GLManager.h"
#include "SystemManager.h"

JE_BEGIN

Model::Model(Object* _pOwner)
	:Sprite(_pOwner)
{
	//m_isModel = true;
	m_vao = &(GLM::m_vao[GLM::SHAPE_CUBE]);
	m_elementSize = GLM::m_elementSize[GLM::SHAPE_CUBE];
}

Model::~Model()
{
	SYSTEM::GetGraphicSystem()->RemoveSprite(this);
}

ModelBuilder::ModelBuilder()
	:ComponentBuilder()
{}

Component* ModelBuilder::CreateComponent(Object* _pOwner) const
{
	return new Model(_pOwner);
}

JE_END

