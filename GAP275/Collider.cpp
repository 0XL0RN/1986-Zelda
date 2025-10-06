#include "Collider.h"
#include "CollisionManager.h"

Collider::Collider(const SDL_FRect& bounds, GameObject* pObject, CollisionManager* pManager)
	: m_box(bounds, pObject)
	, m_pManager(pManager)
	, m_active(true)
{
	m_pManager->AddCollider(this);
}

Collider::~Collider()
{
	m_pManager->RemoveCollider(this);
}

bool Collider::TryMove(const Vector2& deltaPosition)
{
	if (m_pManager == nullptr)
		return false;

	// Store original position
	Vector2 originalPos(m_box.GetBounds().x, m_box.GetBounds().y);

	// Attempt move
	m_box.SetPosition(Vector2(originalPos.m_x + deltaPosition.m_x, originalPos.m_y + deltaPosition.m_y));
	const bool didCollide = m_pManager->UpdateCollider(this);

	if (didCollide)
		m_box.SetPosition(originalPos); // Revert

	return !didCollide;
}

void Collider::SetPosition(const Vector2& position)
{
	m_box.SetPosition(position);
}
