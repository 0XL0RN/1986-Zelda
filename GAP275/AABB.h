#pragma once

#include <SDL3/SDL.h>
#include "Vector2.h"
class GameObject;

class AABB final
{
private:
	using Vector2 = Vector2<float>;
public:
	AABB(const SDL_FRect& bounds, GameObject* pObject);
	bool Intersects(const AABB& other) const;
	SDL_FRect& GetBounds() { return m_bounds; }
	void SetPosition(const Vector2& position);
	GameObject* GetOwner() const { return m_pOwner; }
private:
	SDL_FRect m_bounds;
	GameObject* m_pOwner;
};