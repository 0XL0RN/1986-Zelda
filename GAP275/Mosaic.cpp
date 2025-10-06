#include "Mosaic.h"
#include <SDL3/SDL.h>
#include <iostream>
#include "ShapeFactory.h"

//////////////////////////////////////////////////////////////
// Constructor, constructs and init the rectangles and window
//////////////////////////////////////////////////////////////
Mosaic::Mosaic()
	: m_window("Mosaic", kWidth, kHeight)	// Initialize the window with title, width, and height
	, m_pRenderer(SDL_CreateRenderer(m_window.GetWindow(), nullptr))	// Create an SDL renderer
{
	// Generate a rectangle using the ShapeFactory and add it to m_shapes
	ShapeFactory::GenerateRectangle(m_shapes, m_pRenderer);
}

/////////////////////////////////////////////////////
// Awake function to initialize SDL and create shapes
/////////////////////////////////////////////////////
int Mosaic::Awake() const
{
	// Initialize SDL video subsystem
	const bool errorCode = SDL_Init(SDL_INIT_VIDEO);
	if (errorCode != true)
	{
		std::cout << "SDL Failed To Init\n";
		return 0;
	}
	else
		std::cout << "SDL Successfully Initialized\n";

	// Check the status of the window
	m_window.GetStatus();

	// Check if the renderer was created successfully
	if (m_pRenderer == nullptr)
		std::cout << "SDL Renderer Failed To Init\n";
	else
		std::cout << "SDL Renderer Created Successfully\n";

	// Draw various shapes using ShapeFactory (I used random values to display them on screen)
	ShapeFactory::DrawUnfilledTriangle(m_pRenderer, 200, 200, 250, 200, 200, 150);
	ShapeFactory::DrawUnfilledPentagon(m_pRenderer, 300, 300, 100);
	ShapeFactory::DrawUnfilledRectangle(m_pRenderer, 100, 100, 50, 80);

	// Present the renderer (display the drawn shapes)
	SDL_RenderPresent(m_pRenderer);

	system("pause");
	return errorCode;
}

///////////////////////////////////////
// Sleep function to clean up resources
///////////////////////////////////////
int Mosaic::Sleep() const
{
	// Destroy the SDL renderer
	SDL_DestroyRenderer(m_pRenderer);

	// Call the destructor of the Window object
	m_window.~Window();

	// Quit SDL
	SDL_Quit();

	// Return 1 to indicate successful cleanup
	return 1;
}
