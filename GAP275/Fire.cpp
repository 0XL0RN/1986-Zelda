#include "Fire.h"
#include <iostream>
#include "SceneStateMachine.h"
#include "AnimationComponent.h"
#include "ParticleComponent.h"
#include "Link.h"

Fire::Fire(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_trigger(box, this, pManager)
	, m_damageTimer(0.0)
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
					m_damageTimer = 0.0;
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

void Fire::Update(const double& deltaTime)
{
	m_trigger.TryTrigger();

	// Apply damage if Link is in the fire
	if (m_pLinkPointer)
	{
		m_damageTimer += deltaTime;
		if (m_damageTimer >= kDmgInterval)
		{
			m_pLinkPointer->SetHealth(-2);

			if (m_pLinkPointer->IsAlive() == false)
				SceneStateMachine::SetScene(SceneStateMachine::SceneType::kLoseScene);
			m_damageTimer = 0.0;
		}
	}

	if (IComponent* pTexture = GetComponent("Animation"))
		pTexture->Update(deltaTime);

	ParticleComponent* pParticle = static_cast<ParticleComponent*>(GetComponent("Particle"));
	pParticle->Splash();
	pParticle->Update(deltaTime);
}

void Fire::Draw(SDL_Renderer* pRenderer)
{
	if (IComponent* pParticle = GetComponent("Particle"))
		pParticle->Render(pRenderer);

	if (IComponent* pTexture = GetComponent("Animation"))
		pTexture->Render(pRenderer);
}

void Fire::SetPosition(const Vector2& position)
{
	AnimationComponent* pAnimation = static_cast<AnimationComponent*>(GetComponent("Animation"));
	ParticleComponent* pParticle = static_cast<ParticleComponent*>(GetComponent("Particle"));
	m_position = position;
	m_trigger.SetPosition(position);
	pAnimation->SetPosition(position);
	Vector2 offsetPosition = { (m_trigger.GetBox().GetBounds().h / 2) + position.m_x, (m_trigger.GetBox().GetBounds().w / 2) + position.m_y };
	pParticle->SetPosition(offsetPosition);
}

GameObject* Fire::Clone()
{
	return new Fire(*this);
}
