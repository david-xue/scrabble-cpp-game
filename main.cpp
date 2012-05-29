/*Name: David Xue
Class: ICS4U1-01
Teacher: Mr. Sze
Program Name: ScrabbleTM 
Description: The ScrabbleTM is a traditional word game in which multiple players attempt to
win by scoring points by forming words horizontally or vertically on a 15 by 15 grid board.
It is a game which can incorporate the application of multiple skills of middle school students
including breadth of vocabulary, spelling expertise, math and spatial relation skills. 
The game has three modes, Human vs Human, Human Vs AI, and AI Vs AI.
*/

#include <allegro.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <sstream>

using namespace std;


int strInt(string str)  //function that converts string to integer
{
    int integer;
    stringstream temp(str);
    temp >> integer;
    return (integer);
}

string intStr(int integer) //function that converts integer to string
{
    stringstream temp;
    temp << integer;
    return temp.str();
}

#include "DictionaryClass.h"
#include "Tilepool.h"
#include "ScrabbleBoard.h"
#include "Player.h"
#include "ScrabbleGame.h"


int main() 
{	
    // initialize Allegro (graphics module)
	allegro_init();
	
	install_keyboard();
	

	//set screen color and size
    set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 700, 680, 0, 0);	    	

    ScrabbleGame *game;
	
	// select mode of gameplay
	allegro_message("Press 1 to play Human Vs Human, 2 for Human Vs AI, 3 for AI vs AI \n");
	
	int value = readkey();

	int AIorPlayer = (value & 0xff) - 48;
    
	
	game = new ScrabbleGame(AIorPlayer);
	
	//play game
    game->play();

	readkey();
	
	return 0;
}
END_OF_MAIN();
