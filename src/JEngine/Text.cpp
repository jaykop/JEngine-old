#include "Text.h"
#include "Transform.h"
#include "GLManager.h"
#include "SystemManager.h"

JE_BEGIN

Text::Text(Object* _pOwner)
	:Sprite(_pOwner)
{
	//m_isText = true;
	m_vao = &(GLM::m_vao[GLM::SHAPE_PLANE]);
	m_elementSize = GLM::m_elementSize[GLM::SHAPE_PLANE];
}

Text::~Text()
{
	SYSTEM::GetGraphicSystem()->RemoveSprite(this);
}

void Text::Register()
{
	SYSTEM::GetGraphicSystem()->AddSprite(this);
	if (m_pOwner->HasComponent<Transform>())
		m_transform = m_pOwner->GetComponent<Transform>();
}

void Text::Load(CR_RJValue /*_data*/)
{

}


TextBuilder::TextBuilder()
	:ComponentBuilder()
{}

Component* TextBuilder::CreateComponent(Object* _pOwner) const
{
	return new Text(_pOwner);
}

JE_END

