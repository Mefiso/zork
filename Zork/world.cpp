#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"

// ----------------------------------------------------
World::World()
{
	tick_timer = clock();

	// Rooms ----
	Room* forest = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	Room* house = new Room("House", "You are inside a beautiful but small white house.");
	Room* basement = new Room("Basement", "The basement features old furniture and dim light.");
	Room* beach = new Room("Beach", "You find yourself in a sandy shore. The beatiful blue sea extends before your eyes.");
	Room* cemetery = new Room("Cemetery", "You are in a silent and gloomy graveyard in the middle of the forest.");
	Room* hillside = new Room("Hillside", "A medium sized hill rises before you. The small amount of trees in this area let you see a cave on the slope.");
	Room* hilltop = new Room("Hilltop", "Wind's howling. From the peak you can see the forest and the vast shore on the other side of the house.");
	Room* cave = new Room("Cave", "You are in a cold, damp cavern. Almost no light enters this place.");
	Room* secret_lab = new Room("Laboratory", "The secret laboratory of an alchemist. Crysal pipes, flasks, liquids and fumes all over the place.");
	Room* treasure_room = new Room("Grotto", "You enter deeper in the cave. Here the high ceiling is open and you can see the sky.");
	final_room = new Room("Hell", "Obscure large room, with little floating fires on the corners. It is made of dark grey stone, you can't see the ceiling, only darkness. At the end of the room there's a large Throne.");

	Exit* ex1 = new Exit("east", "west", "Little path", house, forest);
	Exit* ex2 = new Exit("down", "up", "Stairs", house, basement);
	Exit* ex3 = new Exit("westdoor", "east", "There's a white wooden door.", house, beach);
	Exit* ex4 = new Exit("south", "north", "Crooked trail.", forest, cemetery);
	Exit* ex5 = new Exit("northwest", "southeast", "Old pathway.", forest, hillside);
	Exit* ex6 = new Exit("up", "down", "Mountain route.", hillside, hilltop);
	Exit* ex7 = new Exit("northeast", "outside", "Cave entrance.", hillside, cave);
	Exit* ex8 = new Exit("west", "east", "A metal door.", basement, secret_lab);
	Exit* ex9 = new Exit("up", "down", "A secret passage communicates two levels of the cavern.", cave, treasure_room);
	ex3->locked = true;
	ex8->locked = true;
	ex8->hidden = true;
	ex9->hidden = true;

	entities.push_back(forest);
	entities.push_back(house);
	entities.push_back(basement);
	entities.push_back(beach);
	entities.push_back(cemetery);
	entities.push_back(hillside);
	entities.push_back(hilltop);
	entities.push_back(cave);
	entities.push_back(secret_lab);
	entities.push_back(treasure_room);

	entities.push_back(ex1);
	entities.push_back(ex2);
	entities.push_back(ex3);
	entities.push_back(ex4);
	entities.push_back(ex5);
	entities.push_back(ex6);
	entities.push_back(ex7);
	entities.push_back(ex8);
	entities.push_back(ex9);

	// Creatures ----
	Creature* butler = new Creature("Butler", "It's James, the house Butler.", house, 8);
	butler->hit_points = 10;
	butler->dexterity = 1;
	butler->intelligence = 1;
	Creature* witch = new Creature("Witch", "It's the Witch of the Hill flying on its broomstick.", hilltop, 12);
	witch->hit_points = 15;
	witch->max_damage = 1;
	witch->max_protection = 2;
	witch->intelligence = 3;
	witch->dexterity = 2;
	Creature* troll = new Creature("Troll", "A cave troll. Seems aggresive.", cave, 18);
	troll->hit_points = 23;
	troll->min_damage = 2;
	troll->max_damage = 4;
	troll->min_protection = 3;
	troll->max_protection = 5;
	troll->strength = 5;
	Creature* ghost = new Creature("Ghost", "It's a sorrowful wraith.", cemetery, 0);
	ghost->hit_points = 10;
	ghost->min_damage = 1;
	ghost->max_damage = 3;
	ghost->min_protection = ghost->max_protection = 7;
	ghost->intelligence = 2;
	ghost->dexterity = 3;
	final_boss = new Creature("Mephistopheles", "The Prince of Hell. A humanoid demon with red skin, large horns and devious face. It has three unmatched pairs of wings.", final_room, 35);

	entities.push_back(butler);
	entities.push_back(witch);
	entities.push_back(troll);
	entities.push_back(ghost);

	// Items -----
	Item* mailbox = new Item("Mailbox", "Looks like it might contain something.", house, 3, 3);
	Item* key = new Item("Key", "Old iron key.", mailbox, 0, 1);
	ex3->key = key;
	Item* sack = new Item("Sack", "Brown old sack. Looks like it might contain something.", forest, 5, 4);
	Item* apple = new Item("Apple", "A perfect red apple.", sack, 0, 1);
	Item* crystal_ball = new Item("Palantir", "The witch uses it as a clairvoyant ball. It's The Purple Eye.", forest, 0, 2);
	Item* chest = new Item("Chest", "Wooden small chest with iron lock.", cave, 4, 3);
	chest->locked = true;
	Item* pouch = new Item("Pouch", "Pouch filled with gold pieces.", chest, 0, 2);
	Item* black_key = new Item("Black-key", "A black iron key.", witch, 0, 1);
	chest->key = black_key;
	Item* gold_key = new Item("Bronze-key", "A clean and nice bronze key.", basement, 0, 1);
	ex8->key = gold_key;
	Item* wardrobe = new Item("Wardrobe", "Old oak wooden wardrobe with double door.", basement, 14, 14, false);
	wardrobe->hiding = ex8;
	wardrobe->move_description = "On the wall behind where the wardrobe was, a mysterious door is revealed.";
	Item* rock = new Item("Rock", "Huge heavy rock.", cave, 0, 7);
	rock->hiding = ex9;
	rock->move_description = "On the wall behind, it appears a secret passage that goes up, deeper in the cavern.";
	Item* red_stone = new Item("Redstone", "This ardent stone contains the power of infernal fire. It's Mephistopheles's Fire", forest, 0, 2);
	Item* black = new Item("Black-Sphere", "The dark sphere emits a relentless buzz... It makes you feel sick, sends shivers down your spine and drains your life. It's the Black Third.", forest, 0, 2);

	// Weapons and armours --
	Item* sword = new Item("Sword", "A simple old and rusty sword.", forest, 0, 4, true, M_WEAPON);
	sword->min_value = 2;
	sword->max_value = 6;

	Item* sword2 = new Item(*sword);
	sword2->ChangeParentTo(butler);

	Item* broom = new Item("Broomstick", "A crooked, old and dusty broomstick.", witch, 0, 6, true, M_WEAPON);
	broom->min_value = 0;
	broom->max_value = 3;

	Item* knife = new Item("Knife", "A nasty, blood covered big knife.", witch, 0, 2, true, M_WEAPON);
	knife->min_value = 2;
	knife->max_value = 5;

	Item* club = new Item("Greatclub", "Dangerous-looking club of great size.", troll, 0, 7, true, M_WEAPON);
	club->min_value = 3;
	club->max_value = 8;

	Item* shield = new Item("Shield", "An old wooden shield.", butler, 0, 5, true, ARMOUR);
	shield->min_value = 1;
	shield->max_value = 3;
	
	butler->AutoEquip();
	witch->AutoEquip();
	troll->AutoEquip();


	entities.push_back(key);
	entities.push_back(mailbox);
	entities.push_back(sack);
	entities.push_back(apple);
	entities.push_back(crystal_ball);
	entities.push_back(chest);
	entities.push_back(pouch);
	entities.push_back(black_key);
	entities.push_back(gold_key);
	entities.push_back(wardrobe);
	entities.push_back(rock);
	entities.push_back(red_stone);
	entities.push_back(black);

	entities.push_back(sword);
	entities.push_back(sword2);
	entities.push_back(broom);
	entities.push_back(knife);
	entities.push_back(club);
	entities.push_back(shield);

	// Player ----
	player = new Player("Hero", "You are an awesome adventurer!", forest, 15);
	player->hit_points = 20;
	player->strength = 1;
	player->dexterity = 2;
	player->intelligence = 2;
	entities.push_back(player);

	// Objectives ----
	objectives.push_back(crystal_ball);
	objectives.push_back(red_stone);
	objectives.push_back(black);

	ended = false;
}

// ----------------------------------------------------
World::~World()
{
	/* Deletes every entity in the world */
	for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;

	entities.clear();
}

// ----------------------------------------------------
bool World::Tick(vector<string>& args)
{
	bool ret = true; // if there aren't commands in args, returns true

	if (args.size() > 0 && args[0].length() > 0) {
		ret = ParseCommand(args); // analize command in args
		GameLoop(); // World only ticks when a command is inputted
	}
	return ret;
}

// ----------------------------------------------------
void World::GameLoop()
{
	/* Calls every entity Tick method if 0.5s have passed since last tick */
	clock_t now = clock();

	if((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Tick();

		tick_timer = now;
	}
}

// ----------------------------------------------------
bool World::ParseCommand(vector<string>& args)
{
	/* Calls the appropriate player method depending on the command and the number of arguments.
	Returns false in case of unknown command. */
	bool ret = true;

	/* Some player methods return true or false but that value is not used here. They could be void
	but it was preferred to maintain the boolean return for possible future usages. */ 
	switch(args.size())
	{
		case 1: // commands with no arguments ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "north") || Same(args[0], "n"))
			{
				(args.size() == 1) ? args.push_back("north") : args[1] = "north";
				player->Go(args);
			}
			else if(Same(args[0], "south") || Same(args[0], "s"))
			{
				(args.size() == 1) ? args.push_back("south") : args[1] = "south";
				player->Go(args);
			}
			else if(Same(args[0], "east") || Same(args[0], "e"))
			{
				(args.size() == 1) ? args.push_back("east") : args[1] = "east";
				player->Go(args);
			}
			else if(Same(args[0], "west") || Same(args[0], "w"))
			{
				(args.size() == 1) ? args.push_back("west") : args[1] = "west";
				player->Go(args);
			}
			else if(Same(args[0], "up") || Same(args[0], "u"))
			{
				(args.size() == 1) ? args.push_back("up") : args[1] = "up";
				player->Go(args);
			}
			else if(Same(args[0], "down") || Same(args[0], "d"))
			{
				(args.size() == 1) ? args.push_back("down") : args[1] = "down";
				player->Go(args);
			}
			else if(Same(args[0], "stats") || Same(args[0], "st"))
			{
				player->Stats();
			}
			else if(Same(args[0], "inventory") || Same(args[0], "i"))
			{
				player->Inventory();
			}
			else
				ret = false;
			break;
		}
		case 2: // commands with one argument ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "go"))
			{
				player->Go(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else if(Same(args[0], "equip") || Same(args[0], "eq"))
			{
				player->Equip(args);
			}
			else if(Same(args[0], "unequip") || Same(args[0], "uneq"))
			{
				player->UnEquip(args);
			}
			else if(Same(args[0], "examine") || Same(args[0], "ex"))
			{
				player->Examine(args);
			}
			else if(Same(args[0], "attack") || Same(args[0], "at"))
			{
				player->Attack(args);
			}
			else if(Same(args[0], "loot") || Same(args[0], "lt"))
			{
				player->Loot(args);
			}
			else if (Same(args[0], "move") || Same(args[0], "mv"))
			{
				player->Move(args);
			}
			else
				ret = false;
			break;
		}
		case 3: // commands with two arguments ------------------------------
		{
			break;
		}
		case 4: // commands with three arguments ------------------------------
		{
			if(Same(args[0], "unlock") || Same(args[0], "unlk"))
			{
				player->UnLock(args);
			}
			else if(Same(args[0], "lock") || Same(args[0], "lk"))
			{
				player->Lock(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else
				ret = false;
			break;
		}
		default:
		ret =  false;
	}

	return ret;
}

bool World::CheckEndConditions() {
	for (list<Entity*>::const_iterator it = entities.begin(); it != entities.cend(); ++it)
	{
		if ((find(objectives.begin(), objectives.end(), (*it)) != objectives.end()) && (*it)->parent != player)
			return false;
	}
	if (!ended) {
		ended = true;
		return true;
	}
	else
		return false;
}

bool World::GameOver()
{
	if (player->hit_points <= 0)
		return true;
	return false;
}

void World::Ending()
{
	player->ChangeParentTo(final_room);
}

bool World::Win()
{
	if (final_boss->hit_points <= 0)
		return true;
	return false;
}