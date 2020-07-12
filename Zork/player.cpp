#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"

// ----------------------------------------------------
Player::Player(const char* title, const char* description, Room* room) :
Creature(title, description, room)
{
	type = PLAYER;
}

// ----------------------------------------------------
Player::~Player()
{
}

// ----------------------------------------------------
void Player::Look(const vector<string>& args) const
{
	/* If command has arguments, it calls the Look method from the specified Entities in those arguments.
	Otherwise, it calls current Room's Look method. */
	if(args.size() > 1)
	{
		// Looks for the enitity in the room.
		for(list<Entity*>::const_iterator it = parent->container.begin(); it != parent->container.cend(); ++it)
		{
			if(Same((*it)->name, args[1]) || ((*it)->type == EXIT && Same(args[1], ((Exit*)(*it))->GetNameFrom((Room*)parent))))
			{
				(*it)->Look();
				return;
			}
		}
		
		// Looks for the entity in the inventory.
		for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
		{
			if (Same((*it)->name, args[1]) || ((*it)->type == EXIT && Same(args[1], ((Exit*)(*it))->GetNameFrom((Room*)parent))))
			{
				(*it)->Look();
				return;
			}
		}

		if(Same(args[1], "me"))
		{
			cout << "\n" << name << "\n";
			cout << description << "\n";
		}
	}
	else
	{
		parent->Look();
	}
}

// ----------------------------------------------------
bool Player::Go(const vector<string>& args)
{
	/* Moves the player through the specified Exit if possible */
	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->locked)
	{
		cout << "\nThat exit is locked.\n";
		return false;
	}

	cout << "\nYou take direction " << exit->GetNameFrom((Room*) parent) << "...\n";
	ChangeParentTo(exit->GetDestinationFrom((Room*) parent));
	parent->Look();

	return true;
}


// ----------------------------------------------------
bool Player::Take(const vector<string>& args)
{
	/* Tries to pick the specified object either from the Room or from another Item in eithe the 
	inventory or the current Room. */
	
	if(args.size() == 4) // For subitems inside items
	{
		Item* item = (Item*)parent->Find(args[3], ITEM); // From Room

		// we could pick something from a container in our inventory ...
		if(item == NULL)
			item = (Item*)Find(args[3], ITEM); // From inventory

		if(item == NULL)
		{
			cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return false;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM); // Item inside another

		if(subitem == NULL)
		{
			cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this); // Moves to root inventory.
	}
	else if(args.size() == 2) // For items in room
	{
		Item* item = (Item*)parent->Find(args[1], ITEM);

		if(item == NULL)
		{
			cout << "\nThere is no item here with that name.\n";
			return false;
		}

		cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
	}

	return false;
}

// ----------------------------------------------------
void Player::Inventory() const
{
	/* Simply lists all contained Items. */
	list<Entity*> items;
	FindAll(ITEM, items);

	if(items.size() == 0)
	{
		cout << "\nYou do not own any item.\n";
		return;
	}

	for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if(*it == weapon)
			cout << "\n" << (*it)->name << " (as weapon)";
		else if(*it == armour)
			cout << "\n" << (*it)->name << " (as armour)";
		else
			cout << "\n" << (*it)->name;
			list<Entity*> stuff;
			(*it)->FindAll(ITEM, stuff);

			if (stuff.size() > 0) { // If an item contains more inside, list them.
				cout << " containing ";
				for (list<Entity*>::const_iterator it2 = stuff.begin(); it2 != stuff.cend(); ++it2)
					cout << (*it2)->name << ", ";
				cout << "\b\b";
				cout << ".";
				cout << '\b';
			}
			
	}

	cout << "\n";
}

// ----------------------------------------------------
bool Player::Drop(const vector<string>& args)
{
	/* Leaves an Item from your inventory on the room or inside another Item. */
	if(args.size() == 2)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
		{
			cout << "\nThere is no item on you with that name.\n";
			return false;
		}

		cout << "\nYou drop " << item->name << "...\n";
		item->ChangeParentTo(parent);

		return true;
	}
	else if(args.size() == 4) // Drop it inside another Item
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
		{
			cout << "\nCan not find '" << args[1] << "' in your inventory.\n";
			return false;
		}

		Item* container = (Item*)parent->Find(args[3], ITEM); 

		if(container == NULL)
		{
			container = (Item*)Find(args[3], ITEM);
			if (container == NULL) {
				cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
				return false;
			}
		}

		cout << "\nYou put " << item->name << " into " << container->name << ".\n";
		item->ChangeParentTo(container);

		return true;
	}
	
	return false;
}

// ----------------------------------------------------
bool Player::Equip(const vector<string>& args)
{
	/* Equips the specified Item if it is an armour or a weapon */
	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
	{
		cout << "\nCannot find '" << args[1] << "' is not in your inventory.\n";
		return false;
	}

	switch(item->item_type)
	{
		case WEAPON:
		weapon = item;
		break;

		case ARMOUR:
		armour = item;
		break;

		default:
		cout << "\n" << item->name << " cannot be equipped.\n";
		return false;
	}
		
	cout << "\nYou equip " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::UnEquip(const vector<string>& args)
{
	/* Unequips the specified armour/weapon if currently equipped */
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
	{
		cout << "\n" << item->name << " is not in your inventory.\n";
		return false;
	}

	if(item == weapon)
		weapon = NULL;
	else if(item == armour)
		armour = NULL;
	else
	{
		cout << "\n" << item->name << " is not equipped.\n";
		return false;
	}

	cout << "\nYou un-equip " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::Examine(const vector<string>& args) const
{
	/* Prints invetory and stats of target creature if it is on the Room */
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
	{
		cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	target->Inventory();
	target->Stats();

	return true;
}

// ----------------------------------------------------
bool Player::Attack(const vector<string>& args)
{
	/* If the specified creature can be found in the current room, the combat_targetis set to that
	creature. */
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
	{
		cout << "\n" << args[1] << " is not here.";
		return false;
	}

	combat_target = target;
	cout << "\nYou jump to attack " << target->name << "!\n";
	return true;
}

// ----------------------------------------------------
bool Player::Loot(const vector<string>& args)
{
	/* Takes all the items from the specified corpse. */
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
	{
		cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	if(target->IsAlive() == true)
	{
		cout << "\n" << target->name << " cannot be looted until it is killed.\n";
		return false;
	}

	list<Entity*> items;
	target->FindAll(ITEM, items);

	if(items.size() > 0)
	{
		cout << "\nYou loot " << target->name << "'s corpse:\n";

		for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
		{
			Item* i = (Item*)(*it);
			cout << "You find: " << i->name << "\n";
			i->ChangeParentTo(this);
		}
	}
	else
		cout << "\nYou loot " << target->name << "'s corpse, but find nothing there.\n";

	return true;
}

// ----------------------------------------------------
bool Player::Lock(const vector<string>& args)
{
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->locked == true)
	{
		cout << "\nThat exit is already locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL)
	{
		cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if(exit->key != item)
	{
		cout << "\nItem '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	cout << "\nYou lock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = true;

	return true;
}

// ----------------------------------------------------
bool Player::UnLock(const vector<string>& args)
{
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->locked == false)
	{
		cout << "\nThat exit is not locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL)
	{
		cout << "\nKey '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if(exit->key != item)
	{
		cout << "\nKey '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	cout << "\nYou unlock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = false;

	return true;
}