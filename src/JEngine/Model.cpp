#include "Model.h"
#include "GLManager.h"
#include "SystemManager.h"
#include "Mesh.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Model);

Model::Model(Object* _pOwner)
	:Sprite(_pOwner)
{
	pVao = &(GLM::m_vao[GLM::SHAPE_CUBE]);
}

Model::~Model()
{
	if (m_pMeshes) {
		delete m_pMeshes;
		m_pMeshes = nullptr;
	}

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

