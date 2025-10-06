#include "SceneStateMachine.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "WinState.h"
#include "LoseState.h"
#include "CreditsScene.h"
#include "SaveMenu.h"
#include "SDL_mixer.h"

SceneStateMachine::SceneStateMachine()
	: m_collisionManager(m_eventQ)
	, m_pRunningState(nullptr)
	, m_pBufferState(nullptr)
	, m_masterVolume(s_kMaxVolume)
	, m_isRunning(true)
{
}

SceneStateMachine::~SceneStateMachine()
{
	delete m_pRunningState;
}

// Awaken SceneState Machine
bool SceneStateMachine::Awake(SDL_Renderer* pRenderer)
{
	// Scene Machine Awakened
	m_factory.Awake(pRenderer);

	if (m_pBufferState != nullptr)
		return true;
	else
		return false;
}

// Update SceneState or change if buffer != nullptr
void SceneStateMachine::UpdateState([[maybe_unused]] const double& deltaTime)
{
	if (m_sceneQ != SceneType::kDefault)
	{
		LoadState(m_sceneQ);
		m_sceneQ = SceneType::kDefault;
	}

	if (m_pBufferState != nullptr)
	{
		RotateState(m_pBufferState);
		m_pBufferState = nullptr;
	}

	if (m_pRunningState != nullptr)
		m_pRunningState->Update(deltaTime);

	Mix_MasterVolume(m_masterVolume);
}

// Render SceneState
void SceneStateMachine::RenderState(SDL_Renderer* pRenderer)
{
	if (m_pRunningState != nullptr)
		m_pRunningState->Render(pRenderer);
}

// Handle state events
bool SceneStateMachine::HandleEvent()
{
	if (m_pRunningState != nullptr && m_isRunning)
		return m_pRunningState->HandleEvent();
	return m_isRunning;
}

// Rotate to new state
void SceneStateMachine::RotateState(SceneState* pState)
{
	if (m_pRunningState != nullptr)
		m_pRunningState->Exit();

	delete m_pRunningState;
	m_pRunningState = pState;
	m_pRunningState->Enter();
}

// Load instance of state
void SceneStateMachine::LoadState(const SceneType& type)
{
	switch (type)
	{
	case SceneType::kMainMenu: m_pBufferState = new MainMenuState(this); break;
	case SceneType::kGameplay: m_pBufferState = new GameplayState(this); break;
	case SceneType::kWinScene: m_pBufferState = new WinState(this); break;
	case SceneType::kLoseScene: m_pBufferState = new LoseState(this); break;
	case SceneType::kCreditScene: m_pBufferState = new CreditsScene(this); break;
	case SceneType::kLoadScene: m_pBufferState = new SaveMenu(this); break;
	}
}
