# zork
Zork game to learn C++

## Introduction
Base code from [here](https://github.com/d0n3val/zork). The code was fixed for the game to behave as desired. 
Then, a bunch of features were added. For simplicity, the base code was maintained as much as possible and
most attributes are public (no getters and setters used). 

This is a Zork-like game. You will be a Hero that can move freely all over the "world" and interact with objects
and creatures. The world's "ticks" will advance every time a command is introduced (even if it is a not recognised command).
This means that if you are fighting a troll and cast a nonexistent spell the troll will attack you anyways, that is done to give a more realistic behaviour.
Once the conditions are met you are moved to the final battle against the fi al boss, and if you kill it you win the game.
If you die at any point, it's Game Over. 

## Extra Features
### Capacity
A capacity member was added for entities. It indicates the maximum size it can contain in objects.
When a new item is intended to be added to an Entity, it is checked if the remaining capacity is greater than
the object size. In the negative case, the item can be introduced into the other Entity.

It was also added an attribute for Item class that indicates if an Item can be taken or not.

### Combat
As explained above, the world "moves" every time the player inputs a command, so the combat is done in turns.
Once you attack a target you will be its combat target, and that doesn't wears out even if you move to another room (if you go back to
the enemy's Room it will attack you again). It was also added initial aggro for some Creatures.

Furthermore, Creatures have now more stats:
- Hit points.
- Mana points (see Magic feature).
- Strength it adds damage to your melee hits.
- Dexterity adds damage to your distance hits and contibutes to the protection.
- Intelligence adds score to Spells rolls and serves as protection against magic effects.
- Min-Max damage, roll used if no weapon.
- Min-Max protection, roll used if no armour.

The damage computation was modified to use the new stats.
Also, WEAPON item_type was divided into melee wepaon and distance weapon.
Then, the attack damage is the sum of wepaon/min-max damage roll plus strength if melee weapon or plus dexterity if distance weapon.
Finally, the received damage is reduced by the protection roll + half dexterity stat of the Creature receiving damage.

The NPC behaviour when attacked is: 33% attacks, 66% casts a known spell if enough mana.

### Introduction and epilogue
A storyline is introduced when the game is started and some Items are marked as objective. When the player has all the objectives the game takes him to the final scene/battle.

### Lock and hidden
Now some Items can be locked also, like Exits. Furthermore, the hidden property is added. An exit or an Item that is hidden, is not listed when the player looks around, even if specificaly searching for it.
Also, the Items can be "moved" around without taking them. If you move an Item that was hiding something the hidden Entity is revealed.

### Magic system
A new class Spell was added. Spells are Entities that can be casted with the command "cast". They have a min and max value to determine the specific effect and can have 
up to 2 types of effects. Each effect type can be HEAL, make ATTACK, BUFF a stat and DEBUFF a stat.

Spells can be cast on any target (yourself or other Creatures), so yes, you can heal your enemy. The numeric effect of a Spell (that is, the damage or the HP healed, or the stat buff) is the result of the Roll + Intelligence of the caster.
The effect however, if it's a damage or a debuff, is mitigated by the target's Intelligence.

Creatures have a Spell Book which is the "inventory" of known spells. Each Spell has a mana cost, if a Creature hasn't got enough mana_points left, it can't cast the Spell.

### Usable objects
Some item_type's were added. Now there are HP_POTIONs MP_POTIONs and SCROLLS. The first two types heal the corresponding stat, and the SCROLL, when used, adds a specific Spell to the player's Spells Book.

Each of this item_type's are single-use and they are consumed after usage.

## How to play
The game works by commands. There commands with 0, 1 or 3 arguments. When you areive at a Room all its description and all Entities there, are listed.
The visible Exits will be listed with its "direction name" between parenthesis.

Now then, the commands:
- Look. With 0 arguments outputs Room description. With one argument outputs the specified Entity description and, if it contains something and it's not locked, it also outputs it.
- Go direction_name. Moves the player to the room that Exit points.
- Take item_name. Takes specified Item from the Room and puts in players inventory if allowed. If the command is "take item_name from other_item_name", tries to take an item from inside another item, the second currently being either in Inventory or in Room.
- Drop item_name. Inverse of take command. It moves an Item on player's inventory to the current Room. If the command is "drop item_name to other_item_name", tries to move it inside another Item.
- Equip/unequip item_name. Sets/unsets specified Item as weapon or armour if possible.
- Use item_name. Uses and consumes specified Item from Inventory if possible.
- Inventory. Lists Player's possessions and possible Items inside them.
- Spells. Lists known Player's known spells (Spells on Spells Book).
- Loot creature_name. Puts all possessions from a DEAD Creature on Player's inventory.
- Stats. Lists player's stats.
- Examine creature_name. Lists specified creature's stats.
- Lock/Unlock entity_name with key_name. Tries to lock/unlock an Item or Exit with the specified Item as key. It will only be accomplish if it's the correct key and if the target is not already locked/unlocked.
- Move item_name. Moves the specified Item on the Room. If it was hiding something, it is revealed.
- Attack creature_name. Performs an attack against the specified creature, with the damage computation specified above. Uses equipped weapon if any.
- Cast spell_name on creature_name. Applies the Spells' effect/s on the target. The target can also be "me" if the player wants to cadt it on itself.
- Read spell_name. Outputs the description of specified Spell if it is on the Spells Book.

This are the main commands. Complete list and aliases for them can be found on 'world.cpp'.

## Author
Pol Pérez Granero- [Mefiso](https://github.com/Mefiso)
Code [here](https://github.com/Mefiso/zork)
