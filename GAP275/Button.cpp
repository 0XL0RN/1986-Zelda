#include "Button.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"
#include <SDL3/SDL.h>
#include <iostream>

Button::Button(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_box(box)
	, m_isHovering(false)
	, m_isPressed(false)
	, m_isActive(true)
{
}

void Button::Update([[maybe_unused]] const double& deltaTime)
{
	AnimationComponent* animationComponent = static_cast<AnimationComponent*>(GetComponent("Animation"));
	if (!animationComponent)
		return;

	// Update hover state using mouse position
	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	m_isHovering = Intercepts({ static_cast<float>(mouseX), static_cast<float>(mouseY) });

	// Set animation based on state
	if (m_isPressed)
		animationComponent->PlayAnimation("pressed");
	else if (m_isHovering)
		animationComponent->PlayAnimation("hover");
	else if (!m_isActive)
		animationComponent->PlayAnimation("disabled");
	else
		animationComponent->PlayAnimation("unpressed");
}

void Button::Draw(SDL_Renderer* pRenderer)
{
	if (pRenderer && GetComponent("Animation"))
		GetComponent("Animation")->Render(pRenderer);
}

bool Button::HandleEvent(SDL_Event* pEvent)
{
	if (!pEvent)
		return true;
	if (m_isActive)
	{
		AnimationComponent* animationComponent = static_cast<AnimationComponent*>(GetComponent("Animation"));
		if (!animationComponent)
			return false;

		Vector2 mousePos = { static_cast<float>(pEvent->motion.x), static_cast<float>(pEvent->motion.y) };
		switch (pEvent->type)
		{
		case SDL_EVENT_MOUSE_MOTION:
			m_isHovering = Intercepts(mousePos);
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (pEvent->button.button == SDL_BUTTON_LEFT)
			{
				mousePos = { static_cast<float>(pEvent->button.x), static_cast<float>(pEvent->button.y) };
				if (Intercepts(mousePos))
				{
					SoundComponent* pSoundComponent = static_cast<SoundComponent*>(GetComponent("Sound"));
					if (pSoundComponent != nullptr)
						pSoundComponent->Play("click", 0);
					m_isPressed = true;
				}
			}
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (pEvent->button.button == SDL_BUTTON_LEFT)
			{
				mousePos = { static_cast<float>(pEvent->button.x), static_cast<float>(pEvent->button.y) };
				if (Intercepts(mousePos) && m_isPressed)
				{
					if (m_callback)
						m_callback();
					std::cout << "Button clicked!" << std::endl;
				}
				m_isPressed = false;
			}
			break;
		}
	}
	return true;
}

void Button::SetCallback(const std::function<void()>& callback)
{
	m_callback = callback;
}

GameObject* Button::Clone()
{
	return new Button(*this);
}

bool Button::Intercepts(const Vector2& position) const
{
	return (position.m_x >= m_box.x &&
		position.m_x <= m_box.x + m_box.w &&
		position.m_y >= m_box.y &&
		position.m_y <= m_box.y + m_box.h);
}