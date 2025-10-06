#pragma once

#include "AiState.h"
#include "Vector2.h"
#include "mt19937.h"
#include <string>
class EnemyStateMachine;
class AnimationComponent;
class GameObject;

class EnemyWanderState final : public AiState
{
private:
	using Vector2 = Vector2<float>;
	static constexpr float kMaxDistance = 225.0;
	static constexpr double kWaitTime = 1.0;
	static constexpr float kSpeed = 90.0;

	enum Direction
	{
		kUp,
		kDown,
		kLeft,
		kRight
	};
public:
	EnemyWanderState(EnemyStateMachine* pOwner, GameObject* pTarget);
	~EnemyWanderState() override;

	void Enter() override;
	void Update(const double& deltaTime) override;
	void Exit() override;
private:
	EnemyStateMachine* m_pOwner;
	AnimationComponent* m_pAnimationComponent;
	GameObject* m_pTarget;
	mt19937 m_twister;
	Vector2 m_destination;
	bool m_IsAtDestination;
	double m_waitTimer;
	std::string m_pSpriteBuffer;

	void SetRandomDestination();
};