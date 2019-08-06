#include "Common/Common.h"
#include "Entity.h"

#include "Manager.h"

namespace Oath {

	ECS::Entity::Entity(Manager* ecs) : ecs(ecs), transform(this) { }
	
	ECS::Entity::~Entity()
	{
		for (ECS::Entity* child : childs)
			ecs->removeEntity(child);
	}

	void ECS::Entity::add(Entity * e) {
		childs.push_back(e);
		e->parent = this;
	}

	void ECS::Entity::add(Script * script) {
		scripts.emplace_back(script);

		try
		{
			script->Start();
		}
		catch (const std::exception& e)
		{
			LOGERROR(e.what());
		}
	}

	ECS::BaseComponent * ECS::Entity::getComponentByType(ComponentID componentID)
	{
		return ecs->getComponentByType(this, componentID);
	}
}