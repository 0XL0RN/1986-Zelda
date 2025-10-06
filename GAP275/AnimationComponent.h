#pragma once

#include "IComponent.h"
#include <SDL3/SDL.h>
#include <string>
#include "Vector2.h"
#include <unordered_map>

class AnimationComponent : public IComponent
{
private:
	using Vector2 = Vector2<float>;

	struct AnimationSequence
	{
		std::string m_name = "Null";
		int m_firstFrame = 0;
		int m_lastFrame = 0;

		AnimationSequence() = default;
		AnimationSequence(std::string name, const int firstFrame, const int lastFrame)
			: m_name(std::move(name)), m_firstFrame(firstFrame), m_lastFrame(lastFrame) {
		}
	};
public:
	AnimationComponent(const Vector2& dimension, SDL_FRect transform, const double& frameRate, int activeFrame, SDL_Texture* pTexture);
	void AddSequence(const std::string& name, const int& firstFrame, const int& lastFrame);
	void PlayAnimation(const std::string& name);
	void Update(double deltaTime) override;
	void Render(SDL_Renderer* pRenderer) override;
	void SetPosition(const Vector2& position);

	void PauseAnimation() { m_isPlaying = false; }
	void ResumeAnimation() { m_isPlaying = true; }
	std::string GetActiveAnimation() const { return m_activeSequence; }

private:
	Vector2 m_SheetBounds;
	double m_frameRate;
	SDL_FRect m_rectangle;
	SDL_FRect m_activeFrameRectangle;
	SDL_Texture* m_pSpriteSheet;

	bool m_isPlaying;

	int m_activeFrame;
	double m_frameTime;
	std::string m_activeSequence;
	std::unordered_map<std::string, AnimationSequence> m_animations;

	void ResetFrame();
	void UpdateTransform();
};