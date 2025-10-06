#include "Triforce.h"
#include "SceneStateMachine.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"
#include "ParticleComponent.h"
#include "Link.h"
#include <iostream>

Triforce::Triforce(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_trigger(box, this, pManager)
	, m_pLink(nullptr)
	, m_beepTimer(0.0f)
	, m_maxDistance(400.0f)
	, m_minDistance(30.0f)
{

	RegisterEvent(EventType::kTriggerEnter, [this](const Event& event) -> void
		{
			if (event.m_sender == &m_trigger)
			{
				GameObject* pObject = static_cast<GameObject*>(event.m_receiver);
				if (pObject && pObject->GetId() == "Link")
				{
					SoundComponent* pSoundComponent = static_cast<SoundComponent*>(GetComponent("Sound"));
					if (pSoundComponent)
						pSoundComponent->Play("pulse", 0);

					SceneStateMachine::SetScene(SceneStateMachine::SceneType::kWinScene);
					std::cout << "You Win!\n";
				}
			}
		});
}

void Triforce::Awake()
{
	m_pLink = static_cast<Link*>(GetObject("Link"));
}


void Triforce::Update(const double& deltaTime)
{
	m_beepTimer += static_cast<float>(deltaTime);

	if (m_beepTimer >= 2.5f && m_pLink)
	{
		SoundComponent* pSoundComponent = static_cast<SoundComponent*>(GetComponent("Sound"));
		pSoundComponent->SetSound("pulse");
		if (pSoundComponent)
		{
			float distance = CalculateDistance(m_pLink->GetPosition());

			float volume = 0.0f;
			if (distance <= m_minDistance)
				volume = 100.0f;
			else if (distance >= m_maxDistance)
				volume = 0.0f;
			else
				volume = 100.0f * (1.0f - (distance - m_minDistance) / (m_maxDistance - m_minDistance)); // Lerp formula

			pSoundComponent->SetVolume(static_cast<int>(volume));
			pSoundComponent->PlayLoop(0);
		}
		m_beepTimer = 0.0f;
	}

	m_trigger.TryTrigger();
	if (IComponent* animation = GetComponent("Animation"))
		animation->Update(deltaTime);

	ParticleComponent* pParticle = static_cast<ParticleComponent*>(GetComponent("Particle"));
	pParticle->Splash();
	pParticle->Update(deltaTime);
}

void Triforce::Draw(SDL_Renderer* pRenderer)
{
	if (IComponent* pParticle = GetComponent("Particle"))
		pParticle->Render(pRenderer);

	if (IComponent* animation = GetComponent("Animation"))
		animation->Render(pRenderer);
}

void Triforce::SetPosition(const Vector2& position)
{
	AnimationComponent* pAnimation = static_cast<AnimationComponent*>(GetComponent("Animation"));
	ParticleComponent* pParticle = static_cast<ParticleComponent*>(GetComponent("Particle"));
	m_position - position;
	m_trigger.SetPosition(position);
	pAnimation->SetPosition(position);
	Vector2 offsetPosition = { (m_trigger.GetBox().GetBounds().h / 2) + position.m_x, (m_trigger.GetBox().GetBounds().w / 2) + position.m_y };
	pParticle->SetPosition(offsetPosition);
}

GameObject* Triforce::Clone()
{
	return new Triforce(*this);
}

float Triforce::CalculateDistance(const Vector2& linkPosition) const
{
	float deltaX = m_position.m_x - linkPosition.m_x;
	float deltaY = m_position.m_y - linkPosition.m_y;
	return std::sqrt(deltaX * deltaX + deltaY * deltaY);
}