#pragma once

#include "GameObject.h"
#include "Trigger.h"
class Link;

class Triforce final : public GameObject
{
public:
	Triforce(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects);
	~Triforce() override = default;
	void Awake() override;
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	void SetPosition(const Vector2& position) override;
	GameObject* Clone() override;
private:
	Trigger m_trigger;
	Link* m_pLink;
	float m_beepTimer;
	float m_maxDistance;
	float m_minDistance;

	float CalculateDistance(const Vector2& linkPosition) const;
};