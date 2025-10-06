#pragma once

#include "GameObject.h"
#include "Collider.h"

class StaticImage final : public GameObject
{
public:
	StaticImage(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects);

	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	std::string GetId() override;
	GameObject* Clone() override;
private:

};