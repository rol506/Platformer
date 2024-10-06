#pragma once

#include <unordered_set>
#include <memory>

#include <glm/vec2.hpp>

#include "../Renderer/Sprite2D.h"

struct rect
{
	glm::vec2 pos1; //left bottom
	glm::vec2 pos2; //right top
};

class PhysicsEngine
{
public:

	static void init();
	static void terminate();
	static void addDynamicGameObject(std::shared_ptr<RenderEngine::Sprite2D> gameObjectPtr);
	static void update(double delta);
	static std::shared_ptr<RenderEngine::Sprite2D> mouseRayCast(const int xpos, const int ypos);

private:

	static bool RectVsRect(const rect& r1, const rect& r2);
	static bool pointVsRect(const glm::vec2& p, const rect& r);

	static std::unordered_set<std::shared_ptr<RenderEngine::Sprite2D>> dynamicGameObjects;

};