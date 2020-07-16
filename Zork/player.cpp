#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"

// ----------------------------------------------------
Player::Player(const char* title, const char* description, Room* room, const int capacity) :
Creature(title, description, room, capacity)
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
				switch ((*it)->type)
				{
				case ITEM:
					if (((Item*)(*it))->hidden)
						return;
				case EXIT:
					if (((Exit*)(*it))->hidden)
						return;
				default:
					(*it)->Look();
					return;
				}
				
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

	if(exit == NULL || exit->hidden)
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
	Item* item;
	if(args.size() == 4) // For subitems inside items
	{
		item = (Item*)parent->Find(args[3], ITEM); // From Room

		// we could pick something from a container in our inventory ...
		if(item == NULL)
			item = (Item*)Find(args[3], ITEM); // From inventory

		if(item == NULL || item->hidden)
		{
			cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return false;
		}

		if (item->locked) {
			cout << "\n" << item->name << " is locked.\n";
			return false;
		}
		Item* subitem = (Item*)item->Find(args[1], ITEM); // Item inside another

		if(subitem == NULL)
		{
			cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		if(current_storage + subitem->item_size > capacity || !subitem->takeable) {
			cout << "\nCannot take " << subitem->name << ((subitem->takeable) ? ". Inventory is full.\n" : ". It's too heavy to carry.\n");
			return false;
		}
		cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this); // Moves to root inventory.
		item = subitem;
	}
	else if(args.size() == 2) // For items in room
	{
		item = (Item*)parent->Find(args[1], ITEM);

		if(item == NULL || item->hidden)
		{
			cout << "\nThere is no item here with that name.\n";
			return false;
		}

		if (current_storage + item->item_size > capacity || !item->takeable) {
			cout << "\nCannot take " << item->name << ((item->takeable) ? ". Inventory is full.\n" : ". It's too heavy to carry.\n");
			return false;
		}
		cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
	}

	if (item->hiding != NULL) {
		cout << item->move_description << "\n";
		((item->hiding)->type == EXIT ? ((Exit*)item->hiding)->hidden : ((Item*)item->hiding)->hidden) = false;
		item->hiding = NULL;
	}
	return true;
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

			if (stuff.size() > 0 && ((Item*)(*it))->locked == false) { // If an item contains more inside, list them.
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

		if (item == weapon || item == armour)
			UnEquip(args);
	
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
			if (container == NULL || container->hidden) {
				cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
				return false;
			}
		}
		else if(container->hidden) {
			cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return false;
		}

		if (container->locked) { // Check if container locked
			cout << container->name << " is locked. Can't open it to put something inside.\n";
			return false;
		}

		if(container->current_storage + item->item_size > container->capacity) {
			cout << "\nCannot put " << item->name << " into " << container->name << ".\n";
			return false;
		}

		if (item == weapon || item == armour)
			UnEquip(args);
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
		case M_WEAPON:
			weapon = item;
			break;

		case D_WEAPON:
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
	cout << "\nYou attack " << target->name << "!\n";
	MakeAttack();
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
			if(current_storage + i->item_size > capacity) {
				cout << "\nCannot take any more items. Inventory is full.\n";
				return false;
			}
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
	
	Entity* lock;
	auto lock_locked = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->locked : ((Item*)e)->locked); };
	auto lock_key = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->key : ((Item*)e)->key); };
	auto lock_name = [this](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->GetNameFrom((Room*)parent) : ((Item*)e)->name); };
	auto lock_hidden = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->hidden : ((Item*)e)->hidden); };

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
	{
		Item* l_item = (Item*)GetRoom()->Find(args[1], ITEM);
		if (l_item == NULL) {
			l_item = (Item*)Find(args[1], ITEM);
			if (l_item == NULL) {
				cout << "\nThere is no exit nor object '" << args[1] << "'.\n";
				return false;
			}
		}
		lock = l_item;
		lock->type = ITEM;
	}
	else {
		lock = exit;
		lock->type = EXIT;
	}

	if (lock_hidden(lock)) {
		cout << "\nThere is no exit nor object '" << args[1] << "'.\n";
		return false;
	}

	if (lock_locked(lock) == true)
	{
		cout << "\nThat is already locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL)
	{
		cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if (lock_key(lock) != item)
	{
		cout << "\nItem '" << item->name << "' is not the key for " << lock_name(lock) << ".\n";
		return false;
	}

	cout << "\nYou lock " << lock_name(lock) << "...\n";

	(lock->type == EXIT ? ((Exit*)lock)->locked : ((Item*)lock)->locked) = true;

	return true;
}

// ----------------------------------------------------
bool Player::UnLock(const vector<string>& args)
{
	if(!IsAlive())
		return false;

	Entity* lock;
	auto lock_locked = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->locked : ((Item*)e)->locked); };
	auto lock_key = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->key : ((Item*)e)->key); };
	auto lock_name = [this](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->GetNameFrom((Room*)parent) : ((Item*)e)->name); };
	auto lock_hidden = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->hidden : ((Item*)e)->hidden); };

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
	{
		Item* l_item = (Item*)GetRoom()->Find(args[1], ITEM);
		if (l_item == NULL) {
			l_item = (Item*)Find(args[1], ITEM);
			if (l_item == NULL) {
				cout << "\nThere is no exit nor object '" << args[1] << "'.\n";
				return false;
			}
		}
		lock = l_item;
		lock->type = ITEM;
	}
	else {
		lock = exit;
		lock->type = EXIT;
	}

	if (lock_hidden(lock)) {
		cout << "\nThere is no exit nor object '" << args[1] << "'.\n";
		return false;
	}
	
	if (lock_locked(lock) == false)
	{
		cout << "\nThat is already unlocked.\n";
		return false;
	}
	
	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL)
	{
		cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if (lock_key(lock) != item)
	{
		cout << "\nItem '" << item->name << "' is not the key for " << lock_name(lock) << ".\n";
		return false;
	}

	cout << "\nYou unlock " << lock_name(lock) << "...\n";

	(lock->type == EXIT ? ((Exit*)lock)->locked : ((Item*)lock)->locked) = false;

	return true;
}

bool Player::Move(const vector<string>& args)
{
	Item* item = (Item*)parent->Find(args[1], ITEM);

	if (item == NULL || item->hidden)
	{
		cout << "\nThere is no item here with that name.\n";
		return false;
	}

	cout << "\nYou try moving " << item->name << " aside.\n";
	
	if (item->hiding == NULL)
		return false;
	cout << item->move_description << "\n";
	((item->hiding)->type == EXIT ? ((Exit*)item->hiding)->hidden : ((Item*)item->hiding)->hidden) = false;
	item->hiding = NULL;
	
	return true;
}

void Player::Tick()
{}

void Player::Die() {
	cout << "You die. You couldn't complete your quest and now you're dead.\n";
}