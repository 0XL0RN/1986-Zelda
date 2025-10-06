#include "TimeHandler.h"

// Records the current time as the last frame’s timestamp
void TimeHandler::RecordTime()
{
	// Sets m_lastFrameTime to the current time using high_resolution_clock
	m_lastFrameTime = std::chrono::high_resolution_clock::now();
}

// Calculates the time elapsed since the last frame (delta time)
void TimeHandler::CalculateDeltaTime()
{
	// Captures the current time at the start of this frame
	const ChronoClock currentFrameTime = std::chrono::high_resolution_clock::now();
	// Computes the duration between now and the last frame in seconds (as float)
	const std::chrono::duration<float> lastFrameDuration = currentFrameTime - m_lastFrameTime;
	// Extracts the duration in seconds and stores it as delta time
	m_deltaTime = lastFrameDuration.count();
	// Updates m_lastFrameTime to the current time for the next calculation
	m_lastFrameTime = currentFrameTime;
}