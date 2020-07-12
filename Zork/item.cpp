#include <iostream>
#include "room.h"
#include "exit.h"
#include "globals.h"
#include "item.h"

// ----------------------------------------------------
Item::Item(const char* title, const char* description, Entity* parent, ItemType item_type) :
Entity(title, description, parent), item_type(item_type)
{
	type = ITEM;
	min_value = max_value = 0; // sets min and max values to 0 by default
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
		cout << "It contains: " << "\n";
		for(list<Entity*>::const_iterator it = stuff.begin(); it != stuff.cend(); ++it)
			cout << (*it)->name << "\n";
	}
}

// ----------------------------------------------------
int Item::GetValue() const
{
	/* Returns a random value between min and max if max>0, otherwise returns 0 */
	return Roll(min_value, max_value);
}