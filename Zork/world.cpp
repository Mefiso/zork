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
	Room* secret_lab = new Room("Laboratory", "The secret laboratory of an alchemist. Crystal pipes, flasks, liquids and fumes all over the place.");
	Room* treasure_room = new Room("Grotto", "You enter deeper in the cave. Here the high ceiling is open and you can see the sky.");
	Room* witch_hut = new Room("Hut", "A small wooden cottage. It's a piggery: dusty shelves, broken flasks, smoking beberages...");
	Room* glade = new Room("Glade", "You enter a hidden forest glade. Beatiful strong trees surround the place. An altar of stone stands in the middle.");
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
	Exit* ex10 = new Exit("west", "door", "A rotten wooden door.", hilltop, witch_hut);
	Exit* ex11 = new Exit("east", "west", "A passage through a great tree to another part of the forest.", forest, glade);
	ex3->locked = true;
	ex8->locked = true;
	ex8->hidden = true;
	ex9->hidden = true;
	ex10->hidden = true;
	ex11->hidden = true;

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
	entities.push_back(witch_hut);
	entities.push_back(glade);
	entities.push_back(final_room);

	entities.push_back(ex1);
	entities.push_back(ex2);
	entities.push_back(ex3);
	entities.push_back(ex4);
	entities.push_back(ex5);
	entities.push_back(ex6);
	entities.push_back(ex7);
	entities.push_back(ex8);
	entities.push_back(ex9);
	entities.push_back(ex10);
	entities.push_back(ex11);

	// Creatures ----
	Creature* butler = new Creature("Butler", "It's James, the house Butler.", house, 8);
	butler->hit_points = 10;
	butler->dexterity = 1;
	butler->intelligence = 1;
	Creature* witch = new Creature("Witch", "It's the Witch of the Hill flying on its broomstick.", hilltop, 13);
	witch->hit_points = 17;
	witch->mana_points = 13;
	witch->max_damage = 1;
	witch->max_protection = 2;
	witch->intelligence = 4;
	witch->dexterity = 2;
	Creature* troll = new Creature("Troll", "A cave troll. Seems aggresive.", cave, 18);
	troll->hit_points = 23;
	troll->min_damage = 2;
	troll->max_damage = 4;
	troll->min_protection = 3;
	troll->max_protection = 5;
	troll->strength = 5;
	Creature* ghost = new Creature("Ghost", "It's a sorrowful wraith.", cemetery, 0);
	ghost->hit_points = 11;
	ghost->mana_points = 11;
	ghost->min_damage = 1;
	ghost->max_damage = 3;
	ghost->min_protection = 6;
	ghost->max_protection = 7;
	ghost->intelligence = 2;
	ghost->dexterity = 3;
	Creature* alchemist = new Creature("Alchemist", "The alchemist that lives in the white house. He has an intelligent gaze.", secret_lab, 10);
	alchemist->hit_points = 13;
	alchemist->mana_points = 15;
	alchemist->min_damage = 1;
	alchemist->max_damage = 2;
	alchemist->intelligence = 5;
	alchemist->dexterity = 3;
	Creature* drake = new Creature("Dragon", "A powerful, majestic golden dragon. It looks like it wants to eat you.", treasure_room, 35);
	drake->hit_points = 29;
	drake->mana_points = 15;
	drake->min_damage = 3;
	drake->max_damage = 5;
	drake->min_protection = 5;
	drake->max_protection = 7;
	drake->strength = 5;
	drake->intelligence = 4;
	drake->dexterity = 2;
	final_boss = new Creature("Mephistopheles", "The Prince of Hell. A humanoid demon with red skin, large horns and devious face. It has three unmatched pairs of wings.", final_room, 35);
	final_boss->hit_points = 37;
	final_boss->mana_points = 19;
	final_boss->min_damage = 3;
	final_boss->max_damage = 7;
	final_boss->min_protection = 5;
	final_boss->max_protection = 7;
	final_boss->strength = 4;
	final_boss->intelligence = 4;
	final_boss->dexterity = 4;

	entities.push_back(butler);
	entities.push_back(witch);
	entities.push_back(troll);
	entities.push_back(ghost);
	entities.push_back(alchemist);
	entities.push_back(drake);
	entities.push_back(final_boss);

	// Items -----
	Item* mailbox = new Item("Mailbox", "Looks like it might contain something.", house, 3, 3);
	Item* key = new Item("Key", "Old iron key.", mailbox, 0, 1);
	ex3->key = key;

	Item* sack = new Item("Sack", "Brown old sack. Looks like it might contain something.", forest, 5, 4);
	Item* apple = new Item("Apple", "A perfect red apple.", sack, 0, 1, true, HP_POTION);
	apple->min_value = 2;
	apple->max_value = 4;
	Item* gravestone = new Item("Gravestone", "A gravestone at the feet of a large tree on the east side.", forest, 0, 6, false);
	gravestone->hiding = ex11;
	gravestone->move_description = "On the immense tree immediately behind the tombstone, it appears a secret passage to the east.";
	
	Item* crystal_ball = new Item("Palantir", "The witch uses it as a clairvoyant ball. It's The Purple Eye.", witch_hut, 0, 2);
	Item* hp1 = new Item("Light_HP_potion", "A diminute flask containing a red beberage.", witch_hut, 0, 1, true, HP_POTION);
	hp1->min_value = 3;
	hp1->max_value = 8;
	Item* mp1 = new Item("Medium_MP_potion", "A flask containing a blue beberage.", witch_hut, 0, 2, true, MP_POTION);
	mp1->min_value = 3;
	mp1->max_value = 6;
	
	Item* rock = new Item("Rock", "Huge heavy rock.", cave, 0, 7);
	rock->hiding = ex9;
	rock->move_description = "On the wall behind, it appears a secret passage that goes up, deeper in the cavern.";
	Item* stone = new Item("Stone", "A nice rock.", cave, 0, 3);
	Item* chest = new Item("Chest", "Wooden small chest with iron lock.", cave, 4, 3);
	chest->locked = true;
	Item* pouch = new Item("Pouch", "Pouch filled with gold pieces.", chest, 0, 2);
	
	Item* black_key = new Item("Black-key", "A black iron key.", witch, 0, 1);
	chest->key = black_key;
	
	Item* gold_key = new Item("Bronze-key", "A clean and nice bronze key.", cemetery, 0, 1);
	ex8->key = gold_key;
	Item* tomb = new Item("Tombstone", "A small tombstone that stands out because it's more worn-out.", cemetery, 0, 6);
	tomb->move_description = "The tomb is opened. There's no corpse nor skeleton. Instead, a dirty scroll.";

	Item* wardrobe = new Item("Wardrobe", "Old oak wooden wardrobe with double door.", basement, 14, 14, false);
	wardrobe->hiding = ex8;
	wardrobe->move_description = "On the wall behind where the wardrobe was, a mysterious door is revealed.";
	Item* tunic = new Item("Tunic", "Nice purple tunic with hood.", wardrobe, 0, 0);

	Item* red_stone = new Item("Redstone", "This ardent stone contains the power of infernal fire. It's Mephistopheles's Fire", treasure_room, 0, 2);
	
	Item* black = new Item("Black-Sphere", "The dark sphere emits a relentless buzz... It makes you feel sick, sends shivers down your spine and drains your life. It's the Black Third.", secret_lab, 0, 2);
	Item* hp2 = new Item("Medium_HP_potion", "A flask containing a red beberage.", secret_lab, 0, 2, true, HP_POTION);
	hp2->min_value = 5;
	hp2->max_value = 10;
	Item* mp2 = new Item("Small_MP_potion", "A diminute flask containing a blue beberage.", alchemist, 0, 2, true, MP_POTION);
	mp2->min_value = 1;
	mp2->max_value = 4;

	Item* monolith = new Item("Monolith", "A large monolith in the middle of the peak. It has some runes inscribed on it.", hilltop, 0, 14, false);
	monolith->hiding = ex10;
	monolith->move_description = "It only moves an inch, but when you touch it the runes suddenly start to glow. On the west side of the top it appears an old hut that was turned invisible by some spell.";

	Item* altar = new Item("Altar", "It is a stone alter with ancient runes. It emanates a magic aura.", glade, 4, 6, false);
	Item* leaves = new Item("Leaves", "A bunch of tree leaves.", glade, 0, 0);

	// Weapons and armours --
	Item* sword = new Item("Sword", "A simple old and rusty sword.", forest, 0, 4, true, M_WEAPON);
	sword->min_value = 2;
	sword->max_value = 6;

	Item* sword2 = new Item(*sword);
	sword2->ChangeParentTo(butler);

	Item* shortbow = new Item("Shortbow", "A simple wooden shortbow", beach, 0, 3, true, D_WEAPON);
	shortbow->min_value = 1;
	shortbow->max_value = 5;

	Item* longbow = new Item("Longbow", "A strong, compound wooden bow", treasure_room, 0, 5, true, D_WEAPON);
	longbow->min_value = 2;
	longbow->max_value = 9;

	Item* broom = new Item("Broomstick", "A crooked, old and dusty broomstick.", witch, 0, 6, true, M_WEAPON);
	broom->min_value = 0;
	broom->max_value = 3;

	Item* knife = new Item("Knife", "A nasty, blood covered big knife.", witch, 0, 2, true, M_WEAPON);
	knife->min_value = 3;
	knife->max_value = 4;

	Item* club = new Item("Greatclub", "Dangerous-looking club of great size.", troll, 0, 7, true, M_WEAPON);
	club->min_value = 3;
	club->max_value = 8;

	Item* shield = new Item("Shield", "An old wooden shield.", butler, 0, 5, true, ARMOUR);
	shield->min_value = 1;
	shield->max_value = 3;

	Item* padded = new Item("Padded-armor", "Padded torso armor.", alchemist, 0, 3, true, ARMOUR);
	padded->min_value = 1;
	padded->max_value = 2;

	Item* cuirass = new Item("Cuirass", "A plated cuirass", treasure_room, 0, 5, true, ARMOUR);
	cuirass->min_value = 3;
	cuirass->max_value = 4;

	Item* trident = new Item("Trident", "Scarlet giant trident. It holds the power of Hell.", final_boss, 0, 7, true, M_WEAPON);
	trident->min_value = 5;
	trident->max_value = 9;

	Item* mastersword = new Item("Mastersword", "A beautifully crafted magical sword. It holds the power of wind and forest.", altar, 0, 4, true, M_WEAPON);
	mastersword->min_value = 4;
	mastersword->max_value = 8;

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
	entities.push_back(gravestone);
	entities.push_back(hp1);
	entities.push_back(mp1);
	entities.push_back(monolith);
	entities.push_back(stone);
	entities.push_back(tomb);
	entities.push_back(tunic);
	entities.push_back(hp2);
	entities.push_back(mp2);
	entities.push_back(altar);
	entities.push_back(leaves);

	entities.push_back(sword);
	entities.push_back(sword2);
	entities.push_back(broom);
	entities.push_back(knife);
	entities.push_back(club);
	entities.push_back(shield);
	entities.push_back(shortbow);
	entities.push_back(longbow);
	entities.push_back(padded);
	entities.push_back(cuirass);
	entities.push_back(trident);
	entities.push_back(mastersword);

	// Player ----
	player = new Player("Hero", "You are an awesome adventurer!", forest, 15);
	player->hit_points = 20;
	player->mana_points = 13;
	player->strength = 1;
	player->dexterity = 2;
	player->intelligence = 2;
	entities.push_back(player);
	ghost->combat_target = player;
	troll->combat_target = player;
	drake->combat_target = player;
	final_boss->combat_target = player;

	// Spells and Scrolls---
	Spell* stupidity = new Spell("Stupidity", "A tricky spell. It can both harm and heal.", ATTACK, HEAL, 2);
	stupidity->min_value = 0;
	stupidity->max_value = 6;
	Item* stupid_scroll = new Item("Stupidity-scroll", "A magic scripture. Allows to learn the Stupidity spell.", forest, 0, 1, true, SCROLL);
	stupid_scroll->spell = stupidity;

	Spell* enrage = new Spell("Enrage", "Deals huge damage but increases the Strength of the target.", ATTACK, BUFF, 3);
	enrage->stat = STR;
	enrage->min_value = 7;
	enrage->max_value = 10;
	Item* enrage_scroll = new Item("Enrage-scroll", "A magic scripture. Allows to learn the Enrage spell.", chest, 0, 1, true, SCROLL);
	enrage_scroll->spell = enrage;
	
	Spell* shock = new Spell("Shock", "A shock wave that deals damage and reduces dexterity.", ATTACK, DEBUFF, 2);
	shock->stat = DEX;
	shock->min_value = 3;
	shock->max_value = 5;
	ghost->AddSpell(shock);
	drake->AddSpell(shock);
	Item* shock_scroll = new Item("Shock-scroll", "A magic scripture. Allows to learn the Shock spell.", cemetery, 0, 1, true, SCROLL);
	shock_scroll->hidden = true;
	tomb->hiding = shock_scroll;
	shock_scroll->spell = shock;

	Spell*  black_blood = new Spell("BlackBlood", "Uses corrupted blood to heal, but it also can equally harm.", HEAL, ATTACK, 3);
	black_blood->min_value = 0;
	black_blood->max_value = 6;
	witch->AddSpell(black_blood);
	final_boss->AddSpell(black_blood);
	Item* blackblood_scroll = new Item("BlackBlood-scroll", "A magic scripture. Allows to learn the BlackBlood spell.", witch, 0, 1, true, SCROLL);
	blackblood_scroll->spell = black_blood;

	Spell* ultra_instinct = new Spell("UltraInstinct", "Heals and increases dexerity.", HEAL, BUFF, 4);
	ultra_instinct->stat = DEX;
	ultra_instinct->min_value = 3;
	ultra_instinct->max_value = 6;
	alchemist->AddSpell(ultra_instinct);
	final_boss->AddSpell(ultra_instinct);
	Item* ui_scroll = new Item("UltraInstinct-scroll", "A magic scripture. Allows to learn the UltraInstinct spell.", secret_lab, 0, 1, true, SCROLL);
	ui_scroll->spell = ultra_instinct;

	Spell* oblivion = new Spell("Oblivion", "A powerful healing that debuffs massively the Intelligence.", HEAL, DEBUFF, 1);
	oblivion->stat = INT;
	oblivion->min_value = 15;
	oblivion->max_value = 20; // Yes, that means you won't use magic effectively anymore
	Item* oblivion_scroll = new Item("Oblivion-scroll", "A magic scripture. Allows to learn the Oblivion spell.", treasure_room, 0, 1, true, SCROLL);
	oblivion_scroll->spell = oblivion;

	Spell* corruption = new Spell("Corruption", "Corrupts target's mind.", DEBUFF, DEBUFF, 3);
	corruption->stat = INT;
	corruption->min_value = 1;
	corruption->max_value = 3;
	witch->AddSpell(corruption);

	Spell* wind_howl = new Spell("Windhowl", "Increases a lot dexerity.", BUFF, BUFF, 3);
	wind_howl->stat = DEX;
	wind_howl->min_value = 3;
	wind_howl->max_value = 7;
	alchemist->AddSpell(wind_howl);
	drake->AddSpell(wind_howl);
	Item* wind_scroll = new Item("Windhowl-scroll", "A magic scripture. Allows to learn the Windhowl spell.", glade, 0, 1, true, SCROLL);
	wind_scroll->spell = wind_howl;
	wind_scroll->hidden = true;
	leaves->hiding = wind_scroll;

	Spell* cure = new Spell("Cure", "A magical healing.", HEAL, HEAL, 2);
	cure->min_value = 1;
	cure->max_value = 5;
	alchemist->AddSpell(cure);
	final_boss->AddSpell(cure);
	Item* cure_scroll = new Item("Cure-scroll", "A magic scripture. Allows to learn the Cure spell.", beach, 0, 1, true, SCROLL);

	Spell* spark = new Spell("Spark", "A lightning energy strike.", ATTACK, ATTACK, 1);
	cure->min_value = 1;
	cure->max_value = 3;
	alchemist->AddSpell(spark);
	witch->AddSpell(spark);
	
	Spell* fireball = new Spell("Fireball", "A flaming sphere of huge size that explodes on impact.", ATTACK, ATTACK, 5);
	fireball->min_value = 5;
	fireball->max_value = 9;
	drake->AddSpell(fireball);
	final_boss->AddSpell(fireball);

	entities.push_back(stupidity);
	entities.push_back(stupid_scroll);
	entities.push_back(enrage);
	entities.push_back(enrage_scroll);
	entities.push_back(shock);
	entities.push_back(shock_scroll);
	entities.push_back(black_blood);
	entities.push_back(blackblood_scroll);
	entities.push_back(ultra_instinct);
	entities.push_back(ui_scroll);
	entities.push_back(oblivion);
	entities.push_back(oblivion_scroll);

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
			else if (Same(args[0], "spells") || Same(args[0], "sp"))
			{
				player->SpellsBook();
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
			else if (Same(args[0], "use") || Same(args[0], "u"))
			{
				player->Use(args);
			}
			else if (Same(args[0], "read") || Same(args[0], "rd"))
			{
				player->Read(args);
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
			else if (Same(args[0], "cast") || Same(args[0], "c"))
			{
				player->Cast(args);
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