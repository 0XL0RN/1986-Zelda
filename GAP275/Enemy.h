#pragma once

#include "GameObject.h"
#include "EnemyStateMachine.h"
#include "Collider.h"

class CollisionManager;

class Enemy final : public GameObject
{
private:
	static constexpr float kDetectSquare = 27000;
public:
	Enemy(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	~Enemy() override = default;
	void Awake() override;
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	bool Move(const Vector2& direction, double speed) override;
	void SetPosition(const Vector2& position) override;
	GameObject* Clone() override;

	Collider* GetCollider() { return &m_collider; }

private:
	EnemyStateMachine m_stateMachine;
	Collider m_collider;
	GameObject* m_pTarget;
};