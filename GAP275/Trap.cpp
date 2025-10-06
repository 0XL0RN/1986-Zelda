#include "Trap.h"
#include "TextureComponent.h"
#include "Plate.h"
#include <iostream>

Trap::Trap(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, GameObject* pPlate, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_collider(box, this, pManager)
	, m_isActive(false)
	, m_pPlate(pPlate)
	, m_originalPosition(position)
	, m_startPosition(Vector2(112, 132))
	, m_movingRight(true)
{
	Plate* plate = static_cast<Plate*>(pPlate);
	if (plate)
	{
		RegisterEvent(EventType::kTriggerEnter, [this, plate](const Event& event) -> void
			{
				if (event.m_sender == plate->GetTrigger())
				{
					GameObject* pObject = static_cast<GameObject*>(event.m_receiver);
					if (pObject && pObject->GetId() == "Link")
					{
						SetPosition(m_startPosition);
						m_isActive = true;
					};
				}
			});

		RegisterEvent(EventType::kTriggerExit, [this, plate](const Event& event) -> void
			{
				if (event.m_sender == plate->GetTrigger())
				{
					GameObject* pObject = static_cast<GameObject*>(event.m_receiver);
					if (pObject && pObject->GetId() == "Link")
					{
						m_startPosition = m_position;
						SetPosition(m_originalPosition);
						m_isActive = false;

					}
				}
			});
	}
}

void Trap::Update(const double& deltaTime)
{
	Plate* pPlate = static_cast<Plate*>(m_pPlate);
	if (m_isActive && pPlate && pPlate->IsSteppedOn())
	{
		float speed = 200.0f;
		Vector2 moveDelta = m_movingRight ? Vector2(speed * static_cast<float>(deltaTime), 0) : Vector2(-speed * static_cast<float>(deltaTime), 0);
		Move(moveDelta);

		// Check bounds and reverse direction
		if (m_movingRight && m_position.m_x >= kEndX)
			m_movingRight = false;
		else if (!m_movingRight && m_position.m_x <= 112)
			m_movingRight = true;
	}
}

void Trap::Draw(SDL_Renderer* pRenderer)
{
	if (TextureComponent* pTextureComponent = static_cast<TextureComponent*>(GetComponent("Texture")))
	{
		pTextureComponent->SetPosition(m_position);
		pTextureComponent->Render(pRenderer);
	}
}

void Trap::SetPosition(const Vector2& position)
{
	m_position = position;
	m_collider.SetPosition(position);
	if (TextureComponent* pTextureComponent = static_cast<TextureComponent*>(GetComponent("Texture")))
		pTextureComponent->SetPosition(position);
}

void Trap::Move(const Vector2& deltaPos)
{
	if (m_collider.TryMove(deltaPos))
	{
		m_position.m_x += deltaPos.m_x;
		m_position.m_y += deltaPos.m_y;
		SetPosition(m_position);
	}
}

GameObject* Trap::Clone()
{
	return new Trap(*this);
}