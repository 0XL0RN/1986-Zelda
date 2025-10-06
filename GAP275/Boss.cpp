#include "Boss.h"
#include "AnimationComponent.h"
#include "ParticleComponent.h"
#include "Link.h"

Boss::Boss(const Vector2& position, const int& depth, [[maybe_unused]] const SDL_FRect box, const std::string& id, CollisionManager* pManager, std::vector<GameObject*>& objects)
	: GameObject(position, depth, id, objects)
	, m_stateMachine(this)
	, m_pTarget(nullptr)
	, m_hitBox
	{
		{{{position.m_x + 32	, position.m_y, 32, 32}, this, pManager},{32, 0}}, // Top
		{{{position.m_x + 32, position.m_y + 64, 32, 32}, this, pManager},{32, 64}}, // Bottom
		{{{position.m_x + 64, position.m_y + 32, 32, 32}, this, pManager},{64, 32}}, // Right
		{{{position.m_x, position.m_y + 32, 32, 32}, this, pManager},{0, 32}}, // left
	}
	, m_hp(kMaxHp)
	, m_damage(-1)
	, m_attack(false)
	, m_attackTimer(1.0f)
{
	RegisterEvent(EventType::kCollision, [this](const Event& event) -> void
		{
			if (event.m_sender == &m_hitBox[0].m_rect || event.m_sender == &m_hitBox[1].m_rect || event.m_sender == &m_hitBox[2].m_rect || event.m_sender == &m_hitBox[3].m_rect)
			{
				if (const auto pObject = static_cast<Link*>(event.m_receiver); pObject->GetId() == "Link")
				{
					if (m_attackTimer >= 1.0f)
					{
						pObject->SetHealth(static_cast<float>(m_damage));
						m_attackTimer = 0.0f;
					}
				}
			}

		});
}

void Boss::Awake()
{
	m_pTarget = GetObject("Link");
	m_stateMachine.Awake(BossStateMachine::StateType::kSleep, m_pTarget);
}

void Boss::Update([[maybe_unused]] const double& deltaTime)
{
	if (IsAlive())
	{
		m_attackTimer += static_cast<float>(deltaTime);

		GetComponent("Animation")->Update(deltaTime);

		if (m_hp >= 61 && m_hp <= 100)
			m_stateMachine.RotateState(BossStateMachine::StateType::kSleep);
		else if (m_hp >= 31 && m_hp <= 60)
			m_stateMachine.RotateState(BossStateMachine::StateType::kWander);
		else if (m_hp >= 1 && m_hp <= 30)
			m_stateMachine.RotateState(BossStateMachine::StateType::kChase);

		m_stateMachine.Update(deltaTime);
		GetComponent("Particle")->Update(deltaTime);
	}
	else
	{
		for (size_t i = 0; i < kMaxHitBox; ++i)
		{
			m_hitBox[i].m_rect.~Collider();
		}
	}
}

void Boss::Draw(SDL_Renderer* pRenderer)
{
	if (IsAlive())
	{
		GetComponent("Particle")->Render(pRenderer);
		GetComponent("Animation")->Render(pRenderer);

		/*SDL_SetRenderDrawColor(pRenderer, 255, 0, 100, 50);
		for (size_t i = 0; i < kMaxHitBox; ++i)
		{
			SDL_RenderFillRect(pRenderer, &m_hitBox[i].m_rect.GetBounds());
		}*/
	}
}

bool Boss::Move(const Vector2& direction, [[maybe_unused]] double speed)
{
	for (size_t i = 0; i < kMaxHitBox; ++i)
	{
		Vector2 position{ 0,0 };
		// Calculate hitbox position: boss position + offset
		position.m_x = direction.m_x + m_hitBox[i].m_offset.m_x;
		position.m_y = direction.m_y + m_hitBox[i].m_offset.m_y;
		m_hitBox[i].m_rect.SetPosition(position);
	}
	m_position = direction;
	return true;
}

void Boss::SetPosition(const Vector2& position)
{
	AnimationComponent* pAnimation = static_cast<AnimationComponent*>(GetComponent("Animation"));
	ParticleComponent* pParticle = static_cast<ParticleComponent*>(GetComponent("Particle"));
	m_position = position;
	for (size_t i = 0; i < kMaxHitBox; ++i)
	{
		Vector2 newPos{ 0,0 };
		newPos.m_x = position.m_x + m_hitBox[i].m_offset.m_x;
		newPos.m_y = position.m_y + m_hitBox[i].m_offset.m_y;
		m_hitBox[i].m_rect.SetPosition(newPos);
	}
	pAnimation->SetPosition(position);
	Vector2 offsetPosition = { (96 / 2) + position.m_x, (96 / 2) + position.m_y };
	pParticle->SetPosition(offsetPosition);
}

GameObject* Boss::Clone()
{
	return new Boss(*this);
}

void Boss::SetHealth(const int& hp)
{
	ParticleComponent* pParticle = static_cast<ParticleComponent*>(GetComponent("Particle"));

	int oldHp = m_hp;
	m_hp += hp;
	if (m_hp > kMaxHp)
		m_hp = kMaxHp;

	if (m_hp < 0)
		m_hp = 0;

	if (m_hp != oldHp)
	{
		if (m_hp < oldHp)
			pParticle->Splash();
		/*else
			// Heal*/
		std::cout << "Boss health: " << m_hp << "\n";
		if (m_hp <= 0)
		{
			std::cout << "Boss Died\n";
		}
	}
}