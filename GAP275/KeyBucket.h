#pragma once

#include <SDL3/SDL_scancode.h>

// Bucket that holds our keyboard scan keys
class KeyBucket final
{
private:
	// Room for WASD
	static constexpr size_t kDefaultCapacity = 4;

	struct Pair
	{
		SDL_Scancode m_first; // Keycode
		bool m_second;		  // SceneState (pressed or not)
		Pair(const SDL_Scancode& first, const bool second) : m_first(first), m_second(second) {}
		Pair() = default;
	};
public:
	KeyBucket();
	~KeyBucket();
	KeyBucket(const KeyBucket& other) = delete;
	KeyBucket& operator=(const KeyBucket& other) = delete;
	bool& operator[](const SDL_Scancode& key);

private:
	Pair* m_bucket;
	size_t m_size;
	size_t m_capacity;
};