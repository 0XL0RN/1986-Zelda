#include "AABB.h"

AABB::AABB(const SDL_FRect& bounds, GameObject* pObject)
	: m_bounds(bounds)
	, m_pOwner(pObject)
{
}

bool AABB::Intersects(const AABB& other) const
{
	const SDL_FRect& bound = other.m_bounds;

	// Early out checks using SIMD-friendly comparisons
	if (m_bounds.x + m_bounds.w <= bound.x)
		return false;					// This is left of other
	if (m_bounds.x >= bound.x + bound.w)
		return false;					// This is right of other
	if (m_bounds.y + m_bounds.h <= bound.y)
		return false;					// This is above other
	if (m_bounds.y >= bound.y + bound.h)
		return false;					// This is below other

	return true;
}

void AABB::SetPosition(const Vector2& position)
{
	m_bounds.x = position.m_x;
	m_bounds.y = position.m_y;
}