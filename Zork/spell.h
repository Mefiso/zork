#ifndef __Spell__
#define __Spell__

#include <string>
#include "entity.h"

using namespace std;

enum SpellType
{
	ATTACK,
	BUFF,
	DEBUFF,
	HEAL
};

enum StatEffect {
	STR,
	DEX,
	INT
};

class Spell : public Entity
{
public:
	Spell(const char* name, const char* description,  SpellType spell_type, SpellType second_type, int cost);
	~Spell();

	int GetValue() const;

public:
	int min_value;
	int max_value;
	SpellType spell_type;
	SpellType second_type;
	int cost;
	StatEffect stat;

};


#endif //__Spell__
