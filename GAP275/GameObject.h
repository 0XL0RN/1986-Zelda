#pragma once

#include <SDL3/SDL.h>
#include "EventQ.h"
#include "unordered_map"
#include "Vector2.h"
#include <string>

class IComponent;

// Concrete class for managing all objects in the game
class GameObject
{
protected:
	using Vector2 = Vector2<float>;
public:
	GameObject(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects);
	virtual ~GameObject() = default;

	// Virtual interface
	virtual void Awake() {}
	virtual void Update(const double& deltaTime) = 0;
	virtual void Draw(SDL_Renderer* pRenderer) = 0;
	virtual bool HandleEvent([[maybe_unused]] SDL_Event* pEvent) { return true; }
	virtual bool Move(const Vector2& direction, double speed = 1);
	virtual std::string GetId() { return m_id; };
	virtual GameObject* Clone() = 0;

	// EventQ
	void RegisterEvent(const EventType& type, const EventFunction& function);
	void HandleEventQ(const Event& event);

	// Getters & Setters
	int& GetDepth() { return m_depth; }
	[[nodiscard]] Vector2 GetPosition() const { return m_position; }
	virtual void SetPosition(const Vector2& position) { m_position = position; }
	void SetComponent(const std::string& name, IComponent* pComponent);
	[[nodiscard]] IComponent* GetComponent(const std::string& name) const;

protected:
	Vector2 m_position;
	int m_depth;
	std::string m_id;
	std::unordered_map<std::string, IComponent*> m_components;
	GameObject* GetObject(const std::string& name) const;
private:
	std::unordered_map<EventType, EventFunction> m_eventHandlers;
	std::vector<GameObject*>& m_objects;
};