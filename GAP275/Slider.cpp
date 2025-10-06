#include "Slider.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include <SDL3/SDL.h>
#include <string>

Slider::Slider(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_isPressed(false)
	, m_value(10.0f)
	, m_systemValue(nullptr)
{
}

void Slider::Update([[maybe_unused]] const double& deltaTime)
{
}

void Slider::Draw(SDL_Renderer* pRenderer)
{
	if (!pRenderer)
		return;

	if (auto* sliderComp = static_cast<TextureComponent*>(GetComponent("Slider")))
		sliderComp->Render(pRenderer);

	if (auto* dotComp = static_cast<TextureComponent*>(GetComponent("Dot")))
		dotComp->Render(pRenderer);

	if (auto* textComp = static_cast<TextComponent*>(GetComponent("Text")))
		textComp->Render(pRenderer);
}

bool Slider::HandleEvent(SDL_Event* pEvent)
{
	if (!pEvent) return false;

	Vector2 mousePos;
	// Use button.x/y for clicks, motion.x/y for drags
	if (pEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN || pEvent->type == SDL_EVENT_MOUSE_BUTTON_UP)
		mousePos = { .m_x = static_cast<float>(pEvent->button.x), .m_y = static_cast<float>(pEvent->button.y) };
	else
		mousePos = { .m_x = static_cast<float>(pEvent->motion.x), .m_y = static_cast<float>(pEvent->motion.y) };

	TextureComponent* dotComp = static_cast<TextureComponent*>(GetComponent("Dot"));
	if (!dotComp)
		return false;

	switch (pEvent->type)
	{
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		if (pEvent->button.button == SDL_BUTTON_LEFT && Intercepts(mousePos))
		{
			m_isPressed = true;
			ClampValues(mousePos); // Move dot to click position
			return true;
		}
		break;

	case SDL_EVENT_MOUSE_MOTION:
		if (m_isPressed)
		{
			ClampValues(mousePos);
			return true;
		}
		break;

	case SDL_EVENT_MOUSE_BUTTON_UP:
		if (pEvent->button.button == SDL_BUTTON_LEFT)
		{
			m_isPressed = false;
			if (m_callback) m_callback();
			return true;
		}
		break;

	default:
		break;
	}

	return false;
}

void Slider::SetCallback(const std::function<void()>& callback)
{
	m_callback = callback;
}

GameObject* Slider::Clone()
{
	return new Slider(*this);
}

bool Slider::Intercepts(const Vector2& position) const
{
	TextureComponent* dotComp = static_cast<TextureComponent*>(GetComponent("Dot"));
	if (!dotComp)
		return false;

	SDL_FRect dot = dotComp->GetPosition();
	return (position.m_x >= dot.x &&
		position.m_x <= dot.x + dot.w &&
		position.m_y >= dot.y &&
		position.m_y <= dot.y + dot.h);
}

void Slider::ClampValues(const Vector2& mousePos)
{
	TextureComponent* sliderComp = static_cast<TextureComponent*>(GetComponent("Slider"));
	TextureComponent* dotComp = static_cast<TextureComponent*>(GetComponent("Dot"));

	// Get slider rectangle
	SDL_FRect sliderRect = sliderComp->GetPosition();

	// Calculate dot width for proper positioning
	float dotWidth = dotComp->GetPosition().w;

	// Calculate bounds for dot movement
	float minX = sliderRect.x;
	float maxX = sliderRect.x + sliderRect.w - dotWidth;

	// Clamp mouse X position to slider bounds
	float newX = std::max(minX, std::min(mousePos.m_x - dotWidth / 2, maxX));

	// Update dot position
	Vector2 newDotPos = { newX, dotComp->GetPosition().y };
	dotComp->SetPosition(newDotPos);

	float sliderRange = maxX - minX;
	if (sliderRange > 0)
		m_value = (newX - minX) / sliderRange;
	else
		m_value = 0.0f;

	int scaledValue = static_cast<int>(m_value * 10);
	std::string text = std::to_string(scaledValue);
	if (auto* textComp = static_cast<TextComponent*>(GetComponent("Text")))
		textComp->GenerateText(text.c_str());

	// Calculate m_value in the range 0-100
	if (sliderRange > 0)
		m_value = ((newX - minX) / sliderRange) * 100.0f;
	else
		m_value = 0.0f;

	// Explicitly clamp m_value to 0-100
	m_value = std::max(0.0f, std::min(100.0f, m_value));

	// Update system value
	if (m_systemValue)
		*m_systemValue = static_cast<int>(m_value);

}