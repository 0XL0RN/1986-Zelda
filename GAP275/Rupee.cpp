#include "Rupee.h"
#include <iostream>
#include "AnimationComponent.h"

Rupee::Rupee(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_trigger(box, this, pManager)
	, m_collected(false)
{
	RegisterEvent(EventType::kTriggerEnter, [this](const Event& event) -> void
		{
			if (event.m_sender == &m_trigger)
			{
				GameObject* collector = static_cast<GameObject*>(event.m_receiver);
				if (collector->GetId() == "Link")
				{
					m_collected = true;
					m_trigger.Sleep();
					std::cout << collector->GetId() << " collected Rupee\n";
				}
			}
		});
}

void Rupee::Update(const double& deltaTime)
{
	if (!m_collected)
	{
		m_trigger.TryTrigger();

		if (GetComponent("Animation") != nullptr)
			GetComponent("Animation")->Update(deltaTime);
	}
}

void Rupee::Draw(SDL_Renderer* pRenderer)
{
	if (!m_collected)
		GetComponent("Animation")->Render(pRenderer);
}

void Rupee::SetPosition(const Vector2& position)
{
	AnimationComponent* pAnimation = static_cast<AnimationComponent*>(GetComponent("Animation"));
	m_position = position;
	m_trigger.SetPosition(position);
	pAnimation->SetPosition(position);
}

GameObject* Rupee::Clone()
{
	return new Rupee(*this);
}