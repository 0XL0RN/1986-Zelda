#pragma once

#include "GameObject.h"
#include "BossStateMachine.h"
#include "Collider.h"
class CollisionManager;

class Boss final : public GameObject
{
private:
	static constexpr size_t kMaxHitBox = 4;
	static constexpr int kMaxHp = 100;

public:
	struct Hitbox
	{
	public:
		Collider m_rect;
		Vector2 m_offset;
	};
	Boss(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	void Awake() override;
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	bool Move(const Vector2& direction, double speed) override;
	void SetPosition(const Vector2& position) override;
	GameObject* Clone() override;
	Hitbox* GetCollider() { return m_hitBox; }
	void SetHealth(const int& hp);
	[[nodiscard]] bool IsAlive() const { return m_hp > 0; }
	void SetDmg(const int& damage) { m_damage = damage; }

private:
	BossStateMachine m_stateMachine;
	GameObject* m_pTarget;
	Hitbox m_hitBox[kMaxHitBox];
	int m_hp;
	int m_damage;
	bool m_attack;
	float m_attackTimer;
};
