//dictionary class, with one important function that searches for whether a word exists in the official ScrabbleTM dictionary (178691 words)
class Dictionary
{
      public:
             Dictionary();
             bool search(string);
      private:
             vector <string> array;
             int binarySearch(string,int,int);
};

//create an array that contain all the words (in sorted alphabetical order)
Dictionary::Dictionary()
{
      array.resize(178691);
      string str;
      ifstream file;
      int i = 0;
      file.open ("Dictionary.txt");
      do {
         file >> str;
         array[i] = str;
         i++;
      } while (str != "ZZZ");
      file.close();
}

//search for a word in the array
bool Dictionary::search(string str)
{
     int found;
     found = binarySearch(str, 0, 178690);   //min = 0, max = 178690, key = word
     if (found >= 0)
        return true;
     else
         return false;
}

//binary search (recursive)
int Dictionary::binarySearch(string key, int min, int max)
{
    if (max < min)                  //base case - if there is no more splitting (binary divide and conquer) to do (false)
    {
       return -1;
    }
    else
    {
        int mid = (min+max) / 2;    //divide the list in two
        if (array[mid].compare(key) > 0)     //compare the middle item and see if it's bigger or smaller in terms of ASCII, then continue recursively after
        {
            return binarySearch(key,min, mid-1);
        }
        else if (array[mid].compare(key) < 0)
        {
             return binarySearch(key,mid+1, max);
        }
        else
        {
            return mid;                     //true, word found
        }
    }
}
