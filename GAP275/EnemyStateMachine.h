#pragma once

class AiState;
class GameObject;

class EnemyStateMachine final
{
public:
	enum class StateType
	{
		kChase,
		kWander,
		kDefault,
	};

	EnemyStateMachine(GameObject* pOwner);
	~EnemyStateMachine();
	void Awake(const StateType type, GameObject* pTarget);
	void Update(const double& deltaTime);
	void RotateState(const StateType& type);
	GameObject* GetOwner() { return m_pOwner; }

private:
	GameObject* m_pOwner;
	GameObject* m_pTarget;
	AiState* m_pActiveState;
	StateType m_activeStateType;
};