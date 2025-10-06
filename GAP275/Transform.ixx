export module Transform;

import Component;
import <SDL3/SDL.h>;

export class Transform final : public Component
{
public:
	inline static int m_id = 1;
	Transform();

private:
	SDL_FRect m_rectangle;
};