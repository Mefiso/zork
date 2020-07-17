#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "creature.h"
#include "spell.h"
#include <map>

/* Note: Creatures other than Player are not affected by hidden objects or exits, they can take
or use them. That is because the hidden state of those Entities represents only their visibility
w.r.t. the player. */

// ----------------------------------------------------
Creature::Creature(const char* title, const char* description, Room* room, const int capacity) :
Entity(title, description, capacity, (Entity*)room/*Where the Creature is, currently*/)
{
	type = CREATURE;
	hit_points = mana_points = 1;
	min_damage = max_damage = min_protection = max_protection = 0;
	strength = dexterity = intelligence = 0;
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
		cout << "\n" << name << "\n";
		cout << description << "\n";
	}
	else
	{
		cout << "\n" << name << "'s corpse\n";
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

	if(exit == NULL) // Creatures other than player can move throughexits even if hidden
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

		if(item == NULL || item->locked) // Item can't be found or opened up
			return false;

		Item* subitem = (Item*)item->Find(args[3], ITEM); // Search Item inside Item

		if(subitem == NULL) // Subitem not found
			return false;

		if(PlayerInRoom())
			cout << "\n" << name << " looks into " << item->name << "...\n";

		item = subitem;
	}

	if(item == NULL || item->locked) // Item not found or locked
		return false;

	if(current_storage + item->item_size > capacity) {
		if (PlayerInRoom())
			cout << "\n" << name << " tries to take " << item->name << ", but " << name << " can't carry more objects.\n";
		return false;
	}
	
	if(PlayerInRoom())
		cout << "\n" << name << " takes " << item->name << ".\n";

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
		cout << "\n" << name << " does not own any items\n";
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
void Creature::SpellsBook() const
{

	if (spells_book.size() == 0)
	{
		cout << "\n" << name << " does not know any spells.\n";
		return;
	}

	cout << "\n" << name << " knows:\n";
	for (list<Spell*>::const_iterator it = spells_book.begin(); it != spells_book.cend(); ++it)
	{
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
		return false;
	}

	if(PlayerInRoom())
		cout << "\n" << name << " equips " << item->name << "...\n";

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
		cout << "\n" << name << " un-equips " << item->name << "...\n";

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

		if(i->item_type == M_WEAPON || i->item_type == D_WEAPON)
			weapon = i;
		if(i->item_type == ARMOUR)
			armour = i;
	}

	return true;
}

// ----------------------------------------------------
bool Creature::Lock(const vector<string>& args)
{
	/* Locks the specified Item or Exit with an Item if it is the appropriate key. */
	if(!IsAlive())
		return false;

	Entity* lock;
	auto lock_locked = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->locked : ((Item*)e)->locked); };
	auto lock_key = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->key : ((Item*)e)->key); };
	auto lock_name = [this](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->GetNameFrom((Room*)parent) : ((Item*)e)->name); };

	Exit* exit = GetRoom()->GetExit(args[1]);
	
	if (exit == NULL) {
			Item* l_item = (Item*)GetRoom()->Find(args[1], ITEM);
			if (l_item == NULL)
				l_item = (Item*)Find(args[1], ITEM);
			lock = l_item;
			lock->type = ITEM;
	}
	else {
		lock = exit;
		lock->type = EXIT;
	}

	if (lock == NULL || lock_locked(lock))
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL || lock_key(lock) != item)
		return false;

	if(PlayerInRoom())
		cout << "\n" << name << "locks " << lock_name(lock) << "...\n";

	(lock->type == EXIT ? ((Exit*)lock)->locked : ((Item*)lock)->locked) = true;

	return true;
}

// ----------------------------------------------------
bool Creature::UnLock(const vector<string>& args)
{
	/* Unlocks the specified Exit with an Item if it is the appropriate key. */
	if(!IsAlive())
		return false;

	Entity* lock;
	auto lock_locked = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->locked : ((Item*)e)->locked); };
	auto lock_key = [](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->key : ((Item*)e)->key); };
	auto lock_name = [this](Entity* e) { return (e->type == EXIT ? ((Exit*)e)->GetNameFrom((Room*)parent) : ((Item*)e)->name); };

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL) {
		Item* l_item = (Item*)GetRoom()->Find(args[1], ITEM);
		if (l_item == NULL)
			l_item = (Item*)Find(args[1], ITEM);
		lock = l_item;
		lock->type = ITEM;
	}
	else {
		lock = exit;
		lock->type = EXIT;
	}

	if (lock == NULL || lock_locked(lock)==false)
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL || lock_key(lock) != item)
		return false;

	if (PlayerInRoom())
		cout << "\n" << name << "unlocks " << lock_name(lock) << "...\n";

	(lock->type == EXIT ? ((Exit*)lock)->locked : ((Item*)lock)->locked) = false;

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
		cout << "\n" << name << " drops " << item->name << "...\n";
	
	if (item == weapon || item == armour)
		UnEquip(args);
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
void Creature::MakeAttack()
{
	/* Rolls damage from weapon and applies it to the comabt target Creature. It also makes this
	Creature the target of the combat target if it wasn't. */
	if(!IsAlive() || !combat_target->IsAlive()) // If either combatants are dead, remove targets.
	{
		combat_target = combat_target->combat_target = NULL;
		return;
	}

	int result = (weapon) ? weapon->GetValue() : Roll(min_damage, max_damage);
	if (weapon)
		result += (weapon->item_type == D_WEAPON) ? dexterity : strength;
	else
		result += strength;

	if(PlayerInRoom())
		cout << "\n" << name << " attacks " << combat_target->name << " for " << result << "\n";

	combat_target->ReceiveAttack(result);

	// make the attacker react and take me as a target
	if(combat_target->combat_target == NULL)
		combat_target->combat_target = this;

	return;
}

// ----------------------------------------------------
void Creature::Cast(vector<string>& args)
{
	/* Rolls damage/heal/other depending on the spell type. It also makes this
	Creature the target of the combat target if it wasn't. */
	if (!IsAlive()) // If  dead, return
	{
		return;
	}

	Spell* spell = FindSpell(args[1]);

	if (spell == NULL || mana_points-(spell->cost) < 0)
		return;

	Creature* target = (Creature*)parent->Find(args[3], CREATURE);
	
	if (target == NULL || !target->IsAlive()) // No target or dead
		return;

	mana_points -= spell->cost;

	// Create a functions map to deal with each type of spell.
	typedef void (Creature::*pfunc)(int);
	typedef map<SpellType, pfunc> spellMap;
	spellMap spell_map;
	spell_map[ATTACK] = &Creature::ReceiveMAttack;
	spell_map[BUFF] = &Creature::ReceiveBuff;
	spell_map[DEBUFF] = &Creature::ReceiveDebuff;
	spell_map[HEAL] = &Creature::ReceiveHeal;

	if (PlayerInRoom())
		cout << "\n" << name << " casts " << spell->name << " on " << target->name << "\n";

	int result = spell->GetValue() + intelligence;
	if (spell->spell_type == DEBUFF || spell->spell_type == BUFF)
		target_stat = spell->stat;

	(target->*(spell_map[spell->spell_type]))(result);
	if (spell->spell_type != spell->second_type) {
		int result2 = spell->GetValue() + intelligence;
		if (spell->second_type == DEBUFF || spell->second_type == BUFF)
			target->target_stat = spell->stat;

		(target->*(spell_map[spell->second_type]))(result2);
	}

	// make the attacked react and take me as a target
	if (target->combat_target == NULL)
		target->combat_target = this;

	return;
}

// ----------------------------------------------------
void Creature::ReceiveAttack(int damage)
{
	/* Subtract as many hit points as damage surpasses Creature's armour */
	int prot = (armour) ? armour->GetValue() : Roll(min_protection, max_protection);
	int received = (damage - prot- dexterity/2)>0 ? damage-prot-dexterity/2 : 0;

	hit_points -= received;

	if(PlayerInRoom())
		cout << name << " is hit for " << received << " damage (" << prot << " blocked, " << dexterity/2 << " dodged) \n";

	if(IsAlive() == false)
		Die();

	return;
}

// ----------------------------------------------------
void Creature::ReceiveMAttack(int damage)
{
	
	int prot = Roll(min_protection, max_protection);
	int received = (damage - prot - dexterity / 2) > 0 ? damage - prot - dexterity / 2 : 0;
	hit_points -= received;
	if (PlayerInRoom())
		cout << name << " is hit for " << received << " damage (" << prot << " deflected, " << dexterity / 2 << " dodged) \n";
	cout << "GG";
	if (IsAlive() == false)
		Die();

	return;
}

// ----------------------------------------------------
void Creature::ReceiveDebuff(int debuff)
{
	int prot = intelligence;
	int received = (debuff*2/3 - prot) > 0 ? debuff * 2 / 3 - prot : 0;
	switch (target_stat) {
	case STR:
		strength -= received;
		if (PlayerInRoom())
			cout << name << " is debuffed for " << received << " in strength.\n";
		break;
	case DEX:
		dexterity -= received;
		if (PlayerInRoom())
			cout << name << " is debuffed for " << received << " in dexterity.\n";
		break;
	case INT:
		intelligence -= received;
		if (PlayerInRoom())
			cout << name << " is debuffed for " << received << " in intelligence.\n";
		break;
	default:
		return;
	}

	return;
}

// ----------------------------------------------------
void Creature::ReceiveBuff(int buff)
{
	int ad_buff = buff * 2 / 3;
	switch (target_stat) {
	case STR:
		strength += ad_buff;
		if (PlayerInRoom())
			cout << name << " is buffed for " << ad_buff << " in strength.\n";
		break;
	case DEX:
		dexterity += ad_buff;
		if (PlayerInRoom())
			cout << name << " is buffed for " << ad_buff << " in dexterity.\n";
		break;
	case INT:
		intelligence += ad_buff;
		if (PlayerInRoom())
			cout << name << " is buffed for " << ad_buff << " in intelligence.\n";
		break;
	default:
		return;
	}

	return;
}

// ----------------------------------------------------
void Creature::ReceiveHeal(int heal)
{
	hit_points += heal;
	if (PlayerInRoom())
		cout << name << " is healed for " << heal << "\n";
	return;
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
		if (current_storage + i->item_size > capacity || !i->takeable) {
			if (PlayerInRoom())
				cout << "\n" << name << " cannot take any more objects.\n";
			return false;
		}
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
	cout << "\nMana Points: " << mana_points;
	cout << "\nAttack: (" << ((weapon) ? weapon->name : "no weapon") << ") ";
	cout << ((weapon) ? weapon->min_value : min_damage) << "-" << ((weapon) ? weapon->max_value : max_damage);
	cout << "\nProtection: (" << ((armour) ? armour->name : "no armour") << ") ";
	cout << ((armour) ? armour->min_value : min_protection) << "-" << ((armour) ? armour->max_value : max_protection);
	cout << "\nStrength: " << strength;
	cout << "\nDexterity: " << dexterity;
	cout << "\nIntelligence: " << intelligence;
	cout << "\n";
}

// ----------------------------------------------------
bool Creature::Use(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if (item == NULL)
		return false;

	switch (item->item_type)
	{
	case HP_POTION:
		hit_points += item->GetValue();
		container.remove(item);
		current_storage -= item->item_size;
		break;

	case MP_POTION:
		mana_points += item->GetValue();
		container.remove(item);
		current_storage -= item->item_size;
		break;

	default:
		return false;
	}

	if (PlayerInRoom())
		cout << "\n" << name << " uses " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
Spell* Creature::FindSpell(const string& name) const
{
	/* Iterates over the list of contained spells and returns the pointer to the Spell with the
	specified name and type */
	for (list<Spell*>::const_iterator it = spells_book.begin(); it != spells_book.cend(); ++it)
	{
		if (Same((*it)->name, name))
			return *it;
	}

	return NULL;
}

// ----------------------------------------------------
void Creature::AddSpell(Spell* spell)
{
	spells_book.push_back(spell);
}