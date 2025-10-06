#pragma once

#include <vector>
#include <functional>
class GameObject;

enum class EventType
{
	kCollision,
	kTriggerEnter,
	kTriggerExit
};

struct Event
{
	EventType m_type;
	void* m_sender;
	void* m_receiver;
};

using EventFunction = std::function<void(const Event&)>;

class EventQ final	
{
public:
	EventQ() = default;
	void QueueEvent(EventType type, void* sender, void* receiver);
	void ProcessEvents(const std::vector<GameObject*>& objects);
private:
	std::vector<Event> m_events;
};