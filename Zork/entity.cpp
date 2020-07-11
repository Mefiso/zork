#include <iostream>
#include "globals.h"
#include "Entity.h"

// ----------------------------------------------------
Entity::Entity(const char* name, const char* description, Entity* parent = NULL) :
name(name), description(description), parent(parent)
{
	type = ENTITY;

	if(parent != NULL) // Put this Entity to its containing Entity
		parent->container.push_back(this);
}

// ----------------------------------------------------
Entity::~Entity()
{}

// ----------------------------------------------------
void Entity::Look() const
{
	/* Outputs the description of this Entity, e.g. room description. */
	cout << name << "\n";
	cout << description << "\n";
}

// ----------------------------------------------------
void Entity::Tick()
{}

// ----------------------------------------------------
void Entity::ChangeParentTo(Entity* new_parent)
{
	/* Moves this Entity from its current container to the new one by removing from the contents 
	of the first and adding itself to te second */
	if(parent != NULL)
		parent->container.remove(this);

	parent = new_parent;

	if(parent != NULL)
		parent->container.push_back(this);
}

// ----------------------------------------------------
bool Entity::Find(Entity* entity) const
{
	/* Iterates over list of contained Entities and tells whether or not the specified argument
	can be found among them. */
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it) == entity)
			return true;
	}

	return false;
}

// ----------------------------------------------------
Entity* Entity::Find(EntityType type) const
{
	/* Iterates over list of contained entities and returns the pointer of the first Entity 
	with the specified type. */
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == type)
			return *it;
	}

	return NULL;
}

// ----------------------------------------------------
Entity* Entity::Find(const string& name, EntityType type) const
{
	/* Iterates over the list of contained entities and returns the pointer to the Entity with the
	specified name and type */
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == type)
		{
			if(Same((*it)->name, name))
				return *it;
		}
	}

	return NULL;
}

// ----------------------------------------------------
void Entity::FindAll(EntityType type, list<Entity*>& list_to_fill) const
{
	/* Fills a list with all the contained entities that are of the specified type */
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == type)
			list_to_fill.push_back(*it);
	}
}