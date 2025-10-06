#pragma once

#include "GameObject.h"
#include "Trigger.h"
class CollisionManager;
class Trap;

class Plate final : public GameObject
{
public:
	Plate(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	~Plate() override = default;
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	GameObject* Clone() override;
	bool IsSteppedOn() const { return m_IsSteppedOn; }

	Trigger* GetTrigger() { return &m_trigger; }
private:
	Trigger m_trigger;
	bool m_IsSteppedOn;
};