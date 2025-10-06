#include "Link.h"
#include "SceneStateMachine.h"
#include "AnimationComponent.h"
#include "ParticleComponent.h"
#include "SoundComponent.h"
#include "TextComponent.h"
#include "EventHandler.h"
#include "Boss.h"
#include <iostream>

Link::Link(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_movingToTarget(false)
	, m_targetPosition(position)
	, m_collider(box, this, pManager)
	, m_hp(20)
	, m_isAlive(true)
	, m_isColliding(false)
	, m_IsInRange(false)
	, m_range({ box.x - 48, box.y - 48, 128, 128 }, this, pManager)
	, m_pTarget(nullptr)
{
	std::cout << "Link's health: " << m_hp << "\n";
	RegisterEvent(EventType::kCollision, [this](const Event& event) -> void
		{
			if (event.m_sender == &m_collider)
			{
				if (!m_isColliding)
				{
					static_cast<SoundComponent*>(GetComponent("Sound"))->Play("bump", 0);
					static_cast<SoundComponent*>(GetComponent("Sound"))->SetVolume(3);
					m_isColliding = true;
				}
			}
			else
				m_isColliding = false;

		});


	RegisterEvent(EventType::kTriggerEnter, [this](const Event& event) -> void
		{
			if (event.m_sender == &m_range)
			{
				// TODO generalize so I am able to attack all enemies
				if (const auto pObject = static_cast<Boss*>(event.m_receiver); pObject->GetId() == "Boss")
				{
					m_IsInRange = true;
					m_pTarget = pObject;
				}
			}
		});

	RegisterEvent(EventType::kTriggerExit, [this](const Event& event) -> void
		{
			if (event.m_sender == &m_range)
			{
				m_IsInRange = false;
				m_pTarget = nullptr;
			}
		});

}

void Link::SetHealth(const float& health)
{
	float oldHp = m_hp;
	m_hp += health;
	if (m_hp > kMaxHp)
		m_hp = kMaxHp;

	if (m_hp < 0)
		m_hp = 0;

	if (m_hp != oldHp)
	{
		if (m_hp < oldHp)
		{
			static_cast<SoundComponent*>(GetComponent("Sound"))->Play("hurt", 0);
			static_cast<ParticleComponent*>(GetComponent("Particle"))->Splash();

		}
		else
			static_cast<SoundComponent*>(GetComponent("Sound"))->Play("heal", 0);
		std::cout << "Link's health: " << m_hp << "\n";
		if (m_hp <= 0)
		{
			std::cout << "You lose!\n";
			m_isAlive = false;
			SceneStateMachine::SetScene(SceneStateMachine::SceneType::kLoseScene);
		}
	}
}

void Link::Update(const double& deltaTime)
{
	m_range.TryTrigger();
	AnimationComponent* pAnimation = static_cast<AnimationComponent*>(GetComponent("Animation"));
	if (!pAnimation)
		return;

	const float deltaDistance = kSpeed * static_cast<float>(deltaTime);

	Vector2 moveDelta(0, 0);

	// Mouse movement
	if (EventHandler::IsMouseDown(m_targetPosition))
		m_movingToTarget = true;

	// WASD or mouse targeting
	if (m_movingToTarget)
	{
		const float dx = m_targetPosition.m_x - m_position.m_x;
		const float dy = m_targetPosition.m_y - m_position.m_y;

		//Distance Formula
		const float distance = std::sqrt(dx * dx + dy * dy);

		if (distance > deltaDistance)
			moveDelta = { dx / distance * deltaDistance, dy / distance * deltaDistance };
		else
		{
			moveDelta = { dx, dy };
			m_movingToTarget = false;
		}
	}
	else
	{
		if (EventHandler::IsKeyDown(SDL_SCANCODE_W)) moveDelta.m_y -= deltaDistance;
		if (EventHandler::IsKeyDown(SDL_SCANCODE_S)) moveDelta.m_y += deltaDistance;
		if (EventHandler::IsKeyDown(SDL_SCANCODE_A)) moveDelta.m_x -= deltaDistance;
		if (EventHandler::IsKeyDown(SDL_SCANCODE_D)) moveDelta.m_x += deltaDistance;

		// Normalize diagonal WASD movement
		if (moveDelta.m_x != 0 && moveDelta.m_y != 0)
		{
			float length = std::sqrt(moveDelta.m_x * moveDelta.m_x + moveDelta.m_y * moveDelta.m_y);
			moveDelta = { moveDelta.m_x / length * deltaDistance, moveDelta.m_y / length * deltaDistance };
		}
	}

	// Apply movement and set animation
	if (moveDelta.m_x != 0 || moveDelta.m_y != 0)
	{
		if (m_collider.TryMove(moveDelta))
		{
			m_position.m_x += moveDelta.m_x;
			m_position.m_y += moveDelta.m_y;

			m_lastAnimation = (std::abs(moveDelta.m_x) > std::abs(moveDelta.m_y))
				? (moveDelta.m_x > 0 ? "right" : "left")
				: (moveDelta.m_y > 0 ? "down" : "up");

			pAnimation->PlayAnimation(m_lastAnimation);
			pAnimation->ResumeAnimation();
		}
		else if (m_movingToTarget)
			m_movingToTarget = false;
	}
	else
		pAnimation->PauseAnimation();

	// Sync positions
	m_collider.SetPosition(m_position);
	m_range.SetPosition({ m_position.m_x - 40, m_position.m_y - 40 });
	pAnimation->SetPosition(m_position);
	Vector2 offsetPosition = { (48 / 2) + m_position.m_x, (48 / 2) + m_position.m_y };
	static_cast<ParticleComponent*>(GetComponent("Particle"))->SetPosition(offsetPosition);
	pAnimation->Update(deltaTime);
	GetComponent("Particle")->Update(deltaTime);

	std::string text = std::to_string(static_cast<int>(m_hp));
	if (auto* textComp = static_cast<TextComponent*>(GetComponent("Text")))
		textComp->GenerateText(text.c_str());
}

void Link::Draw(SDL_Renderer* pRenderer)
{

	/*SDL_SetRenderDrawColor(pRenderer, 255, 0, 100, 50);
	SDL_RenderFillRect(pRenderer, &m_range.GetBox().GetBounds());
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 100, 50);
	SDL_RenderFillRect(pRenderer, &m_collider.GetBounds());*/

	if (IComponent* pParticle = GetComponent("Particle"))
		pParticle->Render(pRenderer);

	if (IComponent* pAnimation = GetComponent("Animation"))
		pAnimation->Render(pRenderer);

	if (IComponent* pText = GetComponent("Text"))
		pText->Render(pRenderer);
}

bool Link::HandleEvent(SDL_Event* pEvent)
{
	const SDL_KeyboardEvent* pKeyboardEvent = &pEvent->key;

	if (pEvent->type == SDL_EVENT_KEY_UP)
		if (pKeyboardEvent->scancode == SDL_SCANCODE_F)
			if (m_pTarget && m_IsInRange)
				m_pTarget->SetHealth(-10);
	return true;
}

void Link::SetPosition(const Vector2& position)
{
	const auto pAnimation = static_cast<AnimationComponent*>(GetComponent("Animation"));
	m_position = position;
	m_collider.SetPosition(m_position);
	pAnimation->SetPosition(m_position);
	Vector2 offsetPosition = { (48 / 2) + position.m_x, (48 / 2) + position.m_y };
	static_cast<ParticleComponent*>(GetComponent("Particle"))->SetPosition(offsetPosition);
}


GameObject* Link::Clone()
{
	return new Link(*this);
}