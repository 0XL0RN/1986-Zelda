#pragma once

#include "IComponent.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "Vector2.h"

class TextComponent final : public IComponent
{
private:
	using Vector2 = Vector2<float>;
public:
	TextComponent(const Vector2& position, TTF_Font* pFont, const char* pText, SDL_Color color, SDL_Renderer* pRenderer);
	~TextComponent() override;
	void Update(double deltaTime) override;
	void Render(SDL_Renderer* pRenderer) override;
	void GenerateText(const char* text);
private:
	SDL_Texture* m_pText;
	SDL_FRect m_box;
	TTF_Font* m_pFont;
	SDL_Renderer* m_pRenderer;
};