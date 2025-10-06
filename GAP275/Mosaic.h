#pragma once

#include <SDL3/SDL.h>
#include "Window.h"
#include "RectangleContainer.h"

///////////////////////////////////////////////////
// Mosaic is for rendering rectangles to the screen
///////////////////////////////////////////////////
class Mosaic final
{
private:
	// window size width and height
	static constexpr int kWidth = 400;
	static constexpr int kHeight = 400;
public:
	Mosaic();
	int Awake() const;
	int Sleep() const;
private:
	Window m_window;
	SDL_Renderer* m_pRenderer;
	RectangleContainer m_shapes;
};
