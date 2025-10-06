#include "TextComponent.h"

TextComponent::TextComponent(const Vector2& position, TTF_Font* pFont, const char* pText, SDL_Color color, SDL_Renderer* pRenderer)
	: m_pText(nullptr)
	, m_pFont(pFont)
	, m_pRenderer(pRenderer)
{
	SDL_Surface* pSurface = TTF_RenderText_Solid(pFont, pText, 0, color);

	m_pText = SDL_CreateTextureFromSurface(pRenderer, pSurface);

	m_box.x = position.m_x;
	m_box.y = position.m_y;

	m_box.w = static_cast<float>(pSurface->w);
	m_box.h = static_cast<float>(pSurface->h);

	SDL_DestroySurface(pSurface);
}

TextComponent::~TextComponent()
{
	SDL_DestroyTexture(m_pText);
}

void TextComponent::Update([[maybe_unused]] double deltaTime)
{

}

void TextComponent::Render(SDL_Renderer* pRenderer)
{
	SDL_RenderTexture(pRenderer, m_pText, nullptr, &m_box);
}

void TextComponent::GenerateText(const char* text)
{
	SDL_DestroyTexture(m_pText);

	SDL_Surface* pSurface = TTF_RenderText_Solid(m_pFont, text, 0, { 0, 0, 0, 0 });

	m_pText = SDL_CreateTextureFromSurface(m_pRenderer, pSurface);


	m_box.w = static_cast<float>(pSurface->w);
	m_box.h = static_cast<float>(pSurface->h);

	SDL_DestroySurface(pSurface);
}
