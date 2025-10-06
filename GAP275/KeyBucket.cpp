#include "KeyBucket.h"

KeyBucket::KeyBucket()
	: m_bucket(new Pair[kDefaultCapacity]())
	, m_size(0)
	, m_capacity(kDefaultCapacity)
{
}

KeyBucket::~KeyBucket()
{
	// Clean up dynamic array
	delete[] m_bucket;
}

bool& KeyBucket::operator[](const SDL_Scancode& key)
{
	// Look for existing key
	for (size_t i = 0; i < m_size; ++i) 
	{
		if (m_bucket[i].m_first == key) 
		{
			// Found: return state
			return m_bucket[i].m_second;
		}
	}

	// Key not found: add it if room exists
	if (m_size < m_capacity) 
	{
		// Add with default false
		m_bucket[m_size] = Pair(key, false);
		return m_bucket[m_size++].m_second;
	}
	static bool dummy = false;
	return dummy;
}