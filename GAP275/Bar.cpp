#include "Bar.h"
#include "Link.h"
#include "TextureComponent.h"

Bar::Bar(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_pLinkPointer(nullptr)
	, m_pBox(new SDL_FRect{ position.m_x, position.m_y, 0, 0 })
{
}

void Bar::Update([[maybe_unused]] const double& deltaTime)
{
	if (m_pLinkPointer && m_callback)
	{
		m_callback(deltaTime, m_pLinkPointer, this);
	}
}

void Bar::Draw(SDL_Renderer* pRenderer)
{
	GetComponent("Base")->Render(pRenderer);
	GetComponent("Full")->Render(pRenderer);
}

bool Bar::HandleEvent([[maybe_unused]] SDL_Event* pEvent)
{
	return true;
}

void Bar::SetCallback(const std::function<void(double, Link*, Bar*)>& callback)
{
	m_callback = callback;
}

GameObject* Bar::Clone()
{
	return new Bar(*this);
}
