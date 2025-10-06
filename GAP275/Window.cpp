#include "Window.h"
#include "SDL3/SDL.h"
#include <iostream>

// Default constructor
Window::Window()
	: m_pWindow(nullptr)
	, m_title("Window")
	, m_screenSpace{ kDefaultWidth, kDefaultHeight }
{
	// Create SDL window with default title and dimensions
	m_pWindow = SDL_CreateWindow(m_title, static_cast<int>(m_screenSpace.m_x), static_cast<int>(m_screenSpace.m_y), SDL_WINDOW_OPENGL);
}

// Parameterized constructor
Window::Window(const char* title, const float& x, const float& y)
	: m_pWindow(nullptr)
	, m_title(title)
	, m_screenSpace{ x, y }
{
	// Create SDL window with specified title and dimensions
	m_pWindow = SDL_CreateWindow(m_title, static_cast<int>(m_screenSpace.m_x), static_cast<int>(m_screenSpace.m_y), SDL_WINDOW_OPENGL);
}

// Destructor
Window::~Window()
{
	// Clean up SDL window
	SDL_DestroyWindow(m_pWindow);
}

// Method to check and print window creation status
void Window::GetStatus() const
{
	if (m_pWindow == nullptr)
		std::cout << "SDL Window Creation Failed\n";
	else
		std::cout << "SDL Window Created Successfully\n";
}