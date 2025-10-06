#include "ParticleComponent.h"
#include "mt19937.h"
#include <SDL3/SDL.h>

ParticleComponent::ParticleComponent(const Vector2& centerPoint, const int& particleCount, const float& maxSpeed, const float& radiusSqr, const Direction& direction)
	: m_centerPoint(centerPoint)
	, m_particleCount(particleCount)
	, m_radiusSqr(radiusSqr* radiusSqr)
	, m_pParticles(new Particle[particleCount])
	, m_maxSpeed(maxSpeed)
	, m_particleSize(1)
	, m_color(0, 0, 0)
{
	Generate(direction);
}

ParticleComponent::~ParticleComponent()
{
	for (size_t i = 0; i < m_particleCount; ++i)
		delete[] m_pParticles;
}

void ParticleComponent::Update(const double deltaTime)
{
	for (size_t i = 0; i < m_particleCount; ++i)
	{
		if (m_pParticles[i].isAlive)
		{
			m_pParticles[i].offset.m_x += m_pParticles[i].velocity.m_x * static_cast<float>(deltaTime);
			m_pParticles[i].offset.m_y += m_pParticles[i].velocity.m_y * static_cast<float>(deltaTime);

			if (m_pParticles[i].offset.GetLength() >= m_radiusSqr)
			{
				m_pParticles[i].offset = Vector2{ 0, 0 };
				m_pParticles[i].isAlive = false;
			}
		}
	}
}

void ParticleComponent::Render(SDL_Renderer* pRenderer)
{
	SDL_SetRenderDrawColor(pRenderer, static_cast<Uint8>(m_color.x), static_cast<Uint8>(m_color.y), static_cast<Uint8>(m_color.z), 255);
	for (size_t i = 0; i < m_particleCount; ++i)
	{
		const auto& [offset, velocity, alive] = m_pParticles[i];
		const SDL_FRect transform(m_centerPoint.m_x + offset.m_x, m_centerPoint.m_y + offset.m_y, static_cast<float>(m_particleSize), static_cast<float>(m_particleSize));
		SDL_RenderFillRect(pRenderer, &transform);
	}
}

void ParticleComponent::Generate(const Direction& direction) const
{
	mt19937 twister;

	switch (direction)
	{
	case Direction::kUp:
	{
		for (size_t i = 0; i < m_particleCount; ++i)
		{
			const float x = twister.GenerateRange(-1.0f, 1.0f);
			const float y = twister.GenerateRange(-1.0f, 0.0f);

			m_pParticles[i].offset = Vector2{ .m_x = 0, .m_y = 0 };
			m_pParticles[i].velocity = Vector2{ x, y } *m_maxSpeed;
			m_pParticles[i].isAlive = false;
		}
		break;
	}
	case Direction::kDown: break;
	case Direction::kLeft: break;
	case Direction::kRight: break;
	case Direction::kRandom:
	{
		for (size_t i = 0; i < m_particleCount; ++i)
		{
			const float x = twister.GenerateRange(-1.0f, 1.0f);
			const float y = twister.GenerateRange(-1.0f, 1.0f);

			m_pParticles[i].offset = Vector2{ .m_x = 0, .m_y = 0 };
			m_pParticles[i].velocity = Vector2{ x, y } *m_maxSpeed;
			m_pParticles[i].isAlive = false;

		}
		break;
	}
	}
}

void ParticleComponent::Splash()
{
	for (size_t i = 0; i < m_particleCount; ++i)
		m_pParticles[i].isAlive = true;
}
