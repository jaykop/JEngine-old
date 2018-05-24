#include "Model.h"
#include "GLManager.h"
#include "SystemManager.h"
#include "MemoryAllocator.h"

jeBegin

jeDefineStaticAllocator(Model);

Model::Model(Object* _pOwner)
	:Sprite(_pOwner)
{
	m_vao = &(GLM::m_vao[GLM::SHAPE_CUBE]);
	m_elementSize = GLM::m_elementSize[GLM::SHAPE_CUBE];
}

Model::~Model()
{
	SYSTEM::GetGraphicSystem()->RemoveSprite(this);
}

void Model::operator=(const Model & _copy)
{
	(*this).Sprite::operator=(_copy);
}

void Model::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

ModelBuilder::ModelBuilder()
	:ComponentBuilder()
{}

Component* ModelBuilder::CreateComponent(Object* _pOwner) const
{
	return new Model(_pOwner);
}

jeEnd

