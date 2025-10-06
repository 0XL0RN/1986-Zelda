#include "SaveMenu.h"
#include "SceneStateMachine.h"
#include "Button.h"

SaveMenu::SaveMenu(SceneStateMachine* pOwner)
	: m_pOwner(pOwner)
	, m_pMusic(nullptr)
{
}

void SaveMenu::Enter()
{
	m_pOwner->GetSceneFactory().LoadFile("config/SaveMenu.json");
	m_pOwner->GetSceneFactory().LoadScene(m_pObjects, m_pOwner->GetCollisionManager(), nullptr, m_pMusic);

	m_pOwner->GetSceneSaver().CheckForSave();

	Button* pButtonOne = nullptr;
	Button* pButtonTwo = nullptr;

	for (auto& m_pObject : m_pObjects)
	{
		if (m_pObject->GetId() == "MenuButton")
		{
			Button* pButton = static_cast<Button*>(m_pObject);
			pButton->SetCallback([this]()-> void
				{
					m_pOwner->LoadState(SceneStateMachine::SceneType::kMainMenu);
				});
		}
		else if (m_pObject->GetId() == "SaveButtonOne")
		{
			Button* pButton = static_cast<Button*>(m_pObject);
			if (!m_pOwner->GetSceneSaver().IsSaveOne())
				pButton->Deactivate();
			pButton->SetCallback([this]()-> void
				{
					m_pOwner->GetSceneSaver().Load("saveone.save");
					m_pOwner->LoadState(SceneStateMachine::SceneType::kGameplay);
				});
			pButtonOne = pButton;
		}
		else if (m_pObject->GetId() == "SaveButtonTwo")
		{
			Button* pButton = static_cast<Button*>(m_pObject);
			if (!m_pOwner->GetSceneSaver().IsSaveTwo())
				pButton->Deactivate();
			pButton->SetCallback([this]()-> void
				{
					m_pOwner->GetSceneSaver().Load("savetwo.save");
					m_pOwner->LoadState(SceneStateMachine::SceneType::kGameplay);
				});
			pButtonTwo = pButton;
		}
		else if (m_pObject->GetId() == "DeleteButtonOne")
		{
			Button* pButton = static_cast<Button*>(m_pObject);
			if (!m_pOwner->GetSceneSaver().IsSaveOne())
				pButton->Deactivate();
			pButton->SetCallback([this, pButton, pButtonOne]()-> void
				{
					m_pOwner->GetSceneSaver().DeleteFile("saveone.save");
					pButton->Deactivate();
					pButtonOne->Deactivate();
				});
		}
		else if (m_pObject->GetId() == "DeleteButtonTwo")
		{
			Button* pButton = static_cast<Button*>(m_pObject);
			if (!m_pOwner->GetSceneSaver().IsSaveTwo())
				pButton->Deactivate();
			pButton->SetCallback([this, pButton, pButtonTwo]()-> void
				{
					m_pOwner->GetSceneSaver().DeleteFile("savetwo.save");
					pButton->Deactivate();
					pButtonTwo->Deactivate();
				});
		}
	}
}

void SaveMenu::Update([[maybe_unused]] const double& deltaTime)
{
	for (const auto& object : m_pObjects)
		object->Update(deltaTime);
}

void SaveMenu::Render(SDL_Renderer* pRenderer)
{
	for (const auto& object : m_pObjects)
		object->Draw(pRenderer);
}

bool SaveMenu::HandleEvent()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		for (const auto& object : m_pObjects)
			object->HandleEvent(&event);
	}
	return true;
}

void SaveMenu::Exit()
{
	m_pOwner->GetCollisionManager()->Clear();
}
