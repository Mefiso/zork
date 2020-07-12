#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "creature.h"

// ----------------------------------------------------
Creature::Creature(const char* title, const char* description, Room* room) :
Entity(title, description, (Entity*)room) // Where the Creature is, currently.
{
	type = CREATURE;
	hit_points = 1;
	min_damage = max_damage = min_protection = max_protection = 0;
	weapon = armour = NULL;
	combat_target = NULL;
}

// ----------------------------------------------------
Creature::~Creature()
{}

// ----------------------------------------------------
void Creature::Look(const vector<string>& args) const
{
	/* If the creature is alive outputs name and description, if not announces that it is dead
	and outputs name and description. */
	if(IsAlive())
	{
		cout << name << "\n";
		cout << description << "\n";
	}
	else
	{
		cout << name << "'s corpse\n";
		cout << "Here lies dead: " << description << "\n";
	}
}

// ----------------------------------------------------
bool Creature::Go(const vector<string>& args)
{
	/* Moves this Creature through the specified Exit */
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
		return false;

	if(PlayerInRoom()) 
		cout << name << "goes " << args[1] << "...\n";

	ChangeParentTo(exit->GetDestinationFrom((Room*) parent)); 
	// Changes parent to the Room at the opposite side of the Exit

	return true;
}

// ----------------------------------------------------
bool Creature::Take(const vector<string>& args)
{
	/* Finds the Item with the specified name among current Room contained entities. 
	If it can't, tries to find inside another item from Creature's inventory. In any case, if 
	found, the Item is moved to the root of the Inventory (list of contained entities by Creature)	*/
	if(!IsAlive())
		return false;

	Item* item = (Item*)parent->Find(args[1], ITEM); // Casts Entity* to Item*

	if(args.size() > 1)
	{
		// we could pick something from a container in our inventory ...
		if(item == NULL)
			item = (Item*)Find(args[1], ITEM);

		if(item == NULL) // Item can't be found
			return false;

		Item* subitem = (Item*)item->Find(args[3], ITEM); // Search Item inside Item

		if(subitem == NULL) // Subitem not found
			return false;

		if(PlayerInRoom())
			cout << name << " looks into " << item->name << "...\n";

		item = subitem;
	}

	if(item == NULL) // Item not found
		return false;

	if(PlayerInRoom())
		cout << name << " takes " << item->name << ".\n";

	item->ChangeParentTo(this); // Stores the Item on the Inventory.

	return true;
}

// ----------------------------------------------------
void Creature::Inventory() const
{
	/* Simply lists all contained Items. */
	list<Entity*> items;
	FindAll(ITEM, items);

	if(items.size() == 0)
	{
		cout << name << " does not own any items\n";
		return;
	}

	cout << "\n" << name << " owns:\n";
	for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if(*it == weapon)
			cout << (*it)->name << " (as weapon)\n";
		else if(*it == armour)
			cout << (*it)->name << " (as armour)\n";
		else
			cout << (*it)->name << "\n";
	}
}

// ----------------------------------------------------
bool Creature::Equip(const vector<string>& args)
{
	/* Equips the specified Item if it is an armour or a weapon */
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
		return false;

	switch(item->item_type)
	{
		case WEAPON:
		weapon = item;
		break;

		case ARMOUR:
		armour = item;
		break;

		default:
		return false;
	}

	if(PlayerInRoom())
		cout << name << " equips " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Creature::UnEquip(const vector<string>& args)
{
	/* Unequips the specified armour/weapon if currently equipped. */
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
		return false;

	if(item == weapon)
		weapon = NULL;
	else if(item == weapon)
		armour = NULL;
	else
		return false;

	if(PlayerInRoom())
		cout << name << " un-equips " << item->name << "...\n";

	return true;
}


// ----------------------------------------------------
bool Creature::AutoEquip()
{
	/* Iterates over contained items and equips the last weapon and armour that it finds. */
	if(!IsAlive())
		return false;

	list<Entity*> items;
	FindAll(ITEM, items);

	for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		Item* i = (Item*)(*it);

		if(i->item_type == WEAPON)
			weapon = i;
		if(i->item_type == ARMOUR)
			armour = i;
	}

	return true;
}

// ----------------------------------------------------
bool Creature::Lock(const vector<string>& args)
{
	/* Locks the specified Exit with an Item if it is the appropriate key. */
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL || exit->locked == true)
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL || exit->key != item)
		return false;

	if(PlayerInRoom())
		cout << "\n" << name << "locks " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = true;

	return true;
}

// ----------------------------------------------------
bool Creature::UnLock(const vector<string>& args)
{
	/* Unlocks the specified Exit with an Item if it is the appropriate key. */
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL || exit->locked == false)
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL || exit->key != item)
		return false;

	if(PlayerInRoom())
		cout << "\n" << name << "unlocks " << exit->GetNameFrom((Room*) parent) << "...\n";

	exit->locked = false;

	return true;
}

// ----------------------------------------------------
bool Creature::Drop(const vector<string>& args)
{
	/* Moves Item from inventory to the current room. */
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
		return false;

	if(PlayerInRoom())
		cout << name << " drops " << item->name << "...\n";
	
	item->ChangeParentTo(parent);

	return true;
}

// ----------------------------------------------------
Room* Creature::GetRoom() const
{
	return (Room*)parent;
}

// ----------------------------------------------------
bool Creature::PlayerInRoom() const
{
	return parent->Find(PLAYER) != NULL; // Whether player in room or not.
}

// ----------------------------------------------------
bool Creature::IsAlive() const
{
	return hit_points > 0;
}

// ----------------------------------------------------
void Creature::Tick()
{
	/* If the combat target is in the current room, attacks it. */
	if(combat_target != NULL)
	{
		if(parent->Find(combat_target) == true)
			MakeAttack();
		else
			combat_target = NULL;
	}
}

// ----------------------------------------------------
bool Creature::Attack(const vector<string>& args)
{
	/* If the specified creature can be found in current room, the combat_target is set to that
	creature*/
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
		return false;

	combat_target = target;
	cout << "\n" << name << " attacks " << target->name << "!\n";
	return true;
}

// ----------------------------------------------------
int Creature::MakeAttack()
{
	/* Rolls damage from weapon and applies it to the comabt target Creature. It also makes this
	Creature the target of the combat target if it wasn't. */
	if(!IsAlive() || !combat_target->IsAlive()) // If either combatants are dead, remove targets.
	{
		combat_target = combat_target->combat_target = NULL;
		return false;
	}

	int result = (weapon) ? weapon->GetValue() : Roll(min_damage, max_damage);

	if(PlayerInRoom())
		cout << name << " attacks " << combat_target->name << " for " << result << "\n";

	combat_target->ReceiveAttack(result);

	// make the attacker react and take me as a target
	if(combat_target->combat_target == NULL)
		combat_target->combat_target = this;

	return result;
}

// ----------------------------------------------------
int Creature::ReceiveAttack(int damage)
{
	/* Subtract as many hit points as damage surpasses Creature's armour */
	int prot = (armour) ? armour->GetValue() : Roll(min_protection, max_protection);
	int received = damage - prot;

	hit_points -= received;

	if(PlayerInRoom())
		cout << name << " is hit for " << received << " damage (" << prot << " blocked) \n";

	if(IsAlive() == false)
		Die();

	return received;
}

// ----------------------------------------------------
void Creature::Die()
{
	if(PlayerInRoom())
		cout << name << " dies.\n";
}

// ----------------------------------------------------
bool Creature::Loot(const vector<string>& args)
{
	/* Takes all the items from the specified corpse. */
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL || target->IsAlive() == true) // No target or target not dead
		return false;

	list<Entity*> items;
	target->FindAll(ITEM, items);

	for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		Item* i = (Item*)(*it);
		i->ChangeParentTo(this);
	}

	cout << "\n" << name << " loots " << target->name << "'s corpse\n";

	return true;
}

// ----------------------------------------------------
void Creature::Stats() const
{
	/* Prints all creature stats. */
	cout << "\nHit Points: " << hit_points;
	cout << "\nAttack: (" << ((weapon) ? weapon->name : "no weapon") << ") ";
	cout << ((weapon) ? weapon->min_value : min_damage) << "-" << ((weapon) ? weapon->max_value : max_damage);
	cout << "\nProtection: (" << ((armour) ? armour->name : "no armour") << ") ";
	cout << ((armour) ? armour->min_value : min_protection) << "-" << ((armour) ? armour->max_value : max_protection);
	cout << "\n";
}