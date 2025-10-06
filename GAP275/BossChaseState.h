#pragma once

#include "AiState.h"
#include "Vector2.h"
class BossStateMachine;
class GameObject;
class AnimationComponent;

class BossChaseState final : public AiState
{
private:
	using Vector2 = Vector2<float>;
	static constexpr float kSpeed = 70.0;
public:
	explicit BossChaseState(BossStateMachine* pOwner, GameObject* pTarget);
	~BossChaseState() override;

	void Enter() override;
	void Update(const double& deltaTime) override;
	void Exit() override;
private:
	BossStateMachine* m_pOwner;
	GameObject* m_pTarget;
	AnimationComponent* m_pAnimationComponent;
};