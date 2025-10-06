#include "BossStateMachine.h"
#include "BossSleepState.h"
#include "BossWanderState.h"
#include "BossChaseState.h"

BossStateMachine::BossStateMachine(GameObject* pOwner)
	: m_pOwner(pOwner)
	, m_pTarget(nullptr)
	, m_pActiveState(nullptr)
	, m_activeStateType(StateType::kDefault)
{
}

BossStateMachine::~BossStateMachine()
{
	delete m_pActiveState;
}

void BossStateMachine::Awake(const StateType& type, GameObject* pTarget)
{
	m_pTarget = pTarget;
	RotateState(type);
}

void BossStateMachine::Update(const double& deltaTime) const
{
	if (m_pActiveState != nullptr)
		m_pActiveState->Update(deltaTime);
}

void BossStateMachine::RotateState(const StateType& type)
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
	case StateType::kSleep:  m_pActiveState = new BossSleepState;   break;
	case StateType::kWander: m_pActiveState = new BossWanderState(this);  break;
	case StateType::kChase: m_pActiveState = new BossChaseState(this, m_pTarget);  break;
	}

	m_activeStateType = type;
	m_pActiveState->Enter();
}
