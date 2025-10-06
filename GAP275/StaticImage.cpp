#include "StaticImage.h"
#include "TextureComponent.h"

StaticImage::StaticImage(const Vector2& position, const int& depth, const std::string& id, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
{
}

void StaticImage::Update([[maybe_unused]] const double& deltaTime)
{

}

void StaticImage::Draw(SDL_Renderer* pRenderer)
{
	if (GetComponent("Texture") == nullptr)
		return;

	GetComponent("Texture")->Render(pRenderer);
}

std::string StaticImage::GetId()
{
	return "StaticImage";
}

GameObject* StaticImage::Clone()
{
	return new StaticImage(*this);
}
