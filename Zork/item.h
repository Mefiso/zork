#ifndef __Item__
#define __Item__

#include <string>
#include "entity.h"
#include "spell.h"

class Room;

using namespace std;

enum ItemType
{
	COMMON,
	M_WEAPON,
	D_WEAPON,
	ARMOUR,
	HP_POTION,
	MP_POTION,
	SCROLL
};

class Item : public Entity
{
public:
	Item(const char* name, const char* description, Entity* parent, const int capacity, const int size, bool takeable = true, ItemType item_type = COMMON);
	~Item();

	void Look() const;
	void ChangeParentTo(Entity* new_parent);
	int GetValue() const;

public :
	int min_value;
	int max_value;
	ItemType item_type;
	int item_size;
	bool locked;
	Entity* key;
	bool hidden;
	Entity* hiding;
	std::string move_description;
	bool takeable;
	Spell* spell;
};

#endif //__Item__