#include "StaticText.h"
#include "TextComponent.h"

StaticText::StaticText(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
{
}

void StaticText::Update([[maybe_unused]] const double& deltaTime)
{

}

void StaticText::Draw(SDL_Renderer* pRenderer)
{
	GetComponent("Text")->Render(pRenderer);
}

GameObject* StaticText::Clone()
{
	return new StaticText(*this);
}

void StaticText::SetCallback(const std::function<void(const char* text)>& callback)
{
	m_callback = callback;
}
