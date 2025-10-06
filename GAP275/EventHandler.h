#pragma once

#include <SDL3/SDL_scancode.h>
#include "KeyBucket.h"
#include "Vector2.h"

struct SDL_KeyboardEvent;
struct SDL_MouseButtonEvent;

// Event Handler for any input from player
// TODO make singleton
class EventHandler final
{
private:
	using Vector2 = Vector2<float>;
public:
	static bool ProcessEvent();
	static void ProcessKeyboardEvent(const SDL_KeyboardEvent* pEvent);
	static void ProcessMouseEvent(const SDL_MouseButtonEvent* pEvent);
	static bool IsKeyDown(const SDL_Scancode& code);
	static bool IsMouseDown(Vector2& position);
	static void Clear();
private:
	static KeyBucket m_keys;
	static bool m_mouseIsClicked;
	static Vector2 m_mousePosition;
};