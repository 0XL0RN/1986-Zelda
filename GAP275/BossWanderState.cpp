#include "BossWanderState.h"
#include "BossStateMachine.h"
#include "AnimationComponent.h"
#include "Boss.h"

BossWanderState::BossWanderState(BossStateMachine* pOwner)
	: m_pOwner(pOwner)
	, m_pAnimationComponent(nullptr)
	, m_destination(pOwner->GetOwner()->GetPosition())
	, m_IsAtDestination(false)
	, m_waitTimer(0.0)
	, m_pSpriteBuffer("sleep")
{
}

BossWanderState::~BossWanderState()
{

}

void BossWanderState::Enter()
{
	m_pAnimationComponent = static_cast<AnimationComponent*>(m_pOwner->GetOwner()->GetComponent("Animation"));
	static_cast<Boss*>(m_pOwner->GetOwner())->SetDmg(-3);
}

void BossWanderState::Update(const double& deltaTime)
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

			Boss* pBoss = static_cast<Boss*>(m_pOwner->GetOwner());
			bool canMove = true;

			// Check if the proposed movement is valid for all colliders
			for (size_t i = 0; i < 4; ++i)
			{
				if (!pBoss->GetCollider()[i].m_rect.TryMove(moveDelta))
				{
					canMove = false;
					break;
				}
			}
			if (canMove)
			{
				// Update position
				enemyPos.m_x += moveDelta.m_x;
				enemyPos.m_y += moveDelta.m_y;

				// Move the boss and update hitboxes
				pBoss->Move(enemyPos, 1);

				m_pAnimationComponent->PlayAnimation(m_pSpriteBuffer);
				m_pAnimationComponent->ResumeAnimation();
			}
			else
			{
				// If blocked, try a new destination
				/*m_IsAtDestination = true;*/
				SetRandomDestination();
				m_pAnimationComponent->PauseAnimation();
			}
		}
		// Sync animation position
		m_pOwner->GetOwner()->SetPosition(enemyPos);
		m_pAnimationComponent->Update(deltaTime);
	}
}

void BossWanderState::Exit()
{

}

void BossWanderState::SetRandomDestination()
{
	switch (Direction direction = static_cast<Direction>(m_twister.GenerateRange(0u, 3u)))
	{
	case kUp:
		m_destination = { .m_x = m_pOwner->GetOwner()->GetPosition().m_x, .m_y = m_pOwner->GetOwner()->GetPosition().m_y - kMaxDistance };
		m_pSpriteBuffer = "up"; break; //up
	case kDown:
		m_destination = { .m_x = m_pOwner->GetOwner()->GetPosition().m_x, .m_y = m_pOwner->GetOwner()->GetPosition().m_y + kMaxDistance };
		m_pSpriteBuffer = "down"; break; //down
	case kLeft:
		m_destination = { .m_x = m_pOwner->GetOwner()->GetPosition().m_x - kMaxDistance, .m_y = m_pOwner->GetOwner()->GetPosition().m_y };
		m_pSpriteBuffer = "left"; break; //left
	case kRight:
		m_destination = { .m_x = m_pOwner->GetOwner()->GetPosition().m_x + kMaxDistance, .m_y = m_pOwner->GetOwner()->GetPosition().m_y };
		m_pSpriteBuffer = "right"; break; //right
	}
}