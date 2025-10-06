#pragma once

#include "GameObject.h"
#include <functional>
class Link;

class Bar final : public GameObject
{
public:
	Bar(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects);

	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	bool HandleEvent(SDL_Event* pEvent) override;
	void SetCallback(const std::function<void(double, Link*, Bar*)>& callback);
	void SetLink(Link* pLink) { m_pLinkPointer = pLink; }
	SDL_FRect* GetBox() const { return m_pBox; }
	GameObject* Clone() override;
private:
	std::function<void(double, Link*, Bar*)> m_callback;
	Link* m_pLinkPointer;
	SDL_FRect* m_pBox;
};