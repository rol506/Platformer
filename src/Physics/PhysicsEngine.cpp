#include "PhysicsEngine.h"

std::unordered_set<std::shared_ptr<RenderEngine::Sprite2D>> PhysicsEngine::dynamicGameObjects;

bool PhysicsEngine::RectVsRect(const rect& r1, const rect& r2)
{
	return (r1.pos1.x < r2.pos2.x && r1.pos2.x > r2.pos1.x &&
			r1.pos1.y < r2.pos2.y && r1.pos2.y > r2.pos1.y);
}

bool PhysicsEngine::pointVsRect(const glm::vec2& p, const rect& r)
{
	return (p.x >= r.pos1.x && p.y >= r.pos1.y && p.x < r.pos2.x && p.y < r.pos2.y);
}

void PhysicsEngine::addDynamicGameObject(std::shared_ptr<RenderEngine::Sprite2D> gameObjectPtr)
{
	dynamicGameObjects.insert(std::move(gameObjectPtr));
}

void PhysicsEngine::update(double delta)
{
	//collisions
	for (auto& currentDynamicObject : dynamicGameObjects)
	{
		//set target position
		if (currentDynamicObject->getVelocity().x != 0.f || currentDynamicObject->getVelocity().y != 0.f)
		{
			currentDynamicObject->setTargetPosition(glm::vec2(currentDynamicObject->getPosition().x + currentDynamicObject->getVelocity().x * delta, 
															  currentDynamicObject->getPosition().y + currentDynamicObject->getVelocity().y * delta));
		}

		//check collisions
		bool hasCollision = false;
		for (auto& object : dynamicGameObjects)
		{
			if (object != currentDynamicObject)
			{
				hasCollision = RectVsRect(
					{ currentDynamicObject->getTargetPosition(), currentDynamicObject->getTargetPosition() + currentDynamicObject->getSize() },
					{ object->getTargetPosition(), object->getTargetPosition() + object->getSize() });
			}
		}
		
		if (!hasCollision)
			currentDynamicObject->setPosition(currentDynamicObject->getTargetPosition());

		currentDynamicObject->addVelocity(-currentDynamicObject->getVelocity());
	}
}

std::shared_ptr<RenderEngine::Sprite2D> PhysicsEngine::mouseRayCast(const int xpos, const int ypos)
{
	for (auto& object : dynamicGameObjects)
	{
		if (pointVsRect(glm::vec2(xpos, ypos), { object->getPosition(), object->getPosition() + object->getSize() }))
		{
			return object;
		}
	}

	return nullptr;
}

void PhysicsEngine::init()
{

}

void PhysicsEngine::terminate()
{
	dynamicGameObjects.clear();
}