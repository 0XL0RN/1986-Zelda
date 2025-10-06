#pragma once

#include "GameObject.h"
#include "Collider.h"
#include "Trigger.h"
class Boss;
class CollisionManager;

// Link object
class Link final : public GameObject
{
private:
	static constexpr float kMaxHp = 20.0f;
	static constexpr float kSpeed = 150.0f;
public:
	Link(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	~Link() override = default;
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	bool HandleEvent(SDL_Event* pEvent) override;
	GameObject* Clone() override;

	float GetHealth() const { return m_hp; }
	void SetHealth(const float& health);
	Vector2 GetPosition() const { return m_position; }
	void SetPosition(const Vector2& position) override;
	bool IsAlive() const { return m_isAlive; }
	void SetSaveHealth(const float& health) { m_hp = health; }
private:
	bool m_movingToTarget;
	Vector2 m_targetPosition;
	std::string m_lastAnimation;
	Collider m_collider;
	Trigger m_range;
	float m_hp;
	bool m_isAlive;
	bool m_isColliding;
	bool m_IsInRange;
	Boss* m_pTarget;
};