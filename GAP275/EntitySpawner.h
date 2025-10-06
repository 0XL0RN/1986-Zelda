#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "mt19937.h"
#include "Vector2.h"

class GameObject;
class SceneStateMachine;
class Link;

class EntitySpawner final
{
private:
	using Vector2 = Vector2<float>;
	static constexpr int kMaxEnemies = 3;
	static constexpr float kMinDistance = 120.0f;
public:
	explicit EntitySpawner(std::vector<GameObject*>& objects, SceneStateMachine* pOwner);
	~EntitySpawner() = default;

	void Awake(Link* pLink);
	void Update(const double& deltaTime);
	void GenerateEnemy(Vector2 position);
	std::vector<GameObject*>& GetEnemies() { return m_pEnemies; }
private:
	mt19937 m_twister;
	SDL_FRect m_box;
	std::vector<GameObject*>& m_objects;
	std::vector<GameObject*> m_pEnemies;
	Link* m_pLink;
	size_t m_enemyCount;
	SceneStateMachine* m_pOwner;

	Vector2 GetRandomPosition();
	bool IsPositionValid(Vector2& position, bool isLink);
};