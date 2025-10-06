#pragma once

#include "GameObject.h"
#include <functional>

class Slider final : public GameObject
{
public:
	Slider(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects);

	void Update(const double& deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;
	bool HandleEvent(SDL_Event* pEvent) override;
	void SetCallback(const std::function<void()>& callback);
	float GetValue() const { return m_value; }
	void SetValue(int* pValue) { m_systemValue = pValue; };
	GameObject* Clone() override;
private:
	std::function<void()> m_callback;
	bool m_isPressed;
	float m_value;
	int* m_systemValue;

	[[nodiscard]] bool Intercepts(const Vector2& position) const;
	void ClampValues(const Vector2& mousePos);
};