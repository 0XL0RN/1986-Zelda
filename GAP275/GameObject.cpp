#include "GameObject.h"
#include "IComponent.h"

GameObject::GameObject(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects)
	: m_position{ .m_x = position.m_x, .m_y = position.m_y }
	, m_depth(depth)
	, m_id(id)
	, m_objects(objects)
{
}

bool GameObject::Move([[maybe_unused]] const Vector2& direction, [[maybe_unused]] double speed)
{
	return true;
}

void GameObject::SetComponent(const std::string& name, IComponent* pComponent)
{
	if (!pComponent)
		return;

	m_components[name] = pComponent;
}

IComponent* GameObject::GetComponent(const std::string& name) const
{
	auto it = m_components.find(name);
	return (it != m_components.end()) ? it->second : nullptr;
}

void GameObject::RegisterEvent(const EventType& type, const EventFunction& function)
{
	m_eventHandlers[type] = function;
}

void GameObject::HandleEventQ(const Event& event)
{
	const auto it = m_eventHandlers.find(event.m_type);
	if (it != m_eventHandlers.end())
		it->second(event);
}

GameObject* GameObject::GetObject(const std::string& name) const
{
	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		if (name == m_objects[i]->GetId())
			return m_objects[i];
	}

	return nullptr;
}
