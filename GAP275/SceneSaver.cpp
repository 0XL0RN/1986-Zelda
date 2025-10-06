#include "SceneSaver.h"
#include <string>
#include <fstream>
#include <iostream>
#include <direct.h>

SceneSaver::SceneSaver()
	: m_IsLoaded(false)
	, m_saveData()
	, m_fileName(kFileOne)
	, m_IsFileOneLoaded(false)
	, m_IsFileTwoLoaded(false)
{
}

SceneSaver::~SceneSaver()
{
}

bool SceneSaver::Save() const
{
	// If no save is loaded and both files are empty, choose an unused file
	std::string path;
	if (!m_IsLoaded && !m_IsFileOneLoaded && !m_IsFileTwoLoaded)
		path = std::string(kSavePath) + "/" + kFileOne;
	else if (!m_IsLoaded && m_IsFileOneLoaded && !m_IsFileTwoLoaded)
		path = std::string(kSavePath) + "/" + kFileTwo;
	else
		path = std::string(kSavePath) + "/" + m_fileName;

	if (_mkdir(kSavePath) != 0 && errno != EEXIST)
		return false;

	std::ofstream file(path, std::ios::binary);
	if (!file.write((char*)&m_saveData, sizeof(m_saveData)))
		return false;

	std::cout << m_fileName << " saved :)\n";
	return true;
}

bool SceneSaver::Load(const std::string& fileName)
{
	m_fileName = fileName.empty() ? kFileOne : fileName;
	std::string path = std::string(kSavePath) + "/" + m_fileName;

	std::ifstream file(path, std::ios::binary);
	if (!file.read((char*)&m_saveData, sizeof(m_saveData)))
	{
		m_IsLoaded = false;
		return false;
	}

	m_IsLoaded = true;
	return true;
}

void SceneSaver::CheckForSave()
{
	// Check if saveone.save exists
	std::string path = std::string(kSavePath) + "/" + kFileOne;
	std::ifstream fileOne(path, std::ios::binary);
	if (!fileOne.read((char*)&m_saveData, sizeof(m_saveData)))
		m_IsFileOneLoaded = false;
	else
		m_IsFileOneLoaded = true;

	// Check if savetwo.save exists
	path = std::string(kSavePath) + "/" + kFileTwo;
	std::ifstream fileTwo(path, std::ios::binary);
	if (!fileTwo.read((char*)&m_saveData, sizeof(m_saveData)))
		m_IsFileTwoLoaded = false;
	else
		m_IsFileTwoLoaded = true;

	if (!m_IsFileOneLoaded && !m_IsFileTwoLoaded)
		m_fileName = kFileOne;
	else if (m_IsFileOneLoaded && !m_IsFileTwoLoaded)
		m_fileName = kFileTwo;
}

void SceneSaver::DeleteFile(const std::string& path)
{
	const std::string filepath = std::string(kSavePath) + "/" + path;
	const bool result = remove(filepath.c_str());

	if (result == 0)
		std::cout << path << " Deleted\n";
}