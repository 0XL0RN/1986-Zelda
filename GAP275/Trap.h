#pragma once

#include "GameObject.h"
#include "Collider.h"

class Trap : public GameObject
{
private:
	static constexpr float kEndX = 640.0f;
public:
	Trap(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, GameObject* pPlate, std::vector<GameObject*>& objects);
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	void SetPosition(const Vector2& position);
	void Move(const Vector2& deltaPos);
	GameObject* Clone() override;

private:
	Collider m_collider;
	bool m_isActive;
	GameObject* m_pPlate;
	Vector2 m_originalPosition;
	Vector2 m_startPosition;
	bool m_movingRight;
};