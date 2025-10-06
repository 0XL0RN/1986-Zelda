#pragma once

#include "GameObject.h"
#include "Trigger.h"
class Link;

class Fire final : public GameObject
{
private:
	static constexpr double kDmgInterval = 1.0;
	static constexpr float kDmg = 2.0f;
public:
	Fire(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	~Fire() override = default;
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	void SetPosition(const Vector2& position) override;
	GameObject* Clone() override;
private:
	Trigger m_trigger;
	double m_damageTimer;
	Link* m_pLinkPointer;
};