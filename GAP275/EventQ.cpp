#include "EventQ.h"
#include "GameObject.h"

void EventQ::QueueEvent(const EventType type, void* sender, void* receiver)
{
	m_events.push_back({ .m_type = type, .m_sender = sender, .m_receiver = receiver });
}

void EventQ::ProcessEvents(const std::vector<GameObject*>& objects)
{
	// Call HandleEvent on each object
	for (const Event& event : m_events)
		for (GameObject* obj : objects)
			obj->HandleEventQ(event);
	m_events.clear();
}