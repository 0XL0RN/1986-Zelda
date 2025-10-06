#pragma once

#include <string>
#include "Vector2.h"

class SceneSaver final
{
private:
	static constexpr const char* kSavePath = "saves\\";
	static constexpr const char* kFileOne = "saveone.save";
	static constexpr const char* kFileTwo = "savetwo.save";
public:
	struct SaveData
	{
		Vector2<float> playerPosition;
		float playerHealth;
		size_t enemyCount;
		Vector2<float> enemyPositions[4];
		size_t blockCount;
		Vector2<float> blockPositions[3];
		size_t fireCount;
		Vector2<float> firePositions[3];
		size_t rupeeCount;
		Vector2<float> rupeePosition[2];
		Vector2<float> triforcePosition;
		Vector2<float> bossPosition;
	};

	SceneSaver();
	~SceneSaver();

	bool Save() const;
	bool Load(const std::string& fileName);
	void CheckForSave();
	[[nodiscard]] bool IsLoaded() const { return m_IsLoaded; }
	[[nodiscard]] bool IsSaveOne() const { return m_IsFileOneLoaded; }
	[[nodiscard]] bool IsSaveTwo() const { return m_IsFileTwoLoaded; }
	void UnLoad() { m_IsLoaded = false; }
	void DeleteFile(const std::string& path);
	SaveData& GetSaveData() { return m_saveData; }
private:
	bool m_IsLoaded;
	SaveData m_saveData;
	std::string m_fileName;
	bool m_IsFileOneLoaded;
	bool m_IsFileTwoLoaded;
};