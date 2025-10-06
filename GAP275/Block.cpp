#include "Block.h"
#include "TextureComponent.h"
#include <iostream>

Block::Block(const Vector2& position, const int& depth, const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_collider(box, this, pManager)
{
	/*RegisterEvent(EventType::kCollision, [this](const Event& event)-> void
		{
			if (event.m_sender == &m_collider)
			{
				GameObject* collector = static_cast<GameObject*>(event.m_receiver);
				if (collector->GetId() == "Link")
					std::cout << collector->GetId() << " bumped into block\n";
			}
		});*/
}

void Block::Update([[maybe_unused]] const double& deltaTime)
{
}

void Block::Draw(SDL_Renderer* pRenderer)
{
	GetComponent("Texture")->Render(pRenderer);
}

void Block::SetPosition(const Vector2& position)
{
	TextureComponent* pTextureComponent = static_cast<TextureComponent*>(GetComponent("Texture"));
	m_position = position;
	m_collider.SetPosition(position);
	pTextureComponent->SetPosition(position);
}


GameObject* Block::Clone()
{
	return new Block(*this);
}
