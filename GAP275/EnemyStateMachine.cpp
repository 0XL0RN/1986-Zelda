#include "EnemyStateMachine.h"
#include "EnemyChaseState.h"
#include "EnemyWanderState.h"
#include "GameObject.h"

EnemyStateMachine::EnemyStateMachine(GameObject* pOwner)
	: m_pOwner(pOwner)
	, m_pTarget(nullptr)
	, m_pActiveState(nullptr)
	, m_activeStateType(StateType::kDefault)
{
}

EnemyStateMachine::~EnemyStateMachine()
{
	delete m_pActiveState;
}

void EnemyStateMachine::Awake(const StateType type, GameObject* pTarget)
{
	m_pTarget = pTarget;
	RotateState(type);
}

void EnemyStateMachine::Update([[maybe_unused]] const double& deltaTime)
{


	if (m_pActiveState != nullptr)
		m_pActiveState->Update(deltaTime);
}

void EnemyStateMachine::RotateState(const StateType& type)
{
	if (type == m_activeStateType)
		return;

	if (m_pActiveState != nullptr)
	{
		m_pActiveState->Exit();
		delete m_pActiveState;
		m_pActiveState = nullptr;
	}

	switch (type)
	{
	case StateType::kChase:  m_pActiveState = new EnemyChaseState(this, m_pTarget);   break;
	case StateType::kWander: m_pActiveState = new EnemyWanderState(this, m_pTarget);  break;
	}

	m_activeStateType = type;
	m_pActiveState->Enter();
}
