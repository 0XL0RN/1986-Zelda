#pragma once

class GameObject;
class AiState;

class BossStateMachine final
{
public:
	enum class StateType
	{
		kSleep,
		kWander,
		kChase,
		kDefault
	};

	explicit BossStateMachine(GameObject* pOwner);
	~BossStateMachine();

	void Awake(const StateType& type, GameObject* pTarget);
	void Update(const double& deltaTime) const;
	void RotateState(const StateType& type);

	GameObject* GetOwner() { return m_pOwner; }
private:
	GameObject* m_pOwner;
	GameObject* m_pTarget;
	AiState* m_pActiveState;
	StateType m_activeStateType;
};