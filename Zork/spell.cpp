#include <iostream>
#include "globals.h"
#include "spell.h"

// ----------------------------------------------------
Spell::Spell(const char* name, const char* description, SpellType spell_type, SpellType second_type, int cost) :
	Entity(name, description, 0, NULL),
	spell_type(spell_type), second_type(second_type), cost(cost)
{
	type = SPELL;
	min_value = max_value = 0;
}

// ----------------------------------------------------
Spell::~Spell()
{}

// ----------------------------------------------------
int Spell::GetValue() const
{
	/* Returns a random value between min and max if max>0, otherwise returns 0 */
	return Roll(min_value, max_value);
}