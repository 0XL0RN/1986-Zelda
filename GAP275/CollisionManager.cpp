#include "CollisionManager.h"

CollisionManager::CollisionManager(EventQ& eventQ)
	: m_eventQ(eventQ)
{

}

void CollisionManager::AddCollider(Collider* pCollider)
{
	if (pCollider)
		m_colliders.push_back(pCollider);

}

void CollisionManager::AddTrigger(Trigger* pTrigger)
{
	if (pTrigger)
		m_triggers.push_back(pTrigger);

}

void CollisionManager::RemoveCollider(const Collider* pCollider)
{
	// Remove Collider
	for (size_t i = 0; i < m_colliders.size(); ++i)
	{
		if (m_colliders[i] == pCollider)
		{
			m_colliders[i] = m_colliders.back();
			m_colliders.pop_back();
			break;
		}
	}

	// Remove State
	for (size_t j = 0; j < m_triggerStates.size(); ++j)
	{
		if (m_triggerStates[j].m_pCollider == pCollider)
		{
			m_triggerStates[j] = m_triggerStates.back();
			m_triggerStates.pop_back();
		}
	}
}

void CollisionManager::RemoveTrigger(const Trigger* pTrigger)
{
	// Remove Trigger
	for (size_t i = 0; i < m_triggers.size(); ++i)
	{
		if (m_triggers[i] == pTrigger)
		{
			m_triggers[i] = m_triggers.back();
			m_triggers.pop_back();
			break;
		}
	}

	// Remove State
	for (size_t j = 0; j < m_triggerStates.size(); ++j)
	{
		if (m_triggerStates[j].m_pTrigger == pTrigger)
		{
			m_triggerStates[j] = m_triggerStates.back();
			m_triggerStates.pop_back();
		}
	}
}

bool CollisionManager::UpdateCollider(Collider* pCollider) const
{
	bool didCollide = false;

	for (Collider* pOther : m_colliders)
	{
		// If same continue
		if (pCollider == pOther)
			continue;

		// Check collision
		if (pCollider->GetBox().Intersects(pOther->GetBox()))
		{
			didCollide = true;

			// Q event
			m_eventQ.QueueEvent(EventType::kCollision, pCollider, pOther->GetOwner());
			m_eventQ.QueueEvent(EventType::kCollision, pOther, pCollider->GetOwner());
		}
	}

	return didCollide;
}

void CollisionManager::UpdateTrigger(Trigger* pTrigger)
{
	for (Collider* pCollider : m_colliders)
	{
		// Check collision
		const bool isTouching = pTrigger->GetBox().Intersects(pCollider->GetBox());

		// Search for existing state
		bool wasTouching = false;
		size_t stateIndex = 0;
		bool found = false;

		for (size_t i = 0; i < m_triggerStates.size(); ++i)
		{
			if (m_triggerStates[i].m_pTrigger == pTrigger && m_triggerStates[i].m_pCollider == pCollider)
			{
				wasTouching = m_triggerStates[i].m_isTouching;
				stateIndex = i;
				found = true;
				break;	
			}
		}

		// Exit & Enter Trigger Handler
		if (isTouching && !wasTouching)
		{
			if (pTrigger->GetBox().GetOwner() && pCollider->GetOwner())
			{
				// Event Handle Entering Trigger
				m_eventQ.QueueEvent(EventType::kTriggerEnter, pTrigger, pCollider->GetOwner());

				if (!found)
					m_triggerStates.push_back(TriggerState(pTrigger, pCollider, true));
				else
					m_triggerStates[stateIndex].m_isTouching = true;
			}
		}
		else if (!isTouching && wasTouching)
		{
			// Event Handle Exiting Trigger
			if (pTrigger->GetBox().GetOwner() && pCollider->GetOwner())
			{
				m_eventQ.QueueEvent(EventType::kTriggerExit, pTrigger, pCollider->GetOwner());
				m_triggerStates[stateIndex].m_isTouching = false;
			}
		}
	}
}

void CollisionManager::Clear()
{
	m_colliders.clear();
	m_triggerStates.clear();
	m_triggerStates.clear();
}
