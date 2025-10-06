#pragma once

#include "AABB.h"
class CollisionManager;

class Collider final
{
public:
	using Vector2 = Vector2<float>;
	Collider(const SDL_FRect& bounds, GameObject* pObject, CollisionManager* pManager);
	~Collider();
	bool TryMove(const Vector2& deltaPosition);
	void SetPosition(const Vector2& position);
	SDL_FRect& GetBounds() { return m_box.GetBounds(); }
	GameObject* GetOwner() const { return m_box.GetOwner(); }
	AABB& GetBox() { return m_box; }
	void DeActivate() { m_active = false; }
private:
	AABB m_box;
	CollisionManager* m_pManager;
	bool m_active;
};