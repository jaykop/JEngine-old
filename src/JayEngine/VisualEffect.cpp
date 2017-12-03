// TODO
// Move these codes to header file

#include "VisualEffect.h"

JE_BEGIN

VisualEffect::VisualEffect(Sprite* _ownerSprite, VEType type)
	:m_pSprite(_ownerSprite), m_active(true), m_type(type)
{}


JE_END