#pragma once

#include <string>
#include <memory>
#include <map>

#include <glad/glad.h>

namespace RenderEngine
{
	class ShaderProgram;
	class Texture2D;
	class Sprite2D;
}

class ResourceManager
{
public:

	static void setExecutablePath(const std::string& exectablePath);
	static void unloadAllResources();

	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&) = delete;

	static std::shared_ptr<RenderEngine::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<RenderEngine::ShaderProgram> getShaderProgram(const std::string& shaderName);

	static std::shared_ptr<RenderEngine::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath, const GLenum filter = GL_NEAREST, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
	static std::shared_ptr<RenderEngine::Texture2D> getTexture(const std::string& textureName);

	static std::shared_ptr<RenderEngine::Sprite2D> loadSprite(const std::string& spriteName, const std::string& shaderName,
			const std::string& textureName, const std::string& subTextureName = "default");
	static std::shared_ptr<RenderEngine::Sprite2D> getSprite(const std::string& spriteName);

private:

	static std::string getFileString(const std::string& relativeFilePath);

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture2D>> TexturesMap;
	static TexturesMap m_textures;

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Sprite2D>> SpritesMap;
	static SpritesMap m_sprites;

	static std::string m_path;

};