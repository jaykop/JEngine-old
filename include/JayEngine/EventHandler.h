#pragma once
#include <vector>
#include <Macro.h>

NS_JE_BEGIN

class Component;
class EventHandler {

public:

	typedef std::vector<Component> ComponentTypeList;

	template <typename ObjectType>
	void RegisterEvent(void);

	template <typename ObjectType, typename EventT>
	void UpdateEvent(ObjectType* _obj, void (ObjectType::*_method)(EventT*));



private:

	ComponentTypeList m_ctList;

};

NS_JE_END

#include "EventHandler.inl"