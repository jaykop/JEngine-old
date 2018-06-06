#include "Model.h"
#include "GLManager.h"
#include "SystemManager.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Model);

Model::Model(Object* _pOwner)
	:Sprite(_pOwner)
{
	m_vao = &(GLM::m_vao[GLM::SHAPE_CUBE]);
	m_elementSize = GLM::m_elementSize[GLM::SHAPE_CUBE];
	//m_status |= Sprite::IS_MODEL;
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

jeEnd

