#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"

using namespace std;

#define BACKSPACE "\033[D\033[K"

#define WHITE_ "\033[1;36m"
#define _WHITE "\033[0m"

// -------------------------------------------------
int main()
{
	char key;
	string player_input;
	vector<string> args;
	args.reserve(10);

	cout << WHITE_ "Welcome to MyZork!\n" _WHITE;
	cout << "----------------\n";

	World my_world;

	args.push_back("look"); // In order to print presentation when starting game

	while(1)
	{
		if(_kbhit() != 0) // if something in buffer
		{
			key = _getch();
			if(key == '\b') // backspace
			{
				if(player_input.length() > 0)
				{
					player_input.pop_back();
					//cout << BACKSPACE;
					cout << '\b';
					cout << " ";
					cout << '\b';
				}
			}
			else if(key != '\r') // return
			{
				/* Adds char to the input */
				player_input += key;
				cout << key;
			}
			else
				/* Identifies each word in input and adds each as an 
				args vector element */
				Tokenize(player_input, args);
		}

		if(args.size() > 0 && Same(args[0], "quit"))
			/* if the command in args is quit, exits the game */
			break;

		/* Ticks the world and analizes args in case there's any command. 
		Only returning false when a command is not recognized. */
		if(my_world.Tick(args) == false)
			cout << "\nSorry, I do not understand your command.\n";

		if(args.size() > 0)
		{
			/* If there's still commands in args vector, deletes them. */
			args.clear();
			player_input = "";
			cout << "> ";
		}
	}

	cout << "\nThanks for playing, Bye!\n";
	return 0;
}