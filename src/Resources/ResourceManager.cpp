#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite2D.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
std::string ResourceManager::m_path;

void ResourceManager::setExecutablePath(const std::string& exectablePath)
{
	std::size_t pos = exectablePath.find_last_of("/\\");
	m_path = exectablePath.substr(0, pos);
}

void ResourceManager::unloadAllResources()
{
	m_shaderPrograms.clear();
	m_textures.clear();
	m_sprites.clear();
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

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath, const GLenum filter, const GLenum wrapMode)
{
	int channels = 0;
	int width = 0;
	int height = 0;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!data)
	{
		std::cerr << "Failed to open texture: " << texturePath << "\n";
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName,
		std::make_shared<RenderEngine::Texture2D>(width, height, channels, data, filter, wrapMode)).first->second;

	stbi_image_free(data);
	return newTexture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}

	std::cerr << "Can't find texture: " << textureName << "\n";
	return nullptr;
}

std::shared_ptr<RenderEngine::Sprite2D> ResourceManager::loadSprite(const std::string& spriteName, const std::string& shaderName,
	const std::string& textureName, const std::string& subTextureName)
{
	auto shader = getShaderProgram(shaderName);
	if (!shader)
	{
		std::cerr << "Can't find shader program: " << shaderName << " for the sprite: " << spriteName << "\n";
		return nullptr;
	}

	auto texture = getTexture(textureName);
	if (!texture)
	{
		std::cerr << "Can't find texture: " << textureName << " for the sprite: " << spriteName << "\n";
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Sprite2D> newSprite = m_sprites.emplace(spriteName, std::make_shared<RenderEngine::Sprite2D>(
		shader, texture, subTextureName
	)).first->second;

	return newSprite;
}

std::shared_ptr<RenderEngine::Sprite2D> ResourceManager::getSprite(const std::string& spriteName)
{
	auto it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the sprite: " << spriteName << "\n";
	return nullptr;
}