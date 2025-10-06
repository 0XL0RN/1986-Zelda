#include "BossChaseState.h"
#include "BossStateMachine.h"
#include "AnimationComponent.h"
#include "Boss.h"

BossChaseState::BossChaseState(BossStateMachine* pOwner, GameObject* pTarget)
	: m_pOwner(pOwner)
	, m_pTarget(pTarget)
	, m_pAnimationComponent(nullptr)
{
}

BossChaseState::~BossChaseState()
{

}

void BossChaseState::Enter()
{
	m_pAnimationComponent = static_cast<AnimationComponent*>(m_pOwner->GetOwner()->GetComponent("Animation"));
	m_pAnimationComponent->PlayAnimation("chase");
	m_pAnimationComponent->ResumeAnimation();

	Boss* pBoss = static_cast<Boss*>(m_pOwner->GetOwner());

	pBoss->GetCollider()[1].m_rect.~Collider();
	pBoss->GetCollider()[2].m_rect.~Collider();
	pBoss->GetCollider()[3].m_rect.~Collider();

	pBoss->GetCollider()[0].m_rect.GetBox().GetBounds().h = 60;
	pBoss->GetCollider()[0].m_rect.GetBox().GetBounds().w = 60;
	pBoss->GetCollider()[0].m_offset = { 18, 18 };

	pBoss->SetDmg(-6);
}

void BossChaseState::Update(const double& deltaTime)
{
	Vector2 pathToTarget = m_pTarget->GetPosition() - m_pOwner->GetOwner()->GetPosition();
	pathToTarget = pathToTarget.GetNormalized();
	const float moveDistance = kSpeed * static_cast<float>(deltaTime);

	pathToTarget = pathToTarget * moveDistance;

	Vector2 enemyPos{ 0,0 };

	Boss* pBoss = static_cast<Boss*>(m_pOwner->GetOwner());
	bool canMove = true;

	if (!pBoss->GetCollider()[0].m_rect.TryMove(pathToTarget))
		canMove = false;

	if (canMove)
	{
		enemyPos.m_x = pathToTarget.m_x + m_pOwner->GetOwner()->GetPosition().m_x;
		enemyPos.m_y = pathToTarget.m_y + m_pOwner->GetOwner()->GetPosition().m_y;

		// Sync m_pCollider and animation position
		m_pOwner->GetOwner()->Move(enemyPos);
		m_pAnimationComponent->SetPosition(enemyPos);
		m_pAnimationComponent->Update(deltaTime);
	}
}

void BossChaseState::Exit()
{

}
