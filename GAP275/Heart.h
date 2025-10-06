#pragma once

#include "GameObject.h"
#include "Trigger.h"
class Link;

class Heart final : public GameObject
{
private:
	static constexpr double kHealInterval = 1.5;
	static constexpr float kHeal = 5.0f;
public:
	Heart(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	~Heart() override = default;
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	void SetPosition(const Vector2& position) override;
	GameObject* Clone() override;
private:
	Trigger m_trigger;
	double m_healTimer;
	Link* m_pLinkPointer;
};