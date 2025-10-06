#include "SceneFactory.h"

// Core
#include <iostream>
#include <fstream>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL_mixer.h>
#include "SceneSaver.h"

// Objects
#include "GameObject.h"
#include "Dungeon.h"
#include "Link.h"
#include "Rupee.h"
#include "Enemy.h"
#include "Block.h"
#include "Fire.h"
#include "Heart.h"
#include "Triforce.h"
#include "Plate.h"
#include "Trap.h"
#include "Boss.h"

// UI CONTROL
#include "StaticImage.h"
#include "StaticText.h"
#include "Button.h"
#include "Slider.h"
#include "Bar.h"

// Components
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "TextComponent.h"
#include "SoundComponent.h"
#include "ParticleComponent.h"


SceneFactory::SceneFactory()
	: m_objCount(0)
	, m_pRenderer(nullptr)
{
}

SceneFactory::~SceneFactory()
{
	for (auto& [m_pData, m_name] : m_textures)
		SDL_DestroyTexture(m_pData);

	for (auto& [m_pData, m_name] : m_fonts)
		TTF_CloseFont(m_pData);
	// TODO
	/*for (auto& [m_pData, m_name] : m_music)
		Mix_FreeMusic(m_pData);*/

	for (auto& [m_pData, m_name] : m_sound)
		Mix_FreeChunk(m_pData);
}

void SceneFactory::Awake(SDL_Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
	LoadSprites();
	LoadFont();
	LoadMusic();
	LoadSounds();
}

void SceneFactory::LoadFile(const std::string& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cout << "Could not open Scene.json\n";
		return;
	}

	m_file = json::parse(file);

	// Total Object Count
	m_objCount = m_file["objects"].size();
}

void SceneFactory::LoadScene(std::vector<GameObject*>& objects, CollisionManager* pManager, SceneSaver::SaveData* pSaveData, Mix_Music*& pSceneMusic)
{
	std::cout << "Loading Scene...\n";

	GameObject* pPlate = nullptr; // Store plate pointer
	GameObject* pLink = nullptr; // Store Link pointer

	// First pass: Create all objects
	for (size_t i = 0; i < m_objCount; ++i)
	{
		json& obj = m_file["objects"][i];
		std::string type = obj["type"];
		Vector2 pos{ obj["position"][0], obj["position"][1] };
		Vector2 dim{ obj["dimension"][0], obj["dimension"][1] };
		SDL_FRect box = CreateBox(pos, dim);
		std::string id = obj["id"];
		const int depth = obj["depth"];

		GameObject* pTempObject = nullptr;
		if (type == "dungeon") pTempObject = new Dungeon(pos, depth, id, pManager, objects);
		else if (type == "link")
		{
			if (pSaveData)
			{
				pos = pSaveData->playerPosition;
				box = CreateBox(pos, dim);
				pTempObject = new Link(pos, depth, box, id, pManager, objects);
				pLink = pTempObject;
				float saveHp = pSaveData->playerHealth;
				static_cast<Link*>(pTempObject)->SetSaveHealth(saveHp);
			}
			else
			{
				pTempObject = new Link(pos, depth, box, id, pManager, objects);
				pLink = pTempObject;
			}
		}
		else if (type == "rupee")
		{
			if (pSaveData)
			{
				pos = pSaveData->rupeePosition[pSaveData->rupeeCount];
				--pSaveData->rupeeCount;
				box = CreateBox(pos, dim);
				pTempObject = new Rupee(pos, depth, box, id, pManager, objects);
			}
			else
				pTempObject = new Rupee(pos, depth, box, id, pManager, objects);
		}
		else if (type == "darknut")
		{
			if (pSaveData)
			{
				pos = pSaveData->enemyPositions[pSaveData->enemyCount];
				--pSaveData->enemyCount;
				box = CreateBox(pos, dim);
				pTempObject = new Rupee(pos, depth, box, id, pManager, objects);
			}
			else
				pTempObject = new Rupee(pos, depth, box, id, pManager, objects);
		}
		else if (type == "block")
		{
			if (pSaveData)
			{
				pos = pSaveData->blockPositions[pSaveData->blockCount];
				--pSaveData->blockCount;
				box = CreateBox(pos, dim);
				pTempObject = new Block(pos, depth, box, id, pManager, objects);
			}
			else
				pTempObject = new Block(pos, depth, box, id, pManager, objects);
		}
		else if (type == "fire")
		{
			if (pSaveData)
			{
				pos = pSaveData->firePositions[pSaveData->fireCount];
				--pSaveData->fireCount;
				box = CreateBox(pos, dim);
				pTempObject = new Fire(pos, depth, box, id, pManager, objects);
			}
			else
				pTempObject = new Fire(pos, depth, box, id, pManager, objects);
		}
		else if (type == "heart") pTempObject = new Heart(pos, depth, box, id, pManager, objects);
		else if (type == "triforce")
		{
			if (pSaveData)
			{
				pos = pSaveData->triforcePosition;
				box = CreateBox(pos, dim);
				pTempObject = new Triforce(pos, depth, box, id, pManager, objects);
			}
			else
				pTempObject = new Triforce(pos, depth, box, id, pManager, objects);
		}
		else if (type == "plate")
		{
			pTempObject = new Plate(pos, depth, box, id, pManager, objects);
			pPlate = pTempObject; // Store plate pointer
		}
		else if (type == "trap")pTempObject = new Trap(pos, depth, box, id, pManager, pPlate, objects);
		else if (type == "staticimage") pTempObject = new StaticImage(pos, depth, id, objects);
		else if (type == "statictext") pTempObject = new StaticText(pos, depth, id, objects);
		else if (type == "button") pTempObject = new Button(pos, depth, box, id, objects);
		else if (type == "slider") pTempObject = new Slider(pos, depth, id, objects);
		else if (type == "bar") pTempObject = new Bar(pos, depth, id, objects);
		else if (type == "boss")
		{
			if (pSaveData)
			{
				pos = pSaveData->bossPosition;
				box = CreateBox(pos, dim);
				pTempObject = new Boss(pos, depth, box, id, pManager, objects);
			}
			else
				pTempObject = new Boss(pos, depth, box, id, pManager, objects);
		}

		if (pTempObject)
		{
			const size_t compCount = obj["components"].size();
			for (size_t j = 0; j < compCount; ++j)
			{
				json& comp = obj["components"][j];
				std::string compType = comp["type"];
				if (compType == "Texture")
				{
					const std::string sprite = comp["sprite"];
					const std::string compid = comp["id"];
					pTempObject->SetComponent(compid, new TextureComponent{ GrabTexture(sprite), box });
				}
				else if (compType == "TextureDim")
				{
					const std::string sprite = comp["sprite"];
					const std::string compid = comp["id"];
					Vector2 compPos{ comp["position"][0], comp["position"][1] };
					box.x = compPos.m_x;
					box.y = compPos.m_y;
					Vector2 compDim{ comp["dimension"][0], comp["dimension"][1] };
					box.w = compDim.m_x;
					box.h = compDim.m_y;
					pTempObject->SetComponent(compid, new TextureComponent{ GrabTexture(sprite), box });
				}
				else if (compType == "Animation")
				{
					double frameRate = comp["framerate"];
					const std::string sprite = comp["sprite"];
					AnimationComponent* pAnimationComponent = new AnimationComponent{ dim, box, frameRate, 0, GrabTexture(sprite) };
					for (const auto& seq : comp["sequence"])
					{
						std::string name = seq["name"];
						int first = seq["first"];
						int last = seq["last"];
						pAnimationComponent->AddSequence(name, first, last);
					}
					pAnimationComponent->PlayAnimation(comp["sequence"][0]["name"]);
					pTempObject->SetComponent(compType, pAnimationComponent);
				}
				else if (compType == "Text")
				{
					std::string text = comp["text"];
					std::string font = comp["font"];
					Vector2 textpos{ comp["position"][0], comp["position"][1] };
					SDL_Color black{ 0, 0, 0, 0 };
					TextComponent* pTextComponent = new TextComponent(textpos, GrabFont(font), text.c_str(), black, m_pRenderer);
					pTempObject->SetComponent(compType, pTextComponent);
				}
				else if (compType == "Sound")
				{
					SoundComponent* pSoundComponent = new SoundComponent;
					for (const auto& seq : comp["sequence"])
					{
						std::string name = seq["name"];
						std::string path = seq["path"];

						pSoundComponent->AddSound({ name, GrabSound(path) });
					}
					pTempObject->SetComponent(compType, pSoundComponent);
				}
				else if (compType == "Particle")
				{
					int particleCount = comp["max"];
					float speed = comp["maxspeed"];
					float radius = comp["radius"];
					size_t size = comp["size"];
					ParticleComponent::Direction direction = comp["direction"];
					Vector3 color;
					color.x = comp["color"][0];
					color.y = comp["color"][1];
					color.z = comp["color"][2];
					ParticleComponent* pParticle = new ParticleComponent(pos, particleCount, speed, radius, direction);
					pParticle->SetParticleSize(size);
					pParticle->SetColor(color);

					pTempObject->SetComponent(compType, pParticle);
				}
			}
			objects.push_back(pTempObject);
		}
	}

	if (std::string music = m_file["scene"]["music"]; music != "NULL")
		pSceneMusic = GrabMusic(music);

	DepthSort(objects);
	std::cout << "Scene Loaded\n";
}

void SceneFactory::LoadSprites()
{
	std::cout << '\n';
	std::cout << "Loading Textures from Sprite.json\n";

	// Open and parse the Sprite.json m_file
	std::ifstream file("config/Sprite.json");
	if (!file.is_open())
	{
		std::cout << "Could not open Sprite.json\n";
		return;
	}
	json data = json::parse(file);

	const size_t spriteCount = data["sprites"].size();

	m_textures.resize(spriteCount);
	for (size_t i = 0; i < spriteCount; ++i)
	{
		std::string spritePath = data["sprites"][i];

		SDL_Surface* pTempSurface = IMG_Load(spritePath.c_str());
		if (!pTempSurface)
		{
			std::cout << "Failed to load " << spritePath << ": " << SDL_GetError() << '\n';
			continue;
		}
		SDL_Texture* pTempTexture = SDL_CreateTextureFromSurface(m_pRenderer, pTempSurface);
		SDL_DestroySurface(pTempSurface);
		if (!pTempTexture)
		{
			std::cout << "Failed to create texture from " << spritePath << ": " << SDL_GetError() << '\n';
			continue;
		}

		// Store full path as name
		m_textures[i].m_pData = pTempTexture;
		m_textures[i].m_name = spritePath;
		std::cout << "Loaded Texture: " << m_textures[i].m_name << '\n';
	}
	std::cout << '\n';
}

void SceneFactory::LoadFont()
{
	std::cout << "Loading Fonts From Fonts.json\n";

	// Open and parse the Fonts.json m_file
	std::ifstream file("config/Fonts.json");
	if (!file.is_open())
	{
		std::cout << "Could not open Fonts.json\n";
		return;
	}
	json data = json::parse(file);
	const size_t fontCount = data["fonts"].size();
	m_fonts.resize(fontCount);

	for (size_t i = 0; i < fontCount; ++i)
	{
		std::string fontPath = data["fonts"][i];

		m_fonts[i].m_name = fontPath;
		m_fonts[i].m_pData = TTF_OpenFont(fontPath.c_str(), 19);
		std::cout << "Loaded Font: " << fontPath << '\n';
	}
	std::cout << '\n';
}

void SceneFactory::LoadMusic()
{
	std::cout << "Loading Music From Music.json\n";
	std::ifstream file("config/Music.json");
	if (!file.is_open())
	{
		std::cout << "Could not open Music.json\n";
		return;
	}
	json data = json::parse(file);
	const size_t musicCount = data["music"].size();
	m_music.resize(musicCount);

	for (size_t i = 0; i < musicCount; ++i)
	{
		std::string path = data["music"][i];
		m_music[i].m_name = path;
		m_music[i].m_pData = Mix_LoadMUS(path.c_str());
		std::cout << "Loaded Music: " << path << '\n';
	}
	std::cout << '\n';
}

void SceneFactory::LoadSounds()
{
	std::cout << "Loading Sounds From Sounds.json\n";
	std::ifstream file("config/Sounds.json");
	if (!file.is_open())
	{
		std::cout << "Could not open Sounds.json\n";
		return;
	}
	json data = json::parse(file);
	const size_t soundCount = data["sounds"].size();
	m_sound.resize(soundCount);

	for (size_t i = 0; i < soundCount; ++i)
	{
		std::string path = data["sounds"][i];
		m_sound[i].m_name = path;
		m_sound[i].m_pData = Mix_LoadWAV(path.c_str());
		std::cout << "Loaded Sound: " << path << '\n';
	}
	std::cout << '\n';

}

SDL_Texture* SceneFactory::GrabTexture(const std::string& name)
{
	for (const auto& pkg : m_textures)
		if (name == pkg.m_name)
			return pkg.m_pData;
	std::cout << "Texture Not Found: " << name << '\n';
	return nullptr;
}

TTF_Font* SceneFactory::GrabFont(const std::string& name)
{
	for (const auto& pkg : m_fonts)
		if (name == pkg.m_name)
			return pkg.m_pData;
	std::cout << "Font Not Found: " << name << '\n';
	return nullptr;
}

Mix_Music* SceneFactory::GrabMusic(const std::string& name)
{
	for (const auto& pkg : m_music)
		if (name == pkg.m_name)
			return pkg.m_pData;
	std::cout << "Music Not Found: " << name << '\n';
	return nullptr;
}

Mix_Chunk* SceneFactory::GrabSound(const std::string& name)
{
	for (const auto& pkg : m_sound)
		if (name == pkg.m_name)
			return pkg.m_pData;
	std::cout << "Sound Not Found: " << name << '\n';
	return nullptr;
}


SDL_FRect SceneFactory::CreateBox(const Vector2& pos, const Vector2& dim)
{
	return SDL_FRect{ pos.m_x, pos.m_y, dim.m_x, dim.m_y };
}

void SceneFactory::DepthSort(std::vector<GameObject*>& objects)
{
	// Mini bubble sort to sort if depth is out of order
	const size_t n = objects.size();
	for (size_t i = 0; i < n - 1; ++i)
	{
		for (size_t j = 0; j < n - i - 1; ++j)
		{
			if (objects[j]->GetDepth() > objects[j + 1]->GetDepth())
			{
				// Swap elements if out of order
				GameObject* temp = objects[j];
				objects[j] = objects[j + 1];
				objects[j + 1] = temp;
			}
		}
	}

	for (size_t i = 0; i < n; ++i)
		objects[i]->Awake();
}