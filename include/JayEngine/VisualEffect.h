#pragma once
#include "Macro.h"

JE_BEGIN

class Sprite;
class VisualEffect 
{
	friend class Sprite;
	friend class GraphicSystem;

public:

	enum VEType { VS_BLUR, VS_SOBEL, VS_MANIPULATION, VS_INVERSE };

	bool m_active;

private:

	VisualEffect(Sprite* _ownerSprite, VEType type);

	Sprite* m_pSprite;
	VEType	m_type;

	VisualEffect() = delete;
	VisualEffect(const VisualEffect& _copy) = delete;
	void operator=(const VisualEffect& _copy) = delete;

};

JE_END