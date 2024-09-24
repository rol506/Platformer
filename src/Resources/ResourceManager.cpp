#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "../Renderer/ShaderProgram.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
std::string ResourceManager::m_path;

void ResourceManager::setExecutablePath(const std::string& exectablePath)
{
	std::size_t pos = exectablePath.find_last_of("/\\");
	m_path = exectablePath.substr(0, pos);
}

void ResourceManager::unloadAllResources()
{
	m_shaderPrograms.clear();
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "Failed to open file: " << relativeFilePath << "\n";
		return std::string{};
	}

	std::stringstream buff;
	buff << f.rdbuf();
	return buff.str();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty())
	{
		std::cerr << "No vertex shader!\n";
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "No fragment shader!\n";
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString))
																.first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Failed to load shader: " << shaderName
		<< "\n	Vertex: " << vertexPath
		<< "\n	Fragment: " << fragmentPath << "\n";

	return nullptr;
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}

	std::cerr << "Can't find shader program: " << shaderName << "\n";
	return nullptr;
}