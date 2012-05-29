//general Scrabble game class
class ScrabbleGame
{
      public:
             ScrabbleGame(int);
             void play();
             int dealTiles(int);
      private:
             HumanPlayer *player1;
             HumanPlayer *player2;
             ScrabbleBoard *board;
             TilePool *pool;
             Dictionary *dictionary;
             int endGame,activePlayer;
             void alternateActivePlayer();
             vector <BITMAP *> picturePointers;
};

//construct the framework of the game
ScrabbleGame::ScrabbleGame(int AIorPlayer)
{

      //import all pictures (bmp)
      picturePointers.resize(28);
      string alphabet;
      string name;
      const char * c ; 
      alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ12";
      for (int i = 0 ; i < 28; i++)
      {
          name.push_back(alphabet[i]);
          name.append(".bmp");
          c = name.c_str();
          picturePointers[i] = load_bitmap(c, NULL);
          name.clear();
      }
      
      //initialize players (humans or AI)
      player1 = new HumanPlayer(AIorPlayer, AIorPlayer-1,picturePointers);
      player2 = new HumanPlayer(AIorPlayer, AIorPlayer,picturePointers);
      
      //create game board
      board = new ScrabbleBoard(picturePointers);
      
      //create pool of tiles (from which tiles are drawn)
      pool = new TilePool();
      
      //create dictionary class where words are searched for recursively (binary search)
      dictionary = new Dictionary();
      
      //set active player
      activePlayer=1;
      
      endGame = 0;
      //if it's AI Vs AI, the initial game board has the word SCRABBLE on it
      if (AIorPlayer == 3)
      {
         board->setPiece(9,5,'S');
         board->setPiece(9,6,'C');
         board->setPiece(9,7,'R');
         board->setPiece(9,8,'A');
         board->setPiece(9,9,'B');
         board->setPiece(9,10,'B');
         board->setPiece(9,11,'L');
         board->setPiece(9,12,'E');
      }
}

//play game procedure
void ScrabbleGame::play()
{
     //create a blank vector for parameter filling purposes
     vector < vector <string> > temporary;
     //display board
     board->displayBoard(100,100,temporary);
     int i = 0;
     int key;
     bool okay = false;
     //run loop or game as long as the user doesn't press the ESC key
     while ((i < 100) && (okay == false))
     {
         //deal the player tiles as needed
         endGame = dealTiles(activePlayer);
         //display the player's tiles
         player1->displayPool();
         //player 1 (could be human or AI) makes move
         player1->makeMove(board,dictionary);
         //update board
         board->displayBoard(100,100,temporary);
         //alternate players
         alternateActivePlayer();
         endGame = dealTiles(activePlayer);
         player2->displayPool();
         player2->makeMove(board,dictionary);
         key = readkey();
         if ((key >> 8) == KEY_ESC)
         {
              okay = true;
         }
         board->displayBoard(100,100,temporary);
         alternateActivePlayer();
     }
     //destroy pointers to images (prevent memory leak)
     for (int i = 0; i < 27; i++)
     {
         destroy_bitmap(picturePointers[i]);
     }
}

//deal the players tiles (min/max 7) as necessary
int ScrabbleGame::dealTiles(int playerNum)
{
     char temp;
     char returned = ' ';
     int counter;
     counter = 0;
     //check if the pool is out of tiles
     temp = pool->assignTile();
     //deal as long as the player needs tiles or the pool still has tiles remaining
     while ((temp != ' ') && (counter < 7))
     {
           //deal the player tile if the player needs it
           if (playerNum == 1)
           {
              returned = player1->getTile(temp);
           }
           else if (playerNum == 2)
           {
              returned = player2->getTile(temp);
           }
           if (counter < 6)
           {
               if (returned == ' ')
               {
                  temp = pool->assignTile();
                  counter = counter + 1;
               }
               else
               {
                   pool->returnTile(returned);
                   counter = 10;
               }
           }
           else
           {
               counter = counter + 1;
           }
     }
     //return whether the pool is out of tiles
     if ((temp == ' ') && (counter ==10))
     {
        return 1;
     }
     else
     {
         return 0;
     }
}

//alternate active player
void ScrabbleGame::alternateActivePlayer()
{
     if (activePlayer == 1)
        activePlayer = 2;
     else
         activePlayer = 1;
}
