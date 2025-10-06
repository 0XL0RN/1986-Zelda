#pragma once

#include "SceneState.h"
#include <vector>
class SceneStateMachine;
class GameObject;
struct Mix_Music;

class WinState final : public SceneState
{
public:
	WinState(SceneStateMachine* pOwner);
	void Enter() override;
	void Update([[maybe_unused]] const double& deltaTime) override;
	void Render(SDL_Renderer* pRenderer) override;
	bool HandleEvent() override;
	void Exit() override;
private:
	SceneStateMachine* m_pOwner;
	std::vector<GameObject*> m_pObjects;
	Mix_Music* m_pMusic;
	bool m_clicked;
};