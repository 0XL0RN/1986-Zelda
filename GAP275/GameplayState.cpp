#include "GameplayState.h"
#include <SDL_mixer.h>
#include "SceneStateMachine.h"
#include "GameObject.h"
#include "EventHandler.h"
#include "Link.h"
#include "Bar.h"
#include "Slider.h"
#include "TextureComponent.h"
#include "SceneSaver.h"

GameplayState::GameplayState(SceneStateMachine* pOwner)
	: m_pOwner(pOwner)
	, m_spawner(m_pObjects, m_pOwner)
	, m_pMusic(nullptr)
	, m_pLink(nullptr)
{
}

// Read file and load scene
void GameplayState::Enter()
{
	if (m_pOwner->GetSceneSaver().IsLoaded())
	{
		m_pOwner->GetSceneFactory().LoadFile("config/GameScene.json");
		m_pOwner->GetSceneFactory().LoadScene(m_pObjects, m_pOwner->GetCollisionManager(), &m_pOwner->GetSceneSaver().GetSaveData(), m_pMusic);
	}
	else
	{
		m_pOwner->GetSceneFactory().LoadFile("config/GameScene.json");
		m_pOwner->GetSceneFactory().LoadScene(m_pObjects, m_pOwner->GetCollisionManager(), nullptr, m_pMusic);
	}

	if (m_pMusic != nullptr)
		Mix_PlayMusic(m_pMusic, -1);

	Mix_VolumeMusic(m_pOwner->GetMasterVolume());

	Link* linkPtr = nullptr;
	Bar* healthBarPtr = nullptr;
	Bar* mainHealthBarPtr = nullptr;
	Slider* pVolumeSlider = nullptr;

	for (const auto& pObject : m_pObjects)
	{
		if (pObject->GetId() == "Link")
		{
			linkPtr = static_cast<Link*>(pObject);
			m_pLink = linkPtr;
		}

		if (pObject->GetId() == "HealthBar")
			healthBarPtr = static_cast<Bar*>(pObject);

		if (pObject->GetId() == "MainHealthBar")
			mainHealthBarPtr = static_cast<Bar*>(pObject);

		if (pObject->GetId() == "VolumeSlider")
			pVolumeSlider = static_cast<Slider*>(pObject);
	}

	healthBarPtr->SetLink(linkPtr);
	mainHealthBarPtr->SetLink(linkPtr);

	if (m_pOwner->GetSceneSaver().IsLoaded())
	{
		std::cout << "Loading Positions\n";
		m_pLink->SetPosition(m_pOwner->GetSceneSaver().GetSaveData().playerPosition);

		for (size_t i = 0; i < m_pOwner->GetSceneSaver().GetSaveData().enemyCount; ++i)
		{
			Vector2 position = m_pOwner->GetSceneSaver().GetSaveData().enemyPositions[i];
			m_spawner.GenerateEnemy(position);
		}

		size_t blockCount = 0;
		size_t fireCount = 0;
		size_t rupeeCount = 0;
		size_t enemyCount = 0;
		for (const auto& pObject : m_pObjects)
		{
			if (pObject->GetId() == "Block")
			{
				pObject->SetPosition(m_pOwner->GetSceneSaver().GetSaveData().blockPositions[blockCount]);
				++blockCount;
			}
			else if (pObject->GetId() == "Fire")
			{
				pObject->SetPosition(m_pOwner->GetSceneSaver().GetSaveData().firePositions[fireCount]);
				++fireCount;
			}
			else if (pObject->GetId() == "Rupee")
			{
				pObject->SetPosition(m_pOwner->GetSceneSaver().GetSaveData().rupeePosition[rupeeCount]);
				++rupeeCount;
			}
			else if (pObject->GetId() == "Enemy")
			{
				pObject->SetPosition(m_pOwner->GetSceneSaver().GetSaveData().enemyPositions[enemyCount]);
				++enemyCount;
			}
			else if (pObject->GetId() == "Boss")
			{
				pObject->SetPosition(m_pOwner->GetSceneSaver().GetSaveData().bossPosition);
			}
			else if (pObject->GetId() == "Triforce")
			{
				pObject->SetPosition(m_pOwner->GetSceneSaver().GetSaveData().triforcePosition);
			}
		}
	}
	else
		m_spawner.Awake(linkPtr);

	if (linkPtr && healthBarPtr)
	{
		healthBarPtr->SetCallback([]([[maybe_unused]] double deltaTime, const Link* link, const Bar* bar)
			{
				Vector2 linkPos = link->GetPosition();
				int hp = static_cast<int>(link->GetHealth());
				int maxHp = 20;

				// Update bar position above Link
				if (auto* base = static_cast<TextureComponent*>(bar->GetComponent("Base")))
				{
					base->SetPosition({ linkPos.m_x, linkPos.m_y - 15 });
				}

				if (auto* full = static_cast<TextureComponent*>(bar->GetComponent("Full")))
				{
					full->SetPosition({ linkPos.m_x, linkPos.m_y - 15 });

					auto* box = bar->GetBox();
					box->x = 0;
					box->y = 0;
					box->w = (hp * 42.0f) / maxHp;
					box->h = 12;

					full->GetPosition().w = box->w;
					full->GetPosition().h = box->h;
					full->GetCover(box);
				}
			});

		mainHealthBarPtr->SetCallback([]([[maybe_unused]] double deltaTime, Link* link, Bar* bar)
			{
				int hp = static_cast<int>(link->GetHealth());
				int maxHp = 20;

				if (auto* full = static_cast<TextureComponent*>(bar->GetComponent("Full")))
				{

					auto* box = bar->GetBox();
					box->x = 0;
					box->y = 0;
					box->w = (hp * 200.0f) / maxHp;
					box->h = 24;

					full->GetPosition().w = box->w;
					full->GetPosition().h = box->h;
					full->GetCover(box);
				}
			});
	}

	pVolumeSlider->SetValue(&m_pOwner->GetMasterVolume());

}

void GameplayState::Update([[maybe_unused]] const double& deltaTime)
{
	for (const auto& object : m_pObjects)
		object->Update(deltaTime);

	Mix_VolumeMusic(m_pOwner->GetMasterVolume());

	m_spawner.Update(deltaTime);
}

void GameplayState::Render(SDL_Renderer* pRenderer)
{
	for (const auto& object : m_pObjects)
		object->Draw(pRenderer);
}

bool GameplayState::HandleEvent()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
			m_pOwner->Quit();

		for (const auto& object : m_pObjects)
			object->HandleEvent(&event);

		switch (event.type)
		{
		case SDL_EVENT_KEY_DOWN:
			if (event.key.key == SDLK_K)
				SaveGame(); // Save game when 'K' is pressed
			EventHandler::ProcessKeyboardEvent(&event.key);
			break;
		case SDL_EVENT_KEY_UP:
			EventHandler::ProcessKeyboardEvent(&event.key);
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			EventHandler::ProcessMouseEvent(&event.button);
			break;
		case SDL_EVENT_QUIT:
			return false;
		}
	}
	m_pOwner->GetEventQ().ProcessEvents(m_pObjects);

	return true;
}

void GameplayState::SaveGame() const
{
	SceneSaver::SaveData& data = m_pOwner->GetSceneSaver().GetSaveData();

	data.playerHealth = static_cast<Link*>(m_pLink)->GetHealth();
	data.playerPosition = static_cast<Link*>(m_pLink)->GetPosition();

	size_t blockCount = 0;
	size_t fireCount = 0;
	size_t rupeeCount = 0;
	size_t enemyCount = 0;

	for (const auto& object : m_pObjects)
	{
		if (object->GetId() == "Block")
		{
			data.blockPositions[blockCount] = object->GetPosition();
			++blockCount;
		}
		else if (object->GetId() == "Fire")
		{
			data.firePositions[fireCount] = object->GetPosition();
			++fireCount;
		}
		else if (object->GetId() == "Rupee")
		{
			data.rupeePosition[rupeeCount] = object->GetPosition();
			++rupeeCount;
		}
		else if (object->GetId() == "Enemy")
		{
			data.enemyPositions[enemyCount] = object->GetPosition();
			++enemyCount;
		}
		else if (object->GetId() == "Boss")
		{
			data.bossPosition = object->GetPosition();
		}
		else if (object->GetId() == "Triforce")
		{
			data.triforcePosition = object->GetPosition();
		}
	}

	data.blockCount = 3;
	data.fireCount = 3;
	data.enemyCount = 4;
	data.rupeeCount = 2;

	const bool isSaved = m_pOwner->GetSceneSaver().Save();
	if (isSaved)
		std::cout << "Saving Complete\n";
}

// Clear events and collisions
void GameplayState::Exit()
{
	Mix_HaltMusic();
	EventHandler::Clear();
	m_pOwner->GetCollisionManager()->Clear();
}
