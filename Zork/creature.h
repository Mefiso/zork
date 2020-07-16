#ifndef __Creature__
#define __Creature__

#include <string>
#include "entity.h"
#include "spell.h"

class Room;
class Item;
class Spell;

using namespace std;

class Creature : public Entity
{
public:
	Creature(const char* name, const char* description, Room* room, const int capacity);
	~Creature();

	virtual bool Go(const vector<string>& args);
	virtual void Look(const vector<string>& args) const;
	virtual bool Take(const vector<string>& args);
	virtual bool Drop(const vector<string>& args);
	virtual void Inventory() const;
	virtual bool Equip(const vector<string>& args);
	virtual bool UnEquip(const vector<string>& args);
	virtual bool AutoEquip();
	virtual bool Lock(const vector<string>& args);
	virtual bool UnLock(const vector<string>& args);
	virtual void Tick();

	virtual bool Attack(const vector<string>& args);
	virtual void MakeAttack();
	virtual void Cast(vector<string>& args);
	virtual void ReceiveAttack(int damage);
	virtual void ReceiveMAttack(int damage);
	virtual void ReceiveDebuff(int debuff);
	virtual void ReceiveBuff(int buff);
	virtual void ReceiveHeal(int heal);
	virtual void Die();
	virtual bool Loot(const vector<string>& args);
	virtual void Stats() const;
	virtual bool Use(const vector<string>& args);
	virtual void AddSpell(Spell* spell);
	virtual void SpellsBook() const;

	Room* GetRoom() const;
	bool PlayerInRoom() const;
	bool IsAlive() const;
	Spell* FindSpell(const string& name) const;

public:

	int hit_points;
	int mana_points;
	int min_damage;
	int max_damage;
	int min_protection;
	int max_protection;
	int strength;
	int intelligence;
	int dexterity;
	Creature* combat_target;
	Item* weapon;
	Item* armour;
	list <Spell*> spells_book;
	StatEffect target_stat;
	
};

#endif //__Creature__