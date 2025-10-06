#include "Dungeon.h"
#include <iostream>
#include "IComponent.h"

Dungeon::Dungeon(const Vector2& position, const int& depth, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_wallOne(SDL_FRect{ 0, 0, 800, 60 }, this, pManager)
	, m_wallTwo(SDL_FRect{ 0, 0, 43, 600 }, this, pManager)
	, m_wallThree(SDL_FRect{ 0, 540, 800, 60 }, this, pManager)
	, m_wallFour(SDL_FRect{ 757, 0, 43, 600 }, this, pManager)

{
	/*RegisterEvent(EventType::kCollision, [this](const Event& event)
		{
			if (event.m_sender == &m_wallOne ||
				event.m_sender == &m_wallTwo ||
				event.m_sender == &m_wallThree ||
				event.m_sender == &m_wallFour)
			{
				GameObject* collider = static_cast<GameObject*>(event.m_receiver);
				if (collider->GetId() == "Link")
					std::cout << collider->GetId() << " bumped into " << GetId() << '\n';
			}
		});*/
}

void Dungeon::Update([[maybe_unused]] const double& deltaTime)
{

}

void Dungeon::Draw(SDL_Renderer* pRenderer)
{
	if (GetComponent("Texture") == nullptr)
		return;

	GetComponent("Texture")->Render(pRenderer);
}

GameObject* Dungeon::Clone()
{
	return new Dungeon(*this);
}
