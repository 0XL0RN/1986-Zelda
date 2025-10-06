#pragma once

class AiState
{
public:
	virtual ~AiState() = default;
	virtual void Enter() = 0;
	virtual void Update(const double& deltaTime) = 0;
	virtual void Exit() = 0;
};