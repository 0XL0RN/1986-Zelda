#include "EntitySpawner.h"
#include "SceneStateMachine.h"
#include "Link.h"
#include "Enemy.h"
#include "AnimationComponent.h"

EntitySpawner::EntitySpawner(std::vector<GameObject*>& objects, SceneStateMachine* pOwner)
	: m_twister(static_cast<unsigned int>(SDL_GetTicks()))
	, m_box({ .x = 112, .y = 132, .w = 687, .h = 467 })
	, m_objects(objects)
	, m_pLink(nullptr)
	, m_enemyCount(0)
	, m_pOwner(pOwner)
{
}

void EntitySpawner::Awake(Link* pLink)
{
	m_pLink = pLink;

	Vector2 linkPos;
	do
	{
		linkPos = GetRandomPosition();
	} while (!IsPositionValid(linkPos, true));

	m_pLink->SetPosition(linkPos);

	for (GameObject* obj : m_objects)
	{
		std::string type = obj->GetId();
		if (type == "Triforce" || type == "Enemy" || type == "Rupee" ||
			type == "Heart" || type == "Fire" || type == "Boss" || type == "Block")
		{
			Vector2 pos;
			do
			{
				pos = GetRandomPosition();
			} while (!IsPositionValid(pos, false));
			obj->SetPosition(pos);
		}
	}

}

void EntitySpawner::Update([[maybe_unused]] const double& deltaTime)
{
	int roll = m_twister.GenerateRange(1u, 15000u);
	if (roll == 15000)
	{
		if (m_enemyCount < kMaxEnemies)
			GenerateEnemy({ 0,0 });
	}
}

Vector2<float> EntitySpawner::GetRandomPosition()
{
	const int x = m_twister.GenerateRange(0u, 627u);
	const int y = m_twister.GenerateRange(0u, 416u);
	/*std::cout << "X: " << x << " Y: " << y << '\n';*/
	return { static_cast<float>(x), static_cast<float>(y) };
}

bool EntitySpawner::IsPositionValid(Vector2& position, bool isLink)
{
	// Check if position is within m_box bounds
	if (position.m_x < m_box.x || position.m_x >= m_box.x + m_box.w || position.m_y < m_box.y || position.m_y >= m_box.y + m_box.h)
		return false;


	// Check for overlap with existing objects
	for (GameObject* obj : m_objects)
	{
		if (obj->GetId() != "Background")
		{
			Vector2 objPos = obj->GetPosition();
			Vector2 deltaObj = position - objPos;
			float dist = std::sqrt(deltaObj.m_x * deltaObj.m_x + deltaObj.m_y * deltaObj.m_y);
			if (dist < kMinDistance / 2.0f)
			{
				return false;
			}
		}
	}

	// Keep objects away from Link
	if (isLink)
	{
		for (GameObject* obj : m_objects)
		{
			std::string type = obj->GetId();
			if (type == "Triforce" || type == "Enemy" || type == "Boss" || type == "Fire")
			{
				Vector2 objPos = obj->GetPosition();
				Vector2 deltaObj = position - objPos;
				float dist = std::sqrt(deltaObj.m_x * deltaObj.m_x + deltaObj.m_y * deltaObj.m_y);
				if (dist < kMinDistance)
					return false;
			}
		}
	}

	return true;
}

void EntitySpawner::GenerateEnemy(Vector2 position)
{
	std::cout << "Enemy Generated\n";
	Vector2 enemyPos;

	if (position.m_x == 0 && position.m_y == 0)
	{
		do
		{
			enemyPos = GetRandomPosition();
		} while (!IsPositionValid(enemyPos, true));
	}
	else
		enemyPos = position;



	const SDL_FRect box{ enemyPos.m_x, enemyPos.m_y,48, 48 };
	GameObject* pEnemy = new Enemy(enemyPos, 2, box, "Enemy", m_pOwner->GetCollisionManager(), m_objects);
	AnimationComponent* pAnimationComponent = new AnimationComponent({ 48,48 }, box, 6, 0, m_pOwner->GetSceneFactory().GrabTexture("assets/Darknut.png"));

	pAnimationComponent->AddSequence("down", 0, 1);
	pAnimationComponent->AddSequence("up", 6, 7);
	pAnimationComponent->AddSequence("left", 2, 3);
	pAnimationComponent->AddSequence("right", 4, 5);
	pAnimationComponent->PlayAnimation("down");
	pEnemy->SetComponent("Animation", pAnimationComponent);

	pEnemy->Awake();
	m_objects.push_back(pEnemy);
	m_pEnemies.push_back(pEnemy);
	++m_enemyCount;
}
