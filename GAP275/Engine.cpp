#include "Engine.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "SDL_mixer.h"
#include "TimeHandler.h"

Engine::Engine()
	: m_window("Zelda ", kWidth, kHeight)
	, m_pRenderer(nullptr)
	, m_isRunning(true)
{
}

// Awakens the game engine
void Engine::Awake()
{
	// SDL Init
	const bool errorCode = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (errorCode != true)
		std::cout << "SDL Failed To Init\n";
	else
		std::cout << "SDL Successfully Initialized\n";

	m_window.GetStatus();

	// Renderer Init
	m_pRenderer = SDL_CreateRenderer(m_window.GetWindow(), nullptr);
	if (m_pRenderer != nullptr)
		std::cout << "Renderer Created Successfully\n";
	else
		std::cout << "Renderer Creation Failed\n";

	const bool errorCodeTTF = TTF_Init();
	if (errorCodeTTF == 0)
		std::cout << "Failed To Load TTF Loader\n";
	else
		std::cout << "TTF Loader Successfully Initialized\n";

	const bool errorCodeMixer = Mix_OpenAudio(0, nullptr);
	if (errorCodeMixer == 1)
		std::cout << "Mixer Successfully Initialized\n";
	else
		std::cout << "Failed To Load Mixer\n";

	// Awaken StateMachine
	m_stateMachine.Awake(m_pRenderer);
}

// Main engine game loop
void Engine::Run()
{
	std::cout << "Engine Running\n";
	// Record First Frame time
	TimeHandler::RecordTime();
	while (m_isRunning)
	{
		// Calculate delta time based on frame prev and curr frames
		TimeHandler::CalculateDeltaTime();
		ProcessEvents();
		Update();
		Render();
	}
}

// Puts the game engine to sleep
void Engine::Sleep() const
{
	Mix_CloseAudio();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_window.GetWindow());
	SDL_Quit();
	std::cout << "All Systems Have Gone To Sleep zZz\n";
}

// Processes all scene events
void Engine::ProcessEvents()
{
	// Process Events
	m_isRunning = m_stateMachine.HandleEvent();
}

// Updates Scene
void Engine::Update()
{
	const double deltaTime = TimeHandler::GetDeltaTime();
		m_stateMachine.UpdateState(deltaTime);
}

// Renders scene followed by presenting
void Engine::Render()
{
	SDL_RenderClear(m_pRenderer);

	// Render
	m_stateMachine.RenderState(m_pRenderer);

	// Present
	SDL_RenderPresent(m_pRenderer);
}
