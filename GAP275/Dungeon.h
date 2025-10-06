#pragma once

#include "GameObject.h"
#include "Collider.h"

// Dungeon class for handling background
class Dungeon final : public GameObject
{
public:
	Dungeon(const Vector2& position, const int& depth, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	~Dungeon() override = default;
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	GameObject* Clone() override;
private:
	Collider m_wallOne;
	Collider m_wallTwo;
	Collider m_wallThree;
	Collider m_wallFour;
};