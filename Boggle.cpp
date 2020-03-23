/* 	Author: Joseph DeMarco
   	Date: 03/22/202
   	
   	This is a program that solves any boggle board given a list of its letters (ordered
   	from top left to bottom right) along with its width and height. 
*/

#include <bits/stdc++.h> 

class c_boggle
{
private:
	// dictionary of all words
	std::unordered_set<std::string> dict;
	// width of the board
	int board_width;
	// height of the board
	int board_height;
	// boggle board represented as a 2d vector
	std::vector< std::vector<char> > boggleBoard;
	

public:
	// checks to make sure the buffer only consists of alphabet characters
	bool checkParameters(const char * board_letters)
	{
		// sinces words are a minimum of 3 characters long in boggle, boards of size
		// <3 are not allowed
		if(board_height <= 0 || board_width <= 0 || board_width * board_height < 3)
		{
			std::cerr << "board width and height must be above zero and be able to form a words of length 3 or more"
				  	  << std::endl;
		}

		// first checks if the size of board_letters is equal to the size of width*height
		int count = 0;
		const char * b = board_letters;
		while(b[count] != '\0') count++;
		if(count != board_height*board_width)
		{
			std::cerr << "board_letter size must match the size of width * height" << std::endl;
			return false;
		}

		// then checks if all board letters are letters
	  	for(int i = 0; i < board_width*board_height; i++){
	    	if(!isalpha(board_letters[i])){
	    		std::cerr << "Board may consist of letters only" << std:: endl;
	      		return false;
	    	}
	  	}

	  	// returns true if all tests were passed
	  	return true;
	}

	// prior to solving a board, configure legal words
	void set_legal_words(
		// the legal words, alphabetically-sorted
		const std::vector<std::string> &all_words)
	{
		copy(all_words.begin(), all_words.end(), inserter(dict, dict.end()));
	}

	bool checkWord(
		// checks id this word is in the provided dictionary or not
		std::string word)
	{
		if(dict.find(word) != dict.end())
			return true;
		return false;
	}

	// recursively finds all words in the provided boggle board
	void findWord(
		// the std::string used to recursively construct potential words
		std::string word, 
		// boggle board
		const char *board_letters,
		// used to store words found
		std::unordered_set<std::string> &wordsFound,
		// a set used to track which indexes of the boggle board have already been used
		std::vector< std::vector<bool> > used, 
		// current x index
		int x, 
		// current y index
		int y)
	{
		// first marks the current index as being used and adds the letter to the word
		used[x][y] = true;
		word.push_back(boggleBoard[x][y]);
		
		// checks to see if the new word exists in the dictionary
		if(checkWord(word)) wordsFound.insert(word);

		// checks adjacent characters for new possible words, ignoring adjacent
		// dice that have already been used
		for(int i = x-1; i < x+2 && i < board_width; i++)
		{
			for(int j = y-1; j < y+2 && y < board_height; j++)
			{
				if(i >= 0 && j >= 0 && used[i][j] != true)
				{
					findWord(word, board_letters, wordsFound, used, i, j);
				}
			}
		}
	}

	// find all words on the specified board
	std::vector<std::string> solve_board(
		// width of the board
		int _board_width,
		// height of the board
		int _board_height,
		// board width*height characters in row major order
		const char *board_letters)
	{
		// initializes private variables
		board_width = _board_width;
		board_height = _board_height;

		// makes sure parameters are valid
		if(!checkParameters(board_letters)) return std::vector<std::string>();

		// a 2d vector used to track which indexes of the boggle board have already been used
		std::vector< std::vector<bool> > used;
		// initializes the used vector to be false in all indexes, and
		// puts the board letters into a 2d vector for easier visualization
		int count = 0;
		for(int i = 0; i < board_width; i++)
		{
			std::vector<bool> u;
			std::vector<char> bb;
			for(int j = 0; j < board_height; j++)
			{
				u.push_back(false);
				bb.push_back(board_letters[count]);
				count++;
			}
			used.push_back(u);
			boggleBoard.push_back(bb);
		}
		
		// used to store words found
		std::unordered_set<std::string> wordsFound;
		// sets the initial std::string
		std::string word = "";
		// starting from each index, recursively find all possible words 
		for(int i = 0; i < board_width; i++)
		{
			for(int j = 0; j < board_height; j++)
			{
				findWord(word, board_letters, wordsFound, used, i, j);
			}
		}
		
		// sorts the words found into a vector to be returned
		std::vector<std::string> retWordsFound(wordsFound.begin(), wordsFound.end());
		sort(retWordsFound.begin(), retWordsFound.end());
		return retWordsFound;
	}
};

/* used for testing
int main()
{
	// used to make dictionary
	std::string word;
	std::vector<std::string> myWords;
	std::ifstream myFile("words.txt");
	if (!myFile)
		std::cout << "Could not open file" << std::endl;
	while (myFile >> word)
		myWords.push_back(word);
	myFile.close();

	// provided  by bungie
	c_boggle my_boggle;
	std::vector<std::string> my_results;
	my_boggle.set_legal_words(myWords);
	my_results= my_boggle.solve_board(3, 3, "yoxrbaved");

	for(std::string s : my_results)
		std::cout << s << std::endl;
	std::cout <<  my_results.size() << std::endl;
	return 0;
}
*/
