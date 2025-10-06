#include "Heart.h"
#include "TextureComponent.h"
#include "ParticleComponent.h"
#include <iostream>
#include "Link.h"

Heart::Heart(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_trigger(box, this, pManager)
	, m_healTimer(0.0)
	, m_pLinkPointer(nullptr)
{
	RegisterEvent(EventType::kTriggerEnter, [this](const Event& event) -> void
		{
			if (event.m_sender == &m_trigger)
			{
				GameObject* pObject = static_cast<GameObject*>(event.m_receiver);
				if (pObject && pObject->GetId() == "Link")
				{
					m_pLinkPointer = static_cast<Link*>(pObject);
					std::cout << m_pLinkPointer->GetId() << " is on " << GetId() << "\n";
					m_healTimer = 0.0;
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
					m_pLinkPointer = nullptr;
					std::cout << "Link is no longer on " << GetId() << "\n";
				}
			}
		});
}

void Heart::Update(const double& deltaTime)
{
	m_trigger.TryTrigger();

	// Apply heals if Link is on heart
	if (m_pLinkPointer)
	{
		m_healTimer += deltaTime;
		if (m_healTimer >= kHealInterval)
		{
			m_pLinkPointer->SetHealth(m_pLinkPointer->GetHealth() + kHeal);
			m_healTimer = 0.0;
		}
	}

	if (IComponent* texture = GetComponent("Texture"))
		texture->Update(deltaTime);

	ParticleComponent* pParticle = static_cast<ParticleComponent*>(GetComponent("Particle"));
	pParticle->Splash();
	pParticle->Update(deltaTime);
}


void Heart::Draw(SDL_Renderer* pRenderer)
{
	if (IComponent* pParticle = GetComponent("Particle"))
		pParticle->Render(pRenderer);

	if (IComponent* texture = GetComponent("Texture"))
		texture->Render(pRenderer);
}

void Heart::SetPosition(const Vector2& position)
{
	TextureComponent* pTextureComponent = static_cast<TextureComponent*>(GetComponent("Texture"));
	ParticleComponent* pParticle = static_cast<ParticleComponent*>(GetComponent("Particle"));
	m_position = position;
	m_trigger.SetPosition(position);
	pTextureComponent->SetPosition(position);
	Vector2 offsetPosition = { (m_trigger.GetBox().GetBounds().h / 2) + position.m_x, (m_trigger.GetBox().GetBounds().w / 2) + position.m_y };
	pParticle->SetPosition(offsetPosition);
}


GameObject* Heart::Clone()
{
	return new Heart(*this);
}