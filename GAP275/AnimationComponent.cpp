#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(const Vector2& dimension, SDL_FRect transform, const double& frameRate, int activeFrame, SDL_Texture* pTexture)
	: m_SheetBounds(dimension)
	, m_frameRate(frameRate)
	, m_rectangle(transform)
	, m_pSpriteSheet(pTexture)
	, m_isPlaying(true)
	, m_activeFrame(activeFrame)
	, m_frameTime(-1)

{
	m_SheetBounds.m_x = pTexture->w / dimension.m_x;

	m_activeFrameRectangle = m_rectangle;
}

void AnimationComponent::AddSequence(const std::string& name, const int& firstFrame, const int& lastFrame)
{
	m_animations[name] = AnimationSequence{ name, firstFrame, lastFrame };
}

void AnimationComponent::PlayAnimation(const std::string& name)
{
	if (name == m_activeSequence || !m_animations.contains(name))
		return;


	const AnimationSequence& sequence = m_animations[name];
	m_activeFrame = sequence.m_firstFrame;

	m_activeSequence = name;
	ResetFrame();
	UpdateTransform();
}

void AnimationComponent::Update(double deltaTime)
{
	if (m_activeFrame == -1 || !m_isPlaying)
		return;

	if (m_animations[m_activeSequence].m_firstFrame == m_animations[m_activeSequence].m_lastFrame)
	{
		return; // Static frame, no update needed
	}

	const AnimationSequence& currentSequence = m_animations[m_activeSequence];

	bool frameIsDirty = false;

	while (deltaTime > m_frameTime)
	{
		frameIsDirty = true;

		deltaTime -= m_frameTime;
		ResetFrame();

		if (currentSequence.m_lastFrame == m_activeFrame)
			m_activeFrame = currentSequence.m_firstFrame;
		else
			++m_activeFrame;
	}

	m_frameTime -= deltaTime;

	if (frameIsDirty)
		UpdateTransform();
}

void AnimationComponent::Render(SDL_Renderer* pRenderer)
{
	SDL_RenderTexture(pRenderer, m_pSpriteSheet, &m_activeFrameRectangle, &m_rectangle);
}

void AnimationComponent::SetPosition(const Vector2& position)
{
	m_rectangle.x = position.m_x;
	m_rectangle.y = position.m_y;
}

void AnimationComponent::ResetFrame()
{
	m_frameTime = 1.0 / m_frameRate;
}

void AnimationComponent::UpdateTransform()
{
	const int x = m_activeFrame % static_cast<int>(m_SheetBounds.m_x);
	const int y = m_activeFrame / static_cast<int>(m_SheetBounds.m_x);

	m_activeFrameRectangle.x = x * m_activeFrameRectangle.w;
	m_activeFrameRectangle.y = y * m_activeFrameRectangle.h;
}
