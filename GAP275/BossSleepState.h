#pragma once

#include "AiState.h"

class BossSleepState final : public AiState
{
public:
	BossSleepState();
	~BossSleepState() override;

	void Enter() override;
	void Update(const double& deltaTime) override;
	void Exit() override;
private:

};