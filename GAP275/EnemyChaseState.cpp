#include "EnemyChaseState.h"
#include "EnemyStateMachine.h"
#include "AnimationComponent.h"
#include "Enemy.h"

EnemyChaseState::EnemyChaseState(EnemyStateMachine* pOwner, GameObject* pTarget)
	: m_pOwner(pOwner)
	, m_pTarget(pTarget)
	, m_pAnimationComponent(nullptr)
	, m_oldPos(pTarget->GetPosition())
{
}

EnemyChaseState::~EnemyChaseState()
{

}

void EnemyChaseState::Enter()
{
	m_pAnimationComponent = static_cast<AnimationComponent*>(m_pOwner->GetOwner()->GetComponent("Animation"));
}

void EnemyChaseState::Update(const double& deltaTime)
{
	Vector2 pathToTarget = m_pTarget->GetPosition() - m_pOwner->GetOwner()->GetPosition();
	pathToTarget = pathToTarget.GetNormalized();
	const float moveDistance = kSpeed * static_cast<float>(deltaTime);

	pathToTarget = pathToTarget * moveDistance;

	Vector2 enemyPos{ 0,0 };
	if (static_cast<Enemy*>(m_pOwner->GetOwner())->GetCollider()->TryMove(pathToTarget))
	{
		enemyPos.m_x = pathToTarget.m_x + m_pOwner->GetOwner()->GetPosition().m_x;
		enemyPos.m_y = pathToTarget.m_y + m_pOwner->GetOwner()->GetPosition().m_y;


		UpdateSpriteCardinal(m_oldPos, enemyPos);

		m_oldPos = enemyPos;


		// Sync m_pCollider and animation position
		m_pOwner->GetOwner()->Move(enemyPos);
		m_pAnimationComponent->SetPosition(enemyPos);
		m_pAnimationComponent->Update(deltaTime);

	}
}

void EnemyChaseState::Exit()
{

}

void EnemyChaseState::UpdateSpriteCardinal(const Vector2& oldPos, const Vector2& newPos)
{
	if (newPos.m_x > oldPos.m_x)
		m_pAnimationComponent->PlayAnimation("right");
	else if (newPos.m_x < oldPos.m_x)
		m_pAnimationComponent->PlayAnimation("left");
	/*
	else if (newPos.m_y > oldPos.m_y)
		m_pAnimationComponent->PlayAnimation("down");
	else if (newPos.m_y < oldPos.m_y)
		m_pAnimationComponent->PlayAnimation("up");
		*/

	m_pAnimationComponent->ResumeAnimation();
}
