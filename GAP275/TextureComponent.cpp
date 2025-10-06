#include "TextureComponent.h"
#include "Vector2.h"

TextureComponent::TextureComponent(SDL_Texture* pTexture, const SDL_FRect box)
	: m_pTexture(pTexture)
	, m_rectangle{box}
	, m_pBox(nullptr)
{
	
}

TextureComponent::~TextureComponent()
{
	delete m_pBox;
}

void TextureComponent::Update([[maybe_unused]] double deltaTime)
{

}

void TextureComponent::Render(SDL_Renderer* pRenderer)
{
	SDL_RenderTexture(pRenderer, m_pTexture, m_pBox, &m_rectangle);
}

void TextureComponent::SetPosition(const Vector2& position)
{
	m_rectangle.x = position.m_x;
	m_rectangle.y = position.m_y;
}

SDL_FRect* TextureComponent::GetCover(SDL_FRect* pBox)
{
	if (pBox)
		m_pBox = pBox;

	return m_pBox;
}
