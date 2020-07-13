#include <iostream>
#include "globals.h"
#include "exit.h"
#include "item.h"
#include "creature.h"
#include "room.h"

// ----------------------------------------------------
Room::Room(const char* title, const char* description) :
Entity(title, description, numeric_limits<int>::infinity(), NULL) // Rooms are not contained in any Entity and have infinite capacity
{
	type = ROOM;
}

// ----------------------------------------------------
Room::~Room()
{
}

// ----------------------------------------------------
void Room::Look() const
{
	// Outputs room description
	cout << "\n" << name << "\n";
	cout << description;

	// List exits --
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == EXIT)
		{
			Exit* ex = (Exit*)*it;
			cout << "\nDirection (" << ex->GetNameFrom(this) << ") you see " << ex->GetDestinationFrom(this)->name;
		}
	}

	// List items --
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == ITEM)
		{
			Item* item = (Item*)*it;
			cout << "\nThere is an item here: " << item->name;
		}
	}

	// List creatures --
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == CREATURE)
		{
			Creature* cr = (Creature*)*it;
			cout << "\nThere is someone else here: " << cr->name;
			if(cr->IsAlive() == false)
				cout << " (dead)";
		}
	}

	cout << "\n";
}

// ----------------------------------------------------
Exit* Room::GetExit(const string& direction) const
{
	/* Iterates over contained entities and returns the Exit with direction arg as name */
	for(list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == EXIT)
		{
			Exit* ex = (Exit*) *it;
			if(Same(ex->GetNameFrom(this), direction))
				return ex;
		}
	}

	return NULL;
}


