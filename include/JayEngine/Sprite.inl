#pragma once
#include <type_info>
#include "Sprite.h"

JE_BEGIN

template<class EffectType>
inline void Sprite::AddEffect()
{
	VisualEffect::VEType veType;
	auto type = typeid(EffectType);

	ConvertVEType(type, veType);

	auto found = m_effects.find(veType);
	if (found != m_effects.end())
		JE_DEBUG_PRINT("Same effect exists already!\n");

	else {
		m_effects.insert(
			Effects::value_type(veType,
				new VisualEffect(this, )));
	}
}

template<class EffectType>
inline EffectType* Sprite::GetEffect()
{
	VisualEffect::VEType veType;
	auto type = typeid(EffectType);

	ConvertVEType(type, veType);

	auto found = m_effects.find(veType);
	if (found != m_effects.end())
		return static_cast<EffectType>(found->second);

	else {
		JE_DEBUG_PRINT("No such effect in the list.\n");
		return nullptr;
	}
}

template<class EffectType>
inline void Sprite::RemoveEffect()
{
	VisualEffect::VEType veType;
	auto type = typeid(EffectType);
	ConvertVEType(type, veType);

	auto found = m_effects.find(type);
	if (found == m_effects.end())
		JE_DEBUG_PRINT("No such effect in the list.\n");

	else {
		delete found->second;
		found->second = nullptr;
		m_effects.erase(found);
	}
}

template<class EffectType>
inline bool Sprite::HasEffect()
{
	VisualEffect::VEType veType;
	auto type = typeid(EffectType);

	ConvertVEType(type, veType);

	auto found = m_effects.find(veType);
	if (found != m_effects.end())
		return true;

	else {
		JE_DEBUG_PRINT("No such effect in the list.\n");
		return false;
	}
}

void Sprite::ConvertVEType(std::type_info& _type, VisualEffect::VEType& _veType)
{
	switch (_type) {

	case typeid(Blur) :
		_veType = VisualEffect::VEType::VE_BLUR;
		break;
	case typeid(Sobel) :
		_veType = VisualEffect::VEType::VE_SOBEL;
		break;
	case typeid(Inverse) :
		_veType = VisualEffect::VEType::VE_INVERSE;
		break;
	case typeid(Manipulation) :
		_veType = VisualEffect::VEType::VE_MANIPULATION;
		break;
	}
}

JE_END