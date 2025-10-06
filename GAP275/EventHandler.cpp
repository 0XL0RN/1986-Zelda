#include "EventHandler.h"
#include <iostream>
#include <SDL3/SDL.h>

KeyBucket EventHandler::m_keys;
bool EventHandler::m_mouseIsClicked = false;
Vector2<float> EventHandler::m_mousePosition = { 0.0f, 0.0f };

// Processes all SDL events in the queue, returns false to quit
bool EventHandler::ProcessEvent()
{
	// Holds the current event from SDL’s event queue
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// Switch based on event type
		switch (event.type)
		{
		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP:
			ProcessKeyboardEvent(&event.key);
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			ProcessMouseEvent(&event.button);
			break;
		case SDL_EVENT_QUIT:
			return false;
		}
	}
	return true;
}

// Updates key states based on SDL keyboard events
void EventHandler::ProcessKeyboardEvent(const SDL_KeyboardEvent* pEvent)
{
	// Set the key state in m_keys: true for press (KEY_DOWN), false for release (KEY_UP)
	m_keys[pEvent->scancode] = (pEvent->type == SDL_EVENT_KEY_DOWN);
}

// Handles mouse button press events
void EventHandler::ProcessMouseEvent(const SDL_MouseButtonEvent* pEvent)
{
	// Check if it’s a left-click
	if (pEvent->button == SDL_BUTTON_LEFT)
	{
		// Mark that a click occurred
		m_mouseIsClicked = true;
		// Stores Mouse Position
		m_mousePosition.m_x = pEvent->x;
		m_mousePosition.m_y = pEvent->y;
		std::cout << "Click @: " << m_mousePosition.m_x << " " << m_mousePosition.m_y << "\n";
	}
}

// Checks if a specific key is currently pressed
bool EventHandler::IsKeyDown(const SDL_Scancode& code)
{
	// Returns true if key is pressed, false if not
	return m_keys[code];
}

// Checks if a mouse button was clicked and returns its position
bool EventHandler::IsMouseDown(Vector2& position)
{
	// If a click was processed this frame
	if (m_mouseIsClicked)
	{
		// Copy click position to output parameter
		position = m_mousePosition;
		// Reset click flag after handling
		m_mouseIsClicked = false;
		// return true as a click occurred
		return true;
	}
	return false;
}

void EventHandler::Clear()
{
	m_keys[SDL_SCANCODE_W] = false;
	m_keys[SDL_SCANCODE_A] = false;
	m_keys[SDL_SCANCODE_S] = false;
	m_keys[SDL_SCANCODE_D] = false;
}
