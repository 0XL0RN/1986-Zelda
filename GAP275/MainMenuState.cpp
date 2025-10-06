#include "MainMenuState.h"
#include "SceneStateMachine.h"
#include <SDL3/SDL.h>
#include "Button.h"

MainMenuState::MainMenuState(SceneStateMachine* pOwner)
	: m_pOwner(pOwner)
	, m_pMusic(nullptr)
{
}

// Load file & load scene
void MainMenuState::Enter()
{
	m_pOwner->GetSceneFactory().LoadFile("config/MainMenu.json");
	m_pOwner->GetSceneFactory().LoadScene(m_pObjects, m_pOwner->GetCollisionManager(), nullptr, m_pMusic);

	Button* pButton = nullptr;

	for (auto& m_pObject : m_pObjects)
	{
		if (m_pObject->GetId() == "PlayButton")
		{
			pButton = static_cast<Button*>(m_pObject);
			pButton->SetCallback([this]()-> void
				{
					m_pOwner->LoadState(SceneStateMachine::SceneType::kGameplay);
				});
		}
		else if (m_pObject->GetId() == "CreditButton")
		{
			pButton = static_cast<Button*>(m_pObject);
			pButton->SetCallback([this]()-> void
				{
					m_pOwner->LoadState(SceneStateMachine::SceneType::kCreditScene);
				});
		}
		else if (m_pObject->GetId() == "LoadButton")
		{
			pButton = static_cast<Button*>(m_pObject);
			pButton->SetCallback([this]()-> void
				{
					m_pOwner->LoadState(SceneStateMachine::SceneType::kLoadScene);
				});
		}
	}
}

// Update objects
void MainMenuState::Update([[maybe_unused]] const double& deltaTime)
{
	for (const auto& object : m_pObjects)
		object->Update(deltaTime);
}

// Render Objects
void MainMenuState::Render(SDL_Renderer* pRenderer)
{
	for (const auto& object : m_pObjects)
		object->Draw(pRenderer);
}

// Handle Events
bool MainMenuState::HandleEvent()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
			m_pOwner->Quit();

		for (const auto& object : m_pObjects)
			object->HandleEvent(&event);
	}
	return true;
}

// Clear collision manager for next scene state
void MainMenuState::Exit()
{
	m_pOwner->GetCollisionManager()->Clear();
}
