#include "EventHandler.h"

NS_JE_BEGIN

template<typename ObjectType>
inline void EventHandler::RegisterEvent(void)
{
	auto found = m_eventMap.find(type_info(typeid(EventT)));

	// If there is a registered method
	if (m_eventMap.end() == found)
		m_eventMap.insert(
			EventMap::value_type(
				type_info(typeid(EventT)), ));
}


template<typename ObjectType, typename EventT>
inline void EventHandler::UpdateEvent(
	ObjectType * _obj, void(ObjectType::* _method)(EventT *))
{
	auto found = m_eventMap.find(type_info(typeid(EventT)));

	// If there is a registered method
	if (m_eventMap.end() != found)
		found.second->Update(0.f);

}


NS_JE_END