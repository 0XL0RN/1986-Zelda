#pragma once

#include "Vector2.h"
struct SDL_Window;

// Window class for managing SDL window creation and properties
class Window final
{
private:
	using Vector2 = Vector2<float>;
	static constexpr int kDefaultWidth = 640;
	static constexpr int kDefaultHeight = 480;
public:
	Window();
	Window(const char* title, const float& x, const float& y);
	~Window();
	SDL_Window* GetWindow() const { return m_pWindow; }
	void GetStatus() const;
private:
	SDL_Window* m_pWindow;
	const char* m_title;
	const Vector2 m_screenSpace;
};