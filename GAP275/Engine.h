#pragma once

#include "SceneStateMachine.h"
#include "Window.h"
struct SDL_Renderer;

// Game Engine
class Engine final
{
private:
	static constexpr int kWidth = 800;
	static constexpr int kHeight = 600;
public:
	Engine();
	void Awake();
	void Run();
	void Sleep() const;
private:
	SceneStateMachine m_stateMachine;
	Window m_window;
	SDL_Renderer* m_pRenderer;
	bool m_isRunning;

	void ProcessEvents();
	void Update();
	void Render();
};