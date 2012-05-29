//general class for both human and AI players
class HumanPlayer
{
      public:
             HumanPlayer(int,int,vector <BITMAP *>);    //constructor
             void displayPool();        //display player's pool of tiles
             char getTile(char);        //get a tile from main pool
             void makeMove(ScrabbleBoard *board, Dictionary *dictionary);       //make move (player or AI)
      private:
             bool wordInLine(ScrabbleBoard *board, int, int, char, int, Dictionary *dictionary);    //check whether all words formed by letters are actually correct
             char playerPool [7];          //the array of 7 player tiles (could be empty)
             int AIPlayer;                 //0 or 1 is human player, 2 is AI player
             int partialSearch(string,string, int, ScrabbleBoard *board, int, int, Dictionary *dictionary);      //AI function
             vector <BITMAP *> arrayPointers;      //pointers to images
             void humanSetPiece(ScrabbleBoard *board, Dictionary *dictionary);     //human make move function (manages inputs and checks everything)
             int score;                       //score
             int returnScore(char);           //function that get the value of a tile
             BITMAP *blank;                   //blank image pointer
             int mode;                        //1 for H Vs H, 2 for H Vs AI, 3 for AI Vs AI
};

//constructor
HumanPlayer::HumanPlayer(int mode1, int AIorPlayer, vector <BITMAP *> arrayPointers1)
{
     //create player's set of 7 tiles
     for (int i = 0; i < 7; i++)
     {
         playerPool[i] = ' ';
     }
     AIPlayer = AIorPlayer;     //AIPlayer indicates whether this is a player or AI
     mode = mode1;              //mode indicates whether this is human vs human or human vs AI or AI vs AI
     arrayPointers.resize (28); //image pointers
     for (int i = 0 ; i < 28; i++)
     {
       arrayPointers[i] = arrayPointers1[i];
     }
     score = 0;         //player score set to zero at start
     blank = load_bitmap("blank.bmp", NULL);  //blank image to clear screen
}

//check how much points is a tile worth and return the value
int HumanPlayer::returnScore(char char1)
{
    int tempScore = 0;
    if ((char1 == 'E') || (char1 == 'A'))
       tempScore += 1;
    else if ((char1 == 'I') || (char1 == 'O'))
       tempScore += 1;
    else if ((char1 == 'N') || (char1 == 'R'))
       tempScore += 1;
    else if ((char1 == 'T') || (char1 == 'L'))
       tempScore += 1;
    else if ((char1 == 'S') || (char1 == 'U'))
       tempScore += 1;
    else if ((char1 == 'D') || (char1 == 'G'))
       tempScore += 2;
    else if ((char1 == 'B') || (char1 == 'C'))
       tempScore += 3;
    else if ((char1 == 'M') || (char1 == 'P'))
       tempScore += 3;
    else if ((char1 == 'F') || (char1 == 'H'))
       tempScore += 4;
    else if ((char1 == 'V') || (char1 == 'W'))
       tempScore += 4;
    else if (char1 == 'Y')
       tempScore += 4;
    else if (char1 == 'K')
       tempScore += 5;
    else if ((char1 == 'J') || (char1 == 'X'))
       tempScore += 8;
    else if ((char1 == 'Q') || (char1 == 'Z'))
       tempScore += 10;
    return tempScore;
}

//withdraw a tile from the main pool to add to empty spots in the player pool
char HumanPlayer::getTile(char tile)
{
     int i;
     i = 0;
     while (i < 7)
     {
           if (playerPool[i] == ' ')
           {
              playerPool[i] = tile;
              i = 10;
           }
           else
           {
               i++;
           }
     }
     if (i == 10)
        return ' ';
     else
         return tile;
}

//display at the bottom of the screen the player's 7 tiles
void HumanPlayer::displayPool()
{
     char c;
     BITMAP * pos;
     int integer;
     for (int i = 0; i < 7; i++)
     {
         if (playerPool[i] != ' ')
         {
            integer = playerPool[i];
            integer = integer - 65;
            pos = arrayPointers[integer];
            acquire_screen();
            draw_sprite(screen, pos, (i*40), 610);
            release_screen();
         }
         else
         {
            pos = arrayPointers[26];
            acquire_screen();
            draw_sprite(screen ,pos, (i*40), 610);
            release_screen();
         }
     }
}

//human inputs and makes a move (or forfeit)
void HumanPlayer::humanSetPiece(ScrabbleBoard *board, Dictionary *dictionary)
{
     //stores a version of the player's current pool in case the user fails to make a valid input
     char storagePool[7];
     for (int c = 0; c < 7; c++)
         storagePool[c] = playerPool[c];
     int cursorX, cursorY;              //user's cursor position (0,0) is top left
     int val;                           //value of the user's input key (allegro specific)
     int b;                             //counter for a loop
     string tempStr;                    //temporary string
     bool exitInnerLoop;                //conditions
     bool found;
     int verHor;                        //whether the word is vertical or horizontal: 1 for vertical, 2 for horizontal
     char char1;
     int horizontal = 2;                //also vertical or horizontal, but for input purposes
     vector < vector <string> >  word;  //most important vector, stores the (x,y) coordinates and char of tiles that user inputs
     vector <string> temp;
     temp.resize(3);
     bool exit = false;
     bool okay = false;
     cursorX = 0;
     cursorY = 0;
     //run loop as long the user doesn't finish his move or forfeit
     do {
         board->displayBoard(cursorX,cursorY, word);     //display current board status (with cursor and newly inputed tiles)
         val = readkey();                     //get keyboard key stroke
         if ((val >> 8) == KEY_UP)            //directional arrow would change the (x,y) coordinates of cursor accordingly
         {
            if (cursorY != 0)
               cursorY--;
            else
               cursorY = 14;
         }
         else if ((val >> 8) == KEY_DOWN)
         {
            if (cursorY != 14)
               cursorY++;
            else
               cursorY = 0;
         }
         else if ((val >> 8) == KEY_LEFT)
         {
            if (cursorX != 0)
               cursorX--;
            else
               cursorX = 14;
         }
         else if ((val >> 8) == KEY_RIGHT)
         {
            if (cursorX != 14)
               cursorX++;
            else
               cursorX = 0;
         }
         else if ((val >> 8) == KEY_ENTER)    //the enter key would finish user input and exit loop as long as the move is valid
         {
            if (word.size() >= 1)             //if the user attempts a move
               exit = true;
            else
                exit = false;                 //if the user forfeit
         }
         else if (((val & 0xff) >= 'A') && ((val & 0xff) <= 'Z'))  //if the user enters a letter key, then add the tile to the board as long as it is in the same row/column as previously entered tiles and that the board tile is blank
         {
             if (board->getPiece(cursorY,cursorX) == ' ')   //if the board tile is empty, then continue, otherwise the move is invalid
             {
                 char1 = (val & 0xff);
                 b = 0;
                 exitInnerLoop = true;
                 //check if the tile that the user chooses is actually in his pool of 7 tiles
                 do
                 {
                      if (playerPool[b] == char1)
                      {
                         exitInnerLoop = false;
                         playerPool[b] = ' ';
                      }
                      else
                      {
                          exitInnerLoop = true;
                      }
                      b++;
                 } while ((b < 7) && (exitInnerLoop == true));
                 if (exitInnerLoop == false)
                 {
                     // if this is the first tile the user inputs, then the move is valid (since there are no restrictions on orientation)
                     if (word.size() == 0)
                     {
                         temp[0] = intStr(cursorY);
                         temp[1] = intStr(cursorX);
                         tempStr.clear();
                         tempStr.push_back(char1);
                         temp[2] = tempStr;
                         tempStr.clear();
                         word.push_back(temp);
                     }
                     else
                     {
                          okay = false;
                          //otherwise, it is a must to check whether the letter is in the same row/column as all previously inputed tiles
                          for (int i = 0; i < word.size(); i++)
                          {
                              if ((cursorY == strInt(word[i][0])) && (cursorX == strInt(word[i][1])))     //replace letters on the same tile
                              {
                                   tempStr.push_back(char1);
                                   word[i][2] = tempStr;
                                   tempStr.clear();
                                   okay = false;
                              }
                              else if (cursorY == strInt(word[i][0]))
                              {
                                   if (horizontal == 1)
                                        okay = true;
                                   else if (horizontal == 2)
                                   {
                                        okay = true;
                                        horizontal = 1;
                                   }
                                   else
                                       okay = false;
                              }
                              else if (cursorX == strInt(word[i][1]))
                              {
                                   if (horizontal == 0)
                                        okay = true;
                                   else if (horizontal == 2)
                                   {
                                        okay = true;
                                        horizontal = 0;
                                   }
                                   else
                                       okay = false;
                              }
                              else
                              {
                                  okay = false;
                              }
                          }
                          if (okay == true)
                          {
                              temp[0] = intStr(cursorY);
                              temp[1] = intStr(cursorX);
                              tempStr.clear();
                              tempStr.push_back(char1);
                              temp[2] = tempStr;
                              tempStr.clear();
                              word.push_back(temp);
                          }
                     }
                 }
             }
         }
         //if the user hits the ENTER key to make a move
         if (exit == true)
         {
            //first of all, check if the user is making a vertical or horizontal word
            bool exitLoop = false;
            string word1;
            int row, column;
            int startRow, startColumn;
            int a;
            row = strInt(word[0][0]);
            column = strInt(word[0][1]);
            word1.append(word[0][2]);
            //going up from the first tile in user input array
            while (exitLoop == false)
            {
                 row--;
                 if (row < 0)
                 {
                     startRow = 0;
                     startColumn = strInt(word[0][1]);
                     exitLoop = true;
                 }
                 else
                 {
                     a = word.size() - 1;
                     while (a > 0)
                     {
                          if (strInt(word[a][0]) == row)
                          {
                             word1.insert(0, word[a][2]);
                             a = -1;
                          }
                          else
                          {
                              a--;
                          }
                     }
                     if (a == 0)
                     {
                          if (board->getPiece(row, column) != ' ')
                          {
                             word1.insert(0, " ");
                             word1[0] = board->getPiece(row, column);
                          }
                          else
                          {
                              startRow = row + 1;
                              startColumn = strInt(word[0][1]);
                              exitLoop = true;
                          }
                     }
                 }
            }
            //going down
            exitLoop = false;
            row = strInt(word[0][0]);
            column = strInt(word[0][1]);
            while (exitLoop == false)
            {
                 row++;
                 if (row > 14)
                 {
                      exitLoop = true;
                 }
                 else
                 {
                     a = word.size() - 1;
                     while (a > 0)
                     {
                          if (strInt(word[a][0]) == row)
                          {
                             word1.append(word[a][2]);
                             a = -1;
                          }
                          else
                          {
                              a--;
                          }
                     }
                     if (a == 0)
                     {
                          if (board->getPiece(row, column) != ' ')
                          {
                             word1.push_back(board->getPiece(row, column));
                          }
                          else
                          {
                              exitLoop = true;
                          }
                     }
                 }
            }
            //if the word is not vertical, then check if it's horizontal
            if (word1.size() <= 1)
            {
                 word1.clear();
                 word1.append(word[0][2]);
                 exitLoop = false;
                 row = strInt(word[0][0]);
                 column = strInt(word[0][1]);
                 while (exitLoop == false)
                 {
                     column--;
                     if (column < 0)
                     {
                          startColumn = 0;
                          startRow = strInt(word[0][0]);
                          exitLoop = true;
                     }
                     else
                     {
                         a = word.size() - 1;
                         while (a > 0)
                         {
                              if (strInt(word[a][1]) == column)
                              {
                                 word1.insert(0, word[a][2]);
                                 a = -1;
                              }
                              else
                              {
                                  a--;
                              }
                         }
                         if (a == 0)
                         {
                              if (board->getPiece(row, column) != ' ')
                              {
                                 word1.insert(0, " ");
                                 word1[0] = board->getPiece(row, column);
                              }
                              else
                              {
                                  startColumn = column + 1;
                                  startRow = strInt(word[0][0]);
                                  exitLoop = true;
                              }
                         }
                     }
                 }
                 exitLoop = false;
                 row = strInt(word[0][0]);
                 column = strInt(word[0][1]);
                 while (exitLoop == false)
                 {
                     column++;
                     if (column > 14)
                     {
                          exitLoop = true;
                     }
                     else
                     {
                         a = word.size() - 1;
                         while (a > 0)
                         {
                              if (strInt(word[a][1]) == column)
                              {
                                 word1.append(word[a][2]);
                                 a = -1;
                              }
                              else
                              {
                                  a--;
                              }
                         }
                         if (a == 0)
                         {
                              if (board->getPiece(row, column) != ' ')
                              {
                                 word1.push_back(board->getPiece(row, column));
                              }
                              else
                              {
                                  exitLoop = true;
                              }
                         }
                     }
                 }
                 verHor = 2;
            }
            else
            {
                verHor = 1;
            }
            //if the user only enters one tile and that there's no nearby tiles, then it's an invalid move (no word formed)
            if (word1.size() <= 1)
            {
               exit = false;
               //reset player pool
               for (int x = 0; x < 7; x++)
               {
                   playerPool[x] = storagePool[x];
               }
               word1.clear();
               word.clear();
               horizontal = 2;
            }
            else
            {
                //search for word in the dictionary
                found = dictionary->search(word1);
                if (found == true)                //if it's a word, then look at all the possible words that can be formed perpendicular to the word's orientation
                {
                    bool okay1;
                    for (int i = 0; i < word1.length(); i++)
                    {
                        if (verHor == 1)
                        {
                            okay1 = wordInLine(board, startRow+i, startColumn, word1[i], verHor, dictionary);
                        }
                        else
                        {
                            okay1 = wordInLine(board, startRow, startColumn+i, word1[i], verHor, dictionary);
                        }
                        if (okay1 == false)
                        {
                            found = false;
                        }
                    }
                }
                //if all possible letter combinations are actually words, then change the game board accordingly
                if (found == true)
                {
                    exit = true;
                    for (int counter = 0; counter < word1.length(); counter++)
                    {
                        if (verHor == 1)
                        {
                           board->setPiece(startRow+counter,startColumn,word1[counter]);
                        }
                        else
                        {
                           board->setPiece(startRow,startColumn+counter,word1[counter]);
                        }
                    }
                    //add the values of all the tiles used by the player to the score, the position of text depends on what game mode it is
                    for (int counter = 0; counter < word.size(); counter++)
                    {
                        score += returnScore(word[counter][2][0]);
                    }
                    //update score text
                    FONT *font1;
                    font1 = load_font("8x14t.pcx", NULL,NULL);
                    acquire_screen();
                    if (mode == 2)
                    {
                        draw_sprite(screen,blank,300,610);
                        textprintf_ex(screen, font1, 310, 630, makecol(255, 100, 200), -1, "Player Score: %i", score);
                    }
                    else
                    {
                        if (AIPlayer == 0)
                        {
                            draw_sprite(screen,blank,300,610);
                            textprintf_ex(screen, font1, 310, 630, makecol(255, 100, 200), -1, "Player 1 Score: %i", score);
                        }
                        else
                        {
                            draw_sprite(screen,blank,520,610);
                            textprintf_ex(screen, font1, 530, 630, makecol(255, 100, 200), -1, "Player 2 Score: %i", score);
                        }
                    }
                    release_screen();
                    destroy_font(font1);
                }
                else
                {
                    // if there are invalid words, then reset player pool and game board to previous state
                    exit = false;
                    for (int x = 0; x < 7; x++)
                    {
                       playerPool[x] = storagePool[x];
                    }
                    word1.clear();
                    word.clear();
                    horizontal = 2;
                }
            }
         }
         //if the user hits enter when there is no tiles inputed yet, then it's a forfeit
         else if ((word.size() == 0) && ((val >> 8) == KEY_ENTER))
         {
              exit = true;
         }
     }
     while (exit == false);
}   


void HumanPlayer::makeMove(ScrabbleBoard *board, Dictionary *dictionary)
{
     if (AIPlayer < 2)                   //human player
     {
         humanSetPiece(board, dictionary);
     }
     else                     //AI player
     {
         string line;
         string str2;
         int pos;
         int turnSuccess;
         int row = 0;
         bool exit = false;
         bool tempExit;
         int temp = 0;
         //find first tile on each row of game board, and check what vertical words can be formed on this tile (or tiles)
         while ((row < 15) && (exit == false))
         {
             line.clear();
             str2.clear();
             //make a string of the row of game board
             for (int column = 0; column < 15; column++)
             {
                 line.push_back(board->getPiece(row,column));
             }
             tempExit = false;
             temp = 0;
             //find index of first instance of tile
             while ((temp < 15) && (tempExit == false))
             {
                 if (line[temp] != ' ')
                 {
                    pos = temp;
                    tempExit = true;
                 }
                 temp++;
             }
             //if there is indeed a tile or more, make the tile or multiple tiles a string
             if (tempExit == true)
             {
                 tempExit = false;
                 temp = 0;
                 while ((temp < 15) && (tempExit == false))
                 {
                     if ((pos+temp) > 14)
                     {
                        tempExit = true;
                     }
                     else if (line[pos+temp] != ' ')
                     {
                        str2.push_back(line[pos+temp]);
                     }
                     else
                     {
                        tempExit = true;
                     }
                     temp++;
                 }
                 turnSuccess = partialSearch(line, str2, pos, board, row, 2, dictionary);  //search for possible words to make connecting to this string, obeying game rules
                 if (turnSuccess == 1)
                 {
                     exit = true;
                 }
             }
             row++;
         }
         int column = 0;
         //same thing as above while-loop, but for columns with horizontal words
         while ((column < 15)&&(exit == false))
         {
             line.clear();
             str2.clear();
             for (int r = 0; r < 15; r++)
             {
                 line.push_back(board->getPiece(r,column));
             }
             tempExit = false;
             temp = 0;
             while ((temp < 15) && (tempExit == false))
             {
                 if (line[temp] != ' ')
                 {
                    pos = temp;
                    tempExit = true;
                 }
                 temp++;
             }
             if (tempExit == true)
             {
                 tempExit = false;
                 temp = 0;
                 while ((temp < 15) && (tempExit == false))
                 {
                     if ((pos+temp) > 14)
                     {
                        tempExit = true;
                     }
                     else if (line[pos+temp] != ' ')
                     {
                        str2.push_back(line[pos+temp]);
                     }
                     else
                     {
                        tempExit = true;
                     }
                     temp++;
                 }
                 turnSuccess = partialSearch(line, str2, pos, board, column, 1, dictionary);
                 if (turnSuccess == 1)
                 {
                     exit = true;
                 }
             }
             column++;
         }
     }
}

//search for possible words for AI, and making the move if possible
int HumanPlayer::partialSearch(string line, string str2, int position, ScrabbleBoard *board, int row, int verHor, Dictionary *dictionary1)
{
     char storagePool[7];
     for (int c = 0; c < 7; c++)
         storagePool[c] = playerPool[c];
     string str;
     ifstream dictionary;
     size_t found;
     int pos;
     bool bigLoopExit = false;
     bool pass;
     bool loopExit;
     int a , i;
     //open dictionary file
     dictionary.open ("Dictionary.txt");
     //run a partial search of all the items in the file to determine which words fit the criteria
     do {
         dictionary >> str;
         if (str.length() > str2.length())   //first of all, there is no point in searching if the word in the dictionary is smaller than the searched word
         {
             found = str.find(str2);
             if (found != string::npos)      //if the partial string is found in the dictionary word
             {
                 pos = position - int(found);     //calculate whether the word would fit inside the row/column on the game board (ie, it doesn't go over the boundary, it doesn't override other existing tiles, etc)
                 if ((pos + str.length()) <= 15)
                 {
                     i = 0;
                     loopExit = false;
                     do
                     {
                         if (line[i+pos] == ' ')
                         {
                              a = 0;
                              pass = false;
                              do   //check if all the tiles used are actually in the AI player's pool
                              {
                                  if (playerPool[a] == str[i])
                                  {
                                     pass = true;
                                     playerPool[a] = ' ';
                                  }
                                  else
                                  {
                                     pass = false;
                                  }
                                  a++;
                              } while ((a < 7) && (pass == false));
                              if (pass == false)
                              {
                                  loopExit = true;
                              }
                              else
                              {
                                  loopExit = false;
                              }
                         }
                         else if (line[i+pos] == str[i])
                         {
                              loopExit = false;
                         }
                         else
                         {
                              loopExit = true;
                         }
                         i++;
                     } while ((loopExit == false) && (i < str.length()));
                     if (loopExit == false)
                     {
                         if (pos != 0)
                         {
                              if (line[pos-1] != ' ')
                              {
                                  loopExit = true;
                              }
                         }
                         if ((pos+str.length()) < 15)
                         {
                              if (line[pos+str.length()] != ' ')
                              {
                                  loopExit = true;
                              }
                         }
                     }
                     //if the word is horizontal, check if all the vertical combinations of characters in the word are also words, and vice versa
                     if (loopExit == false)
                     {
                         bool okay;
                         for (int len = 0; len < str.length(); len++)
                         {
                             if (verHor == 2)
                             {
                                okay = wordInLine(board, row, pos+len, str[len], verHor, dictionary1);
                             }
                             else
                             {
                                 okay = wordInLine(board, pos+len, row, str[len], verHor, dictionary1);
                             }
                             if (okay == false)
                             {
                                   loopExit = true;
                             }
                         }
                     }
                     //if every criterion is met, then the game board is updated accordingly, so is the score
                     if (loopExit == false)
                     {
                         for (int b = 0; b < str.length(); b++)
                         {
                             if (verHor == 2)
                             {
                                board->setPiece(row, b+pos, str[b]);
                             }
                             else
                             {
                                 board->setPiece(b+pos, row, str[b]);
                             }
                         }
                         for (int place = 0; place < 7; place++)
                         {
                             if (playerPool[place] == ' ')
                             {
                                score += returnScore(storagePool[place]);
                             }
                         }
                         FONT *font1;
                         font1 = load_font("8x14t.pcx", NULL,NULL);
                         acquire_screen();
                         if (mode == 2)
                         {
                             draw_sprite(screen,blank,520,610);
                             textprintf_ex(screen, font1, 530, 630, makecol(255, 100, 200), -1, "Computer Score: %i", score);
                         }
                         else
                         {
                             if (AIPlayer == 2)
                             {
                                 draw_sprite(screen,blank,300,610);
                                 textprintf_ex(screen, font1, 310, 630, makecol(255, 100, 200), -1, "Computer 1 Score: %i", score);
                             }
                             else
                             {
                                 draw_sprite(screen,blank,520,610);
                                 textprintf_ex(screen, font1, 530, 630, makecol(255, 100, 200), -1, "Computer 2 Score: %i", score);
                             }
                         }
                         release_screen();
                         destroy_font(font1);
                         bigLoopExit = true; // exit loop at first possible word found
                     }
                     else            //otherwise, reset player pool
                     {
                         for (int x = 0; x < 7; x++)
                         {
                             playerPool[x] = storagePool[x];
                         }
                         bigLoopExit = false;
                     }
                 }
             }
         }
     } while ((bigLoopExit == false) && (str != "ZZZ"));
     dictionary.close();
     //return whether the search was successful
     if (bigLoopExit == true)
     {
        return 1;
     }
     else
     {
         return 2;
     }
}

//search for all word possibilities along the word's axis (vertical or horizontal)
bool HumanPlayer::wordInLine(ScrabbleBoard *board, int row, int column, char char1, int vecticalHorizontal, Dictionary *dictionary)
{
     bool okay;
     string str;
     str.push_back(char1);
     int newInt = row;
     if (vecticalHorizontal == 2)            //vertical
     {
        newInt = row - 1;
        if (newInt >= 0)
        {
            while ((newInt >= 0) && (board->getPiece(newInt,column) != ' '))
            {
                  str.insert(str.begin(),board->getPiece(newInt,column));
                  newInt = newInt - 1;
            }
        }
        newInt = row + 1;
        if (newInt <= 14)
        {
            while ((newInt <= 14) && (board->getPiece(newInt,column) != ' '))
            {
                  str.push_back(board->getPiece(newInt,column));
                  newInt++;
            }
        }
     }
     else   //horizontal
     {
        newInt = column - 1;
        if (newInt >= 0)
        {
            while ((newInt >= 0) && (board->getPiece(row,newInt) != ' '))
            {
                  str.insert(str.begin(),board->getPiece(row,newInt));
                  newInt = newInt - 1;
            }
        }
        newInt = column + 1;
        if (newInt <= 14)
        {
            while ((newInt <= 14) && (board->getPiece(row,newInt) != ' '))
            {
                  str.push_back(board->getPiece(row,newInt));
                  newInt++;
            }
        }
     }
     //search in dictionary
     if (str.length() > 1)
     {
         bool found1;
         found1 = dictionary->search(str);
         if (found1 == true)
         {
               okay = true;
         }
         else
         {
             okay = false;
         }
     }
     else
     {
         okay = true;
     }
     return okay;
}
