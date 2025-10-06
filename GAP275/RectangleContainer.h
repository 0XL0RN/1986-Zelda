#pragma once

#include "SDL3/SDL.h"

////////////////////////////////////////////////////
// RectangleContainer used to store rectangle shapes
////////////////////////////////////////////////////
class RectangleContainer final
{
public:
	RectangleContainer();
	~RectangleContainer();
	void Init(const size_t& size);
	void Add(SDL_FRect* pRectangle);
	SDL_FRect* GetAt(const size_t& index) const;
private:
	SDL_FRect** m_pRectangles;
	size_t m_length;
	size_t m_occupied;
};