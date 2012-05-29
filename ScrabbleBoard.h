const int COLUMNS = 15;
const int ROWS = 15;

//Scrabble Game Board Class
class ScrabbleBoard
{
      public:
             ScrabbleBoard(vector <BITMAP *> picturePointers);
             void displayBoard(int,int, vector < vector <string> >);
             void clear();
             char getPiece(int,int);
             void setPiece(int,int,char);
      private:
             vector < vector <char> >  Board;
             vector <BITMAP *> spritePointers;
};

//constructor
ScrabbleBoard::ScrabbleBoard(vector <BITMAP *> picturePointers)
{
      Board.resize ( ROWS , vector <char> ( COLUMNS , ' ') );  //create board (empty)
      //import pictures
      spritePointers.resize (28);
      for (int i = 0 ; i < 28; i++)
      {
          spritePointers[i] = picturePointers[i];
      }
}

//clear board
void ScrabbleBoard::clear()
{
    for(int i = 0; i < ROWS; i++)
		for(int j = 0; j < COLUMNS; j++)
			Board[i][j] = ' ';
}

//display game board (using graphics Allegro)
void ScrabbleBoard::displayBoard(int cursorX, int cursorY, vector <vector <string> > word)
{
     char c;
     BITMAP * pos;
     int integer;
     bool pass = false;
     for(int i = 0; i < ROWS; i++)
     {
		for(int j = 0; j < COLUMNS; j++)
		{
            pass = false;
            //if the cursor is on this tile, display cursor
            if ((cursorX == j) && (cursorY == i))
            {
                pos = spritePointers[27];
                acquire_screen();
                draw_sprite(screen, pos, (j*40), (i*40));
                release_screen();
                pass = true;
            }
            //if not, then check if the user has inputed temporary tiles and display them
            if (pass == false)
            {
                int T = 0;
                while ((pass == false) && (T < word.size()))
                {
                    if ((strInt(word[T][0]) == i) && (strInt(word[T][1]) == j))
                    {
                         pass = true;
                         c = word[T][2][0];
                         integer = c;
                         integer = integer - 65;
                         pos = spritePointers[integer];
                         acquire_screen();
                         draw_sprite(screen, pos, (j*40), (i*40));
                         release_screen();
                    }
                    T++;
                }
            }
            //if still not true, then check if the tile is occupied, and display that
            if (pass == false)
            {
    			if (Board[i][j] != ' ')
    			{
                    integer = Board[i][j];
                    integer = integer - 65;
                    pos = spritePointers[integer];
                    acquire_screen();
                    draw_sprite(screen, pos, (j*40), (i*40));
                    release_screen();
                }
                //if not occupied, display an empty tile
                else if (Board[i][j] == ' ')
                {
                    pos = spritePointers[26];
                    acquire_screen();
                    draw_sprite(screen,pos,j*40,i*40);
                    release_screen();
                }
            }
        }
     }
}

//return the tile situated at a point on board
char ScrabbleBoard::getPiece(int row, int column)
{
     return Board[row][column];
}

//change a tile on the board
void ScrabbleBoard::setPiece (int row,int column,char newChar)
{
     Board[row][column] = newChar;
}
