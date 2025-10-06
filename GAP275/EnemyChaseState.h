#pragma once

#include "AiState.h"
#include "Vector2.h"
class EnemyStateMachine;
class AnimationComponent;
class GameObject;

class EnemyChaseState final : public AiState
{
private:
	using Vector2 = Vector2<float>;
	static constexpr float kMaxDistance = 270.0;
	static constexpr float kSpeed = 70.0;
public:
	EnemyChaseState(EnemyStateMachine* pOwner, GameObject* pTarget);
	~EnemyChaseState() override;

	void Enter() override;
	void Update(const double& deltaTime) override;
	void Exit() override;
private:
	EnemyStateMachine* m_pOwner;
	GameObject* m_pTarget;
	AnimationComponent* m_pAnimationComponent;
	Vector2 m_oldPos;

	void UpdateSpriteCardinal(const Vector2& oldPos, const Vector2& newPos);
};