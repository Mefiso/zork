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
Then, the attack damage is the sum of wepaon/min-max damage roll

