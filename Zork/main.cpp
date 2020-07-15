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
#define RED_ "\033[1;31m"

bool introduction() {
	cout << "\nYou are dead.\nThe darkness surrounds you, and you feel cold despite not being alive.";
	cout << "\nYou cannot move, cannot speak. You are not breathing. Still, you feel...\nYou feel pain, you feel sorrow, and you feel cold.\n"
		"Why can you feel, you wonder...\n\n";
	system("pause");
	cout << "\nSomething moved out there. You can't see, but you sensed it...\n"
		"Suddenly, a voice! A smooth, whispering, yet clear voice: \n"
		"'How does it feel, being dead?'- asks...\n'Oh! I am sorry, you can't speak.'- the voice direcly speaking to your mind. - 'Let me fix this.'\n"
		"\nYou gasp abruptly. There isn't absolute darkness now, but neither is there an specific source of light.\nAt some point in the space before your eyes"
		" (you can't really tell the distance) there's a reddish humanoid figure sitting on a dark grey throne with yellowish white things stuck in it... Bones?\n\n";
	system("pause");
	cout << "\n'H...H-Hello?' - you manage to say in a thin voice. A shiver goes down your spine and you feel heavy.\n";
	cout << "\n'Let me introduce myself. I am Mephistopheles, the Prince of Hell.' -says the figure. Another shiver... -'You are here because I need something.'\n"
		"...\n\n'I need something that you may be able to acquire. So I will offer you a Deal.'\n\nYou are out of words, you try asking something but your voice does not"
		" come out. The mysterious being who claims to be a prince of Hell proceeds:\n'Three items where stolen from Hell. MY items.'- Mephistopheles rises one hand.\n"
		"'Three powerful magical objects that I created, the so called Trident of Mephistopheles:'\n\n";
	system("pause");
	cout << "\n'The first spike, The Purple Eye.'- an image appears in your mind - 'The second spike, Mephistopheles's Fire. And finally, The Black Third'- more images fill your"
		"mind as it speaks. 'I need them back.'\n\nYou stutter -'I-I don...'\n'The Trident is in the realm of mortals. The deal is the following: I am resurrecting you,"
		" so that you find the spikes and bring them to me.' - The demon being stands up from its throne and steps a little closer. Now you can see it is definitely larger than a human."
		"It wears some kind of brown robes on the low-half of its body, the torso is uncovered and you can see its skin is actually red. The humanoid is considerably musculated and large horns emerge"
		" from its head. You can't see its face. - 'Do you accept the Contract?'\n[Yes/No]\n\n";
	cout << ">";
	
	string answer;
	cin >> answer;
	
	if (Same(answer, "yes"))
		return true;
	else
		return false;
}

bool epilogue() {
	cout << "\nSuddenly, a magic light appears below your feet. A bunch of spirtual hands emerge from it and grab you. You cannot move, you are being swallowed by the purple light.\n";
	system("pause");
	cout << "\nYou find yourself on a large dim lighted room. Four small floating fires illuminate the scene from the corners, and three columns arise towards a ceiling that you can't see. "
		"One column behind you, and the others at each side of the room.\nSome meters away in front of you, a large humanoid figure stands before a throne. The creature has reddish skin, long"
		" horns and something in its back... Folded wings? It's Mephistopheles.\n\n";
	cout << "'Aah... Finally!' - exclaims the demon. - 'You've brought me the Trident. You have done well.'\nYou nod, not knowing what to say.\n'Now, give them to me and we will terminate the Contract'"
		" - Mephistopheles says.\n\nYou make a couple steps forward to proceed with the delivery. You can see its face now. And then, it comes to your mind and you stop.\n'W-wait, what do you mean"
		" terminate? You won't be taking this life away, will you?'- you ask. Mephistopheles stares at you for a moment, and then grins.\n'You noticed. Of course that is what it means, the Contract"
		" specified that I give you a life TO find the Trident. The purpose of that life is to find the spikes and bring them to me, thus when that purpose is gone, so is the life.'"
	cout << "[Deliver/No]";
	
	string answer;
	cin >> answer;
	while (!Same(answer, "Deliver") && !Same(answer, "No")) {
		cout << "\nThat is not an answer.\n[Deliver/No]";
		cin >> answer;
	}

	if (Same(answer, "Deliver"))
		return false;
	else
		return true;
}

// -------------------------------------------------
int main()
{
	char key;
	string player_input;
	vector<string> args;
	args.reserve(10);

	cout << WHITE_ << "Welcome to MephisZorkpheles!\n" << _WHITE;
	cout << "----------------\n";

	World my_world;

	if (introduction()) {
		system("pause");
		cout << "\nYou wake up in a... ";
		args.push_back("look"); // In order to print presentation when starting game

		while (1)
		{
			if (_kbhit() != 0) // if something in buffer
			{
				key = _getch();
				if (key == '\b') // backspace
				{
					if (player_input.length() > 0)
					{
						player_input.pop_back();
						//cout << BACKSPACE;
						cout << '\b';
						cout << " ";
						cout << '\b';
					}
				}
				else if (key != '\r') // return
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

			if (args.size() > 0 && Same(args[0], "quit"))
				/* if the command in args is quit, exits the game */
				break;

			/* Ticks the world and analizes args in case there's any command.
			Only returning false when a command is not recognized. */
			if (my_world.Tick(args) == false)
				cout << "\nSorry, I do not understand your command.\n";

			if (my_world.GameOver())
				break;
			else if (my_world.CheckEndConditions()) {
				if (epilogue())
					my_world.Ending();
				else
					break;
			}

			if (args.size() > 0)
			{
				/* If there's still commands in args vector, deletes them. */
				args.clear();
				player_input = "";
				cout << "\n> ";
			}
		}
	}

	cout << RED_ "\nGame Over\n" _WHITE;
	cout << "\nThanks for playing, Bye!\n";
	system("pause");
	return 0;
}