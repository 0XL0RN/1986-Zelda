#pragma once

#include "GameObject.h"
#include "Trigger.h"
class CollisionManager;

class Rupee final : public GameObject
{
public:
	Rupee(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	~Rupee() override = default;
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	void SetPosition(const Vector2& position) override;
	GameObject* Clone() override;
private:
	Trigger m_trigger;
	bool m_collected;
};