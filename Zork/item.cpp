#include <iostream>
#include "room.h"
#include "exit.h"
#include "globals.h"
#include "item.h"


// ----------------------------------------------------
Item::Item(const char* title, const char* description, Entity* parent, const int capacity, const int size, bool takeable, ItemType item_type) :
Entity(title, description, capacity, parent), 
item_type(item_type), item_size(size), locked(false), key(NULL), hidden(false), hiding(NULL), takeable(takeable)
{
	type = ITEM;
	min_value = max_value = 0; // sets min and max values to 0 by default
	move_description = "";

	if (parent != NULL)
		parent->current_storage += size;
}

// ----------------------------------------------------
Item::~Item()
{}

// ----------------------------------------------------
void Item::Look() const
{
	/* Outputs the item description and if it contains other items in it, outputs their names */
	cout << "\n" << name << "\n";
	cout << description << "\n";

	list<Entity*> stuff;
	FindAll(ITEM, stuff);

	if(stuff.size() > 0)
	{
		if (locked) {
			cout << "This item is locked." << "\n";
			return;
		}
		cout << "It contains: " << "\n";
		for(list<Entity*>::const_iterator it = stuff.begin(); it != stuff.cend(); ++it)
			cout << (*it)->name << "\n";
	}
}

// ----------------------------------------------------
void Item::ChangeParentTo(Entity* new_parent)
{
	/* Moves this Entity from its current container to the new one by removing from the contents
	of the first and adding itself to te second */
	
	if (parent != NULL) {
		parent->container.remove(this);
		parent->current_storage -= item_size;
	}

	parent = new_parent;

	if (parent != NULL) {
		parent->container.push_back(this);
		parent->current_storage += item_size;
	}
}

// ----------------------------------------------------
int Item::GetValue() const
{
	/* Returns a random value between min and max if max>0, otherwise returns 0 */
	return Roll(min_value, max_value);
}