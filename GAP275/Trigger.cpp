#include "Trigger.h"
#include "CollisionManager.h"

Trigger::Trigger(const SDL_FRect& bounds, GameObject* pObject, CollisionManager* pManager)
	: m_box(bounds, pObject)
	, m_pManager(pManager)
	, m_active(true)
{
	m_pManager->AddTrigger(this);
}

Trigger::~Trigger()
{
	m_pManager->RemoveTrigger(this);
}

void Trigger::TryTrigger()
{
	if (!m_pManager)
		return;

	if (m_active == true)
		m_pManager->UpdateTrigger(this);
}

void Trigger::SetPosition(const Vector2& position)
{
	m_box.SetPosition(position);
}