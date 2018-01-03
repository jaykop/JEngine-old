#pragma once
#include "Sprite.h"

JE_BEGIN

template<class EffectType>
inline void Sprite::AddEffect() {

	VisualEffect::VEType veType;
	ConvertVEType(typeid(EffectType).name(), veType);

	auto found = m_effects.find(veType);
	if (found != m_effects.end())
		JE_DEBUG_PRINT("Same effect exists already!\n");

	else {
		m_effects.insert(
			Effects::value_type(veType,
				new EffectType(this, veType)));
	}
}

template<class EffectType>
inline EffectType* Sprite::GetEffect() {

	VisualEffect::VEType veType;
	ConvertVEType(typeid(EffectType).name(), veType);

	auto found = m_effects.find(veType);
	if (found != m_effects.end())
		return static_cast<EffectType*>(found->second);

	else {
		JE_DEBUG_PRINT("No such effect in the list.\n");
		return nullptr;
	}
}

template<class EffectType>
inline void Sprite::RemoveEffect() {

	VisualEffect::VEType veType;
	ConvertVEType(typeid(EffectType).name(), veType);

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
inline bool Sprite::HasEffect() {

	VisualEffect::VEType veType;
	ConvertVEType(typeid(EffectType).name(), veType);

	auto found = m_effects.find(veType);
	if (found != m_effects.end())
		return true;

	else {
		JE_DEBUG_PRINT("No such effect in the list.\n");
		return false;
	}
}

JE_END