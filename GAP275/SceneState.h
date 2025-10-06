#pragma once

class SceneState
{
public:
	SceneState() = default;
	virtual ~SceneState() = default;
	virtual void Enter() = 0;
	virtual void Update([[maybe_unused]] const double& deltaTime) = 0;
	virtual void Render(struct SDL_Renderer* pRenderer) = 0;
	virtual bool HandleEvent() = 0;
	virtual void Exit() = 0;

};