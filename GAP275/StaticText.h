#pragma once

#include "GameObject.h"
#include <functional>

class StaticText final : public GameObject
{
public:
	StaticText(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects);
	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	void SetCallback(const std::function<void(const char* text)>& callback);
	GameObject* Clone() override;
private:
	std::function<void(const char* text)> m_callback;
};