#pragma once

#include "GameObject.h"
#include <functional>
class CollisionManager;

class Button final : public GameObject
{
public:
	Button(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, std::vector<GameObject*>& objects);

	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	bool HandleEvent(SDL_Event* pEvent) override;
	void SetCallback(const std::function<void()>& callback);
	void Activate() { m_isActive = true; }
	void Deactivate() { m_isActive = false; }
	GameObject* Clone() override;
private:
	SDL_FRect m_box;
	std::function<void()> m_callback;
	bool m_isHovering;
	bool m_isPressed;
	bool m_isActive;

	[[nodiscard]] bool Intercepts(const Vector2& position) const;
};