#include "SoundComponent.h"
#include "SDL_mixer.h"

SoundComponent::SoundComponent()
	: m_pActiveChuck(nullptr)
	, m_channelID(0)
	, m_size(0)
{

}

SoundComponent::~SoundComponent()
{

}

void SoundComponent::Update([[maybe_unused]] double deltaTime)
{

}

void SoundComponent::Render([[maybe_unused]] SDL_Renderer* pRenderer)
{

}

void SoundComponent::Pause() const
{
	Mix_HaltChannel(m_channelID);
}

void SoundComponent::Play(const std::string& sound, const int loops)
{
	for (auto& [name, pChunk] : m_sounds)
	{
		if (name == sound)
		{
			m_pActiveChuck = pChunk;
			m_channelID = Mix_PlayChannel(-1, m_pActiveChuck, loops);
		}
	}
}

void SoundComponent::PlayLoop(int loops)
{
	m_channelID = Mix_PlayChannel(-1, m_pActiveChuck, loops);
}

void SoundComponent::AddSound(SoundPackage package)
{
	if (m_size < kMaxSounds)
		m_sounds[m_size] = package;
	++m_size;
}

void SoundComponent::SetVolume(int volume)
{
	Mix_VolumeChunk(m_pActiveChuck, volume);
}

void SoundComponent::SetSound(const std::string& sound)
{
	for (auto& [name, pChunk] : m_sounds)
	{
		if (name == sound)
		{
			m_pActiveChuck = pChunk;
		}
	}
}
