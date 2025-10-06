#pragma once

#include "IComponent.h"
#include "Vector2.h"
#include "Vector3.h"

class ParticleComponent final : public IComponent
{
private:
	using Vector2 = Vector2<float>;
	struct Particle
	{
	public:
		Vector2 offset;
		Vector2 velocity;
		bool isAlive;
	};
public:
	enum class Direction : uint8_t
	{
		kUp,
		kDown,
		kLeft,
		kRight,
		kRandom
	};

	ParticleComponent(const Vector2& centerPoint, const int& particleCount, const float& maxSpeed, const float& radiusSqr, const Direction& direction);
	~ParticleComponent() override;

	void Update(double deltaTime) override;
	void Render(SDL_Renderer* pRenderer) override;
	void Splash(double deltaTime);
	[[nodiscard]] Vector2 GetPosition() const { return m_centerPoint; }
	void SetPosition(const Vector2& position) { m_centerPoint = position; }
	void SetColor(const Vector3& color) { m_color = color; }
	void SetParticleSize(const size_t& particleSize) { m_particleSize = particleSize; }
	void Generate(const Direction& direction) const;
	void Splash();
private:
	Vector2 m_centerPoint;
	const int m_particleCount;
	const float m_radiusSqr;
	Particle* m_pParticles;
	float m_maxSpeed;
	size_t m_particleSize;
	Vector3 m_color;
	bool m_splash;
};