#pragma once

#include "GameObject.h"
#include "Collider.h"

class Block final : public GameObject
{
public:
	Block(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	void SetPosition(const Vector2& position) override;
	GameObject* Clone() override;
private:
	Collider m_collider;
};