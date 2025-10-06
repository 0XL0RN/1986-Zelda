#pragma once

#include "json.hpp"
#include <SDL3/SDL.h>
#include "SceneSaver.h"
#include "Vector2.h"
class CollisionManager;
class GameObject;
struct TTF_Font;
struct Mix_Music;
struct Mix_Chunk;

using json = nlohmann::json;

struct ScenePackage
{
public:
	std::vector<GameObject*>& objects;
	CollisionManager* pCollisionManager;
	Mix_Music* pSceneMusic;
};

class SceneFactory final
{
private:
	using Vector2 = Vector2<float>;

	template<typename T>
	struct Package
	{
		T* m_pData = nullptr;
		std::string m_name = "NULL";
	};
public:
	SceneFactory();
	~SceneFactory();
	void LoadFile(const std::string& path);
	void Awake(SDL_Renderer* pRenderer);
	void LoadScene(std::vector<GameObject*>& objects, CollisionManager* pManager, SceneSaver::SaveData* pSaveData, Mix_Music*& pSceneMusic);
	inline static SDL_Texture* GrabTexture(const std::string& name);
	inline static TTF_Font* GrabFont(const std::string& name);
	inline static Mix_Music* GrabMusic(const std::string& name);
	inline static Mix_Chunk* GrabSound(const std::string& name);
	private:
	json m_file;
	size_t m_objCount;
	inline static std::vector<Package<SDL_Texture>> m_textures;
	inline static std::vector<Package<TTF_Font>> m_fonts;
	inline static std::vector<Package<Mix_Music>> m_music;
	inline static std::vector<Package<Mix_Chunk>> m_sound;
	SDL_Renderer* m_pRenderer;

	void LoadSprites();
	void LoadFont();
	void LoadMusic();
	void LoadSounds();
	inline static SDL_FRect CreateBox(const Vector2& pos, const Vector2& dim);
	inline static void DepthSort(std::vector<GameObject*>& objects);
};