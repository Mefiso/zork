#ifndef __World__
#define __World__

#include <string>
#include <list>
#include <time.h>

#define TICK_FREQUENCY 0.5f

using namespace std;

class Entity;
class Player;
class Room;
class Creature;

class World
{
public :

	World();
	~World();

	bool Tick(vector<string>& args);
	bool ParseCommand(vector<string>& args);
	void GameLoop();
	bool CheckEndConditions();
	bool GameOver();
	void Ending();

private:

	clock_t tick_timer;
	list<Entity*> entities;
	Player* player;
	list<Entity*> objectives;
	bool ended;
	Room* final_room;

};

#endif //__World__