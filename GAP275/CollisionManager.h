#pragma once

#include "Collider.h"
#include "Trigger.h"
#include "EventQ.h"
#include <vector>

class CollisionManager final
{
private:
	struct TriggerState
	{
		Trigger* m_pTrigger;
		Collider* m_pCollider;
		bool m_isTouching;

		TriggerState(Trigger* pTrigger, Collider* pCollider, const bool state)
			: m_pTrigger(pTrigger)
			, m_pCollider(pCollider)
			, m_isTouching(state)
		{

		}
	};
public:
	CollisionManager(EventQ& eventQ);

	void AddCollider(Collider* pCollider);
	void AddTrigger(Trigger* pTrigger);
	void RemoveCollider(const Collider* pCollider);
	void RemoveTrigger(const Trigger* pTrigger);

	bool UpdateCollider(Collider* pCollider) const;
	void UpdateTrigger(Trigger* pTrigger);

	void Clear();
private:
	std::vector<Collider*> m_colliders;
	std::vector<Trigger*> m_triggers;
	std::vector<TriggerState> m_triggerStates;
	EventQ& m_eventQ;
};