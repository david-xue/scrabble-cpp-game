//main pool of excess tiles
class TilePool
{
      public:
             TilePool();
             char assignTile();
             void shuffle();
             void returnTile(char);
      private:
             vector <char> tilePool;
};

//constructor
TilePool::TilePool()
{
      ifstream setupFile;
      //setup the pool according to the letter distribution shown by the text file
      setupFile.open ("poolComposition.txt");
      char tile;
      int num;
      for (int i = 0; i < 26; i++)
      {
          setupFile>>tile;
          setupFile>>num;
          for (int a = 0; a < num; a++)
          {
              tilePool.push_back(tile);
          }
      }
      //shuffle the pool (it's alphabetically sorted at the beginning)
      shuffle();
}

//the shuffle algorithm
void TilePool::shuffle()
{
     srand(time(0));
     int i, j;
     char stored;
     i = tilePool.size();
     while (i > 1)
     {
           i = i - 1;
           j = (rand() % (i+1));
           stored = tilePool[j];
           tilePool[j] = tilePool[i];
           tilePool[i] = stored;
     }
}

//return a tile to this pool
void TilePool::returnTile(char char1)
{
     tilePool.push_back(char1);
}

//take a tile from the pool to give to a player
char TilePool::assignTile()
{
     char tile;
     if (tilePool.size() > 0)
     {
         tile = tilePool.back();
         tilePool.pop_back();
         return tile;
     }
     else
     {
         return ' ';
     }
}
