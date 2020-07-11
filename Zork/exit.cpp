#include <iostream>
#include "room.h"
#include "Exit.h"

// ----------------------------------------------------
Exit::Exit(const char* name, const char* opposite_name, const char* description, Room* origin, Room* destination, bool one_way) :
Entity(name, description, (Entity*)origin),
closed(false), locked(false), key(NULL), one_way(one_way), destination(destination), opposite_name(opposite_name)
{
	type = EXIT;

	/* If the Exit is not unidirectional, it is also contained by the Entity specified as destination */
	if(one_way == false)
		destination->container.push_back(this);
}

// ----------------------------------------------------
Exit::~Exit()
{
}


// ----------------------------------------------------
void Exit::Look() const
{
	cout << name << " to " << opposite_name << "\n";
	cout << description << "\n";
}

// ----------------------------------------------------
const string& Exit::GetNameFrom(const Room* room) const
{
	/* Returns the name of the Exit from the side of Room arg */
	if(room == parent)
		return name;
	if(room == destination)
		return opposite_name;

	return name; // error ?
}

// ----------------------------------------------------
Room* Exit::GetDestinationFrom(const Room* room) const
{
	/* Returns the Room in the opposite side (viewing from room arg) of this Exit. */
	if(room == parent)
		return destination;
	if(room == destination)
		return (Room*) parent;

	return NULL;
}