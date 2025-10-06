#pragma once

#include "SceneState.h"
#include "EntitySpawner.h"
#include <vector>
class SceneStateMachine;
class GameObject;
struct Mix_Music;

// Gameplay state for gameplay
class GameplayState final : public SceneState
{
public:
	GameplayState(SceneStateMachine* pOwner);
	void Enter() override;
	void Update([[maybe_unused]] const double& deltaTime) override;
	void Render(SDL_Renderer* pRenderer) override;
	bool HandleEvent() override;
	void SaveGame() const;
	void Exit() override;
private:
	SceneStateMachine* m_pOwner;
	std::vector<GameObject*> m_pObjects;
	EntitySpawner m_spawner;
	Mix_Music* m_pMusic;
	GameObject* m_pLink;;
};