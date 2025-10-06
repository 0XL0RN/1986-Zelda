#include "CreditsScene.h"
#include "SceneStateMachine.h"
#include "Button.h"

CreditsScene::CreditsScene(SceneStateMachine* pOwner)
	: m_pOwner(pOwner)
	, m_pMusic(nullptr)
{
}

void CreditsScene::Enter()
{
	m_pOwner->GetSceneFactory().LoadFile("config/Credits.json");
	m_pOwner->GetSceneFactory().LoadScene(m_pObjects, m_pOwner->GetCollisionManager(), nullptr, m_pMusic);

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
	}
}

void CreditsScene::Update([[maybe_unused]] const double& deltaTime)
{
	for (const auto& object : m_pObjects)
		object->Update(deltaTime);
}

void CreditsScene::Render(SDL_Renderer* pRenderer)
{
	for (const auto& object : m_pObjects)
		object->Draw(pRenderer);
}

bool CreditsScene::HandleEvent()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		for (const auto& object : m_pObjects)
			object->HandleEvent(&event);
	}
	return true;
}

void CreditsScene::Exit()
{
	m_pOwner->GetCollisionManager()->Clear();
}
