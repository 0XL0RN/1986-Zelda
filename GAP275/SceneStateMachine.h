#pragma once

#include "SceneFactory.h"
#include "CollisionManager.h"
#include "EventQ.h"
#include "SceneSaver.h"
struct SDL_Renderer;
class SceneState;

// Sub Scene SceneState Machine
class SceneStateMachine final
{
private:
	static constexpr size_t s_kMaxVolume = 100;
public:
	enum class SceneType
	{
		kMainMenu,
		kGameplay,
		kWinScene,
		kLoseScene,
		kCreditScene,
		kLoadScene,
		kDefault
	};
	SceneStateMachine();
	~SceneStateMachine();
	bool Awake(SDL_Renderer* pRenderer);
	void UpdateState([[maybe_unused]] const double& deltaTime);
	void RenderState(SDL_Renderer* pRenderer);
	bool HandleEvent();
	void LoadState(const SceneType& type);

	// Getters
	SceneFactory& GetSceneFactory() { return m_factory; }
	CollisionManager* GetCollisionManager() { return &m_collisionManager; }
	EventQ& GetEventQ() { return m_eventQ; }

	static void SetScene(const SceneType& type) { m_sceneQ = type; }
	int& GetMasterVolume() { return m_masterVolume; }
	SceneSaver& GetSceneSaver() { return m_sceneSaver; }
	void Quit() { m_isRunning = false; }
private:
	SceneFactory m_factory;
	CollisionManager m_collisionManager;
	EventQ m_eventQ;
	SceneSaver m_sceneSaver;
	SceneState* m_pRunningState;
	SceneState* m_pBufferState;
	inline static SceneType m_sceneQ;
	int m_masterVolume;
	bool m_isRunning;
	void RotateState(SceneState* pState);
};