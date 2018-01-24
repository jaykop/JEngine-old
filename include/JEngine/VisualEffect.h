#pragma once
#include "Macro.h"

JE_BEGIN

class Sprite;

//////////////////////////////////////////////////////////////////////////
// VisualEffect class
//////////////////////////////////////////////////////////////////////////
class VisualEffect 
{
	friend class Sprite;
	friend class GraphicSystem;

public:

	enum VisualEffectType { VISUALEFFECT_NONE, VISUALEFFECT_BLUR, VISUALEFFECT_SOBEL, VISUALEFFECT_INVERSE };
	bool m_active;

protected:

	VisualEffect(Sprite* _pOwnerSprite, VisualEffectType type);

	Sprite*				m_pSprite;
	VisualEffectType	m_type;

private:

	VisualEffect() = delete;
	VisualEffect(const VisualEffect& /*_copy*/) = delete;
	void operator=(const VisualEffect& /*_copy*/) = delete;

};

//////////////////////////////////////////////////////////////////////////
// Blur class
//////////////////////////////////////////////////////////////////////////
class Blur : public VisualEffect
{
	friend class Sprite;
public:

	float m_amount, m_size;

private:

	Blur(Sprite* _pOwnerSprite, VisualEffectType type);

	Blur() = delete;
	Blur(const Blur& /*_copy*/) = delete;
	void operator=(const Blur& /*_copy*/) = delete;
};

//////////////////////////////////////////////////////////////////////////
// Sobel class
//////////////////////////////////////////////////////////////////////////
class Sobel : public VisualEffect
{
	friend class Sprite;
public:

	float m_amount;

private:

	Sobel(Sprite* _pOwnerSprite, VisualEffectType type);

	Sobel() = delete;
	Sobel(const Sobel& /*_copy*/) = delete;
	void operator=(const Sobel& /*_copy*/) = delete;
};

//////////////////////////////////////////////////////////////////////////
// Inverse class
//////////////////////////////////////////////////////////////////////////
class Inverse : public VisualEffect 
{
	friend class Sprite;
public:

private:

	Inverse(Sprite* _pOwnerSprite, VisualEffectType type);

	Inverse() = delete;
	Inverse(const Inverse& /*_copy*/) = delete;
	void operator=(const Inverse& /*_copy*/) = delete;
};

JE_END