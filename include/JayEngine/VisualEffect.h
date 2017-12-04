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

	enum VEType { VE_BLUR, VE_SOBEL, VE_MANIPULATION, VE_INVERSE };
	bool m_active;

protected:

	VisualEffect(Sprite* _ownerSprite, VEType type);

	Sprite* m_pSprite;
	VEType	m_type;

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

public:

private:

	Blur(Sprite* _ownerSprite, VEType type);

	Blur() = delete;
	Blur(const Blur& /*_copy*/) = delete;
	void operator=(const Blur& /*_copy*/) = delete;
};

//////////////////////////////////////////////////////////////////////////
// Sobel class
//////////////////////////////////////////////////////////////////////////
class Sobel : public VisualEffect
{

public:

private:

	Sobel(Sprite* _ownerSprite, VEType type);

	Sobel() = delete;
	Sobel(const Sobel& /*_copy*/) = delete;
	void operator=(const Sobel& /*_copy*/) = delete;
};

//////////////////////////////////////////////////////////////////////////
// Manipulation class
//////////////////////////////////////////////////////////////////////////
class Manipulation : public VisualEffect
{

public:

private:

	Manipulation(Sprite* _ownerSprite, VEType type);

	Manipulation() = delete;
	Manipulation(const Manipulation& /*_copy*/) = delete;
	void operator=(const Manipulation& /*_copy*/) = delete;
};

//////////////////////////////////////////////////////////////////////////
// Inverse class
//////////////////////////////////////////////////////////////////////////
class Inverse : public VisualEffect 
{

public:

private:

	Inverse(Sprite* _ownerSprite, VEType type);

	Inverse() = delete;
	Inverse(const Inverse& /*_copy*/) = delete;
	void operator=(const Inverse& /*_copy*/) = delete;
};

JE_END