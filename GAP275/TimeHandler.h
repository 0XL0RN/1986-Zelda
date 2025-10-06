#pragma once

#include <chrono>

// Handles DeltaTime Calculations
class TimeHandler final
{
public:
	// type alias for a time point using steady_clock
	using ChronoClock = std::chrono::steady_clock::time_point;
	static void RecordTime();
	static void CalculateDeltaTime();
	// Returns the delta time in seconds
	static float GetDeltaTime() { return m_deltaTime; }
private:
	// Stores the delta time in seconds, accessible across all instances
	inline static float m_deltaTime = 0.0f;
	// Stores the timestamp of the last frame, static for shared use
	inline static ChronoClock m_lastFrameTime = std::chrono::high_resolution_clock::now();
};