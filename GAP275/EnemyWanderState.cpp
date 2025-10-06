#include "EnemyWanderState.h"
#include "EnemyStateMachine.h"
#include "AnimationComponent.h"
#include "Enemy.h"

EnemyWanderState::EnemyWanderState(EnemyStateMachine* pOwner, GameObject* pTarget)
	: m_pOwner(pOwner)
	, m_pAnimationComponent(nullptr)
	, m_pTarget(pTarget)
	, m_destination(pOwner->GetOwner()->GetPosition())
	, m_IsAtDestination(false)
	, m_waitTimer(0.0)
	, m_pSpriteBuffer("down")
{
}

EnemyWanderState::~EnemyWanderState()
{

}

void EnemyWanderState::Enter()
{
	m_pAnimationComponent = static_cast<AnimationComponent*>(m_pOwner->GetOwner()->GetComponent("Animation"));
}

void EnemyWanderState::Update(const double& deltaTime)
{
	Vector2 enemyPos = m_pOwner->GetOwner()->GetPosition();

	if (m_IsAtDestination)
	{
		// Wait before moving again
		m_waitTimer += deltaTime;

		if (m_waitTimer >= kWaitTime)
		{
			SetRandomDestination();
			m_IsAtDestination = false;
			m_waitTimer = 0.0; // Reset timer
		}
		m_pAnimationComponent->PauseAnimation(); // Stay paused while waiting
	}
	else
	{
		// Calculate movement vector and distance
		const float dx = m_destination.m_x - enemyPos.m_x;
		const float dy = m_destination.m_y - enemyPos.m_y;
		const float distance = std::sqrt(dx * dx + dy * dy);
		const float moveDistance = kSpeed * static_cast<float>(deltaTime);

		if (distance < 2.0f)
		{
			enemyPos = m_destination;
			m_IsAtDestination = true;
			m_pAnimationComponent->PauseAnimation();
		}
		else if (distance > 0)
		{
			Vector2 moveDelta(dx / distance * moveDistance, dy / distance * moveDistance);

			if (static_cast<Enemy*>(m_pOwner->GetOwner())->GetCollider()->TryMove(moveDelta))
			{
				enemyPos.m_x += moveDelta.m_x;
				enemyPos.m_y += moveDelta.m_y;

				m_pAnimationComponent->PlayAnimation(m_pSpriteBuffer);
				m_pAnimationComponent->ResumeAnimation();
			}
			else
			{
				// Stop if blocked
				m_IsAtDestination = true;
				m_pAnimationComponent->PauseAnimation();
			}
		}
	}

	// Sync m_pCollider and animation position
	m_pAnimationComponent->SetPosition(enemyPos);
	m_pAnimationComponent->Update(deltaTime);
	m_pOwner->GetOwner()->Move(enemyPos);

}

void EnemyWanderState::Exit()
{

}

void EnemyWanderState::SetRandomDestination()
{
	switch (Direction direction = static_cast<Direction>(m_twister.GenerateRange(0u, 3u)))
	{
	case kUp:
		m_destination = { .m_x = m_pOwner->GetOwner()->GetPosition().m_x, .m_y = m_pOwner->GetOwner()->GetPosition().m_y - kMaxDistance };
		m_pSpriteBuffer = "up"; break;
	case kDown:
		m_destination = { .m_x = m_pOwner->GetOwner()->GetPosition().m_x, .m_y = m_pOwner->GetOwner()->GetPosition().m_y + kMaxDistance };
		m_pSpriteBuffer = "down"; break;
	case kLeft:
		m_destination = { .m_x = m_pOwner->GetOwner()->GetPosition().m_x - kMaxDistance, .m_y = m_pOwner->GetOwner()->GetPosition().m_y };
		m_pSpriteBuffer = "left"; break;
	case kRight:
		m_destination = { .m_x = m_pOwner->GetOwner()->GetPosition().m_x + kMaxDistance, .m_y = m_pOwner->GetOwner()->GetPosition().m_y };
		m_pSpriteBuffer = "right"; break;
	}
}