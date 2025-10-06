#pragma once

#include "AABB.h"
#include "Vector2.h"
class CollisionManager;

class Trigger final
{
public:
	using Vector2 = Vector2<float>;
	Trigger(const SDL_FRect& bounds, GameObject* pObject, CollisionManager* pManager);
	~Trigger();
	void TryTrigger();
	AABB& GetBox() { return m_box; }
	void Sleep() { m_active = false; }
	bool IsActive() const { return m_active; }
	void SetPosition(const Vector2& position);
private:
	AABB m_box;
	CollisionManager* m_pManager;
	bool m_active;
};