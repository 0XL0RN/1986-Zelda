#include "Plate.h"
#include <iostream>
#include "Trap.h"
#include "AnimationComponent.h"

Plate::Plate(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_trigger(box, this, pManager)
	, m_IsSteppedOn(false)
{
	RegisterEvent(EventType::kTriggerEnter, [this](const Event& event) -> void
		{

			if (event.m_sender == &m_trigger)
			{
				GameObject* pObject = static_cast<GameObject*>(event.m_receiver);
				if (pObject && pObject->GetId() == "Link")
				{
					m_IsSteppedOn = true;
					std::cout << "Plate pressed\n";
				}
			}
		});

	RegisterEvent(EventType::kTriggerExit, [this](const Event& event) -> void
		{
			if (event.m_sender == &m_trigger)
			{
				GameObject* pObject = static_cast<GameObject*>(event.m_receiver);
				if (pObject && pObject->GetId() == "Link")
				{
					m_IsSteppedOn = false;
					std::cout << "Plate released\n";
				}
			}
		});
}

void Plate::Update([[maybe_unused]] const double& deltaTime)
{
	AnimationComponent* pAnimation = static_cast<AnimationComponent*>(GetComponent("Animation"));
	if (!pAnimation)
		return;

	m_trigger.TryTrigger();

	if (m_IsSteppedOn)
		pAnimation->PlayAnimation("pressed");
	else
		pAnimation->PlayAnimation("unpressed");
}

void Plate::Draw(SDL_Renderer* pRenderer)
{
	if (GetComponent("Animation"))
		GetComponent("Animation")->Render(pRenderer);
}

GameObject* Plate::Clone()
{
	return new Plate(*this);
}
