#include "Enemy.h"
#include "AnimationComponent.h"
#include "CollisionManager.h"
#include <iostream>

Enemy::Enemy(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_stateMachine(this)
	, m_collider(box, this, pManager)
	, m_pTarget(nullptr)
{
}

void Enemy::Awake()
{
	m_pTarget = GetObject("Link");
	m_stateMachine.Awake(EnemyStateMachine::StateType::kChase, m_pTarget);
}

void Enemy::Update(const double& deltaTime)
{
	Vector2 pathToTarget = m_pTarget->GetPosition() - this->GetPosition();
	if (pathToTarget.GetLength() <= kDetectSquare)
		m_stateMachine.RotateState(EnemyStateMachine::StateType::kChase);
	else
		m_stateMachine.RotateState(EnemyStateMachine::StateType::kWander);

	m_stateMachine.Update(deltaTime);
}

void Enemy::Draw(SDL_Renderer* pRenderer)
{
	if (IComponent* animation = GetComponent("Animation"))
		animation->Render(pRenderer);
}

bool Enemy::Move(const Vector2& direction, [[maybe_unused]] double speed)
{
	m_position = direction;
	m_collider.SetPosition(m_position);
	return true;
}

void Enemy::SetPosition(const Vector2& position)
{
	AnimationComponent* pAnimation = static_cast<AnimationComponent*>(GetComponent("Animation"));
	m_position = position;
	m_collider.SetPosition(position);
	pAnimation->SetPosition(position);
}


GameObject* Enemy::Clone()
{
	return new Enemy(*this);
}
