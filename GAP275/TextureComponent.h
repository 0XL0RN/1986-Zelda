#pragma once

#include "IComponent.h"
#include <SDL3/SDL_render.h>
#include "Vector2.h"

class TextureComponent final : public IComponent
{
private:
	using Vector2 = Vector2<float>;
public:
	TextureComponent(SDL_Texture* pTexture, const SDL_FRect box);
	~TextureComponent() override;
	void Update(double deltaTime) override;
	void Render(SDL_Renderer* pRenderer) override;

	void SetPosition(const Vector2& position);
	SDL_FRect& GetPosition() { return m_rectangle; }
	SDL_FRect* GetCover(SDL_FRect* pBox);
private:
	SDL_Texture* m_pTexture;
	SDL_FRect m_rectangle;
	SDL_FRect* m_pBox;
};
