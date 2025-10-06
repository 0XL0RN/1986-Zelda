#pragma once

#include "IComponent.h"
#include <string>
struct Mix_Chunk;

class SoundComponent final : public IComponent
{
	static constexpr size_t kMaxSounds = 5;
private:
	struct SoundPackage
	{
	public:
		std::string name = "NULL";
		Mix_Chunk* pChunk = nullptr;
	};
public:
	explicit SoundComponent();
	~SoundComponent() override;
	void Update(double deltaTime) override;
	void Render(SDL_Renderer* pRenderer) override;
	void Pause() const;
	void Play(const std::string& sound, const int loops);
	void PlayLoop(int loops);
	void AddSound(SoundPackage package);
	void SetVolume(int volume);
	void SetSound(const std::string& sound);
private:
	SoundPackage m_sounds[kMaxSounds];
	Mix_Chunk* m_pActiveChuck;
	int m_channelID;
	size_t m_size;
};