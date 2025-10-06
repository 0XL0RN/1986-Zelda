#pragma once

struct SDL_Renderer;
class IComponent
{
public:
	virtual ~IComponent() = default;
	virtual void Update(double deltaTime) = 0;
	virtual void Render(SDL_Renderer* pRenderer) = 0;
};
