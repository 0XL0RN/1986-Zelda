#pragma once

#include "AiState.h"
#include "Vector2.h"
#include "mt19937.h"
class BossStateMachine;
class AnimationComponent;

class BossWanderState final : public AiState
{
private:
	using Vector2 = Vector2<float>;
	static constexpr float kMaxDistance = 225.0;
	static constexpr double kWaitTime = 1.0;
	static constexpr float kSpeed = 75.0;

	enum Direction
	{
		kUp,
		kDown,
		kLeft,
		kRight
	};
public:
	explicit BossWanderState(BossStateMachine* pOwner);
	~BossWanderState() override;

	void Enter() override;
	void Update(const double& deltaTime) override;
	void Exit() override;
private:
	BossStateMachine* m_pOwner;
	AnimationComponent* m_pAnimationComponent;
	mt19937 m_twister;
	Vector2 m_destination;
	bool m_IsAtDestination;
	double m_waitTimer;
	std::string m_pSpriteBuffer;

	void SetRandomDestination();
};