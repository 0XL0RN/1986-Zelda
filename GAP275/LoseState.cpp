#include "LoseState.h"
#include "SceneStateMachine.h"
#include "Button.h"

LoseState::LoseState(SceneStateMachine* pOwner)
	: m_pOwner(pOwner)
	, m_pMusic(nullptr)
	, m_clicked(false)
{
}

void LoseState::Enter()
{
	m_pOwner->GetSceneSaver().UnLoad();
	m_pOwner->GetSceneFactory().LoadFile("config/LoseScene.json");
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
		else if (m_pObject->GetId() == "MainButton")
		{
			pButton = static_cast<Button*>(m_pObject);
			pButton->SetCallback([this]()-> void
				{
					m_pOwner->LoadState(SceneStateMachine::SceneType::kMainMenu);
				});
		}
		else if (m_pObject->GetId() == "ExitButton")
		{
			pButton = static_cast<Button*>(m_pObject);
			pButton->SetCallback([this]()-> void
				{
					m_pOwner->Quit();
				});
		}
	}
}

void LoseState::Update([[maybe_unused]] const double& deltaTime)
{
	for (const auto& object : m_pObjects)
		object->Update(deltaTime);
}

void LoseState::Render(SDL_Renderer* pRenderer)
{
	for (const auto& object : m_pObjects)
		object->Draw(pRenderer);
}

bool LoseState::HandleEvent()
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

void LoseState::Exit()
{
	m_pOwner->GetCollisionManager()->Clear();
}
