#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fstream>
#include <bits/stdc++.h> 
using namespace std;

class c_boggle_index
{
private:
	// the letter that represents the current node/letter on the boggle board
	char c;
	// uniquely idenifies the node in an unordered list for fast look-up times
	int id;
	// adajcent boggle indexes are numbered from top left to bottom right, as depicted below
	vector<c_boggle_index> adjacent;

public:
	c_boggle_index(
		// the letter represented on the boggle index/"die"
		char _c,
		// used to identify which nodes have been used (letters can repeat)
		int _id)
	{
		c = _c;
		id = _id;
	}

	// returns the letter stored by the node index
	char getLetter() { return c; }
	// returns the id stored by the node index
	char getId() { return id; }

	// sets all adjacent dice to the adjacent vector
	void addAdjacent(
		// index to be added
		c_boggle_index i)
	{
		adjacent.push_back(i);
	}

	void printAdjacent()
	{
		printf("%c: ", c);
		for(unsigned int i = 0; i < adjacent.size(); i++)
		{
			printf("%c, ", adjacent[i].getLetter());
		}
		printf("\n");
	}

	bool findInDict(int low, int mid, int high, string word, std::vector<std::string> &dict)
	{
		if(low == high)
			return false;
		if(word == dict[mid])
			return true;
		else if(word < dict[mid])
			findInDict(low, (low+mid)/2, mid, word, dict);
		else
			findInDict(mid, (mid+high)/2, high, word, dict);
	}

	void findWord(
		// the word containing previous letters used, initialized at ""
		string word, 
		// the set of words already found
		unordered_set<string> &wordsFound,
		// the nodes/dice already used to create the current word (as to not repeat)
		unordered_set<int> diceUsed,
		// the dictionary of all valid words
		std::vector<std::string> &dict)
	{
		printf("%c\n", c);
		// ensures we do no reuse dice
		if(diceUsed.find(id) != diceUsed.end()) return;
		// if the die is unused, try adding it to form a word
		word.push_back(c);
		if(findInDict(0, dict.size()/2, dict.size(), word, dict))
		{
			wordsFound.insert(word);
		}


		//after finding a word or  but possibility of a subsequent words, we continue
		diceUsed.insert(id);
		for(c_boggle_index n : adjacent)
		{
			n.findWord(word, wordsFound, diceUsed, dict);
		}
	}
	


};


class c_boggle
{
private:
	// dictionary of all words
	std::vector<std::string> dict;
	// vector of boggle board
	vector< vector<c_boggle_index> > bl;

public:
	// checks to make sure the buffer only consists of alphabet characters
	bool checkAlpha(const char * buffer, int len){
		//try{
		  	for(int i = 0; i < len; i++){
		    	if(!isalpha(buffer[i])){
		      		return false;
		    	}
		  	}
		  	return true;

	}

	// sets up the board by initializing all indexes with adjacent indexes
	void initializeBoard(
		// width of the board
		int board_width,
		// height of the board
		int board_height,
		// board width*height characters in row major order
		const char *board_letters)
	{
		// sets up all letters as a node in the boggle board (c_boggle_index)
		int count = 0;
		for(int i = 0; i < board_width; i++)
		{
			// makes a board letter saver to assemble the board one row at a time
			vector<c_boggle_index> bls;
			for(int j = 0; j < board_height; j++)
			{
				c_boggle_index w(board_letters[count], count);
				bls.push_back(w);
				count++;
			}
			bl.push_back(bls);
		} 

		// connects the boggle nodes through adajcent nodes, as words can only be made with
		// adajent dice
		for(int i = 0; i < board_width; i++)
		{
			for(int j = 0; j < board_height; j++)
			{
				printf("%c\n", bl[i][j].getLetter());	
				//printf("1\n");			
				if(i-1 >= 0)						bl[i][j].addAdjacent(bl[(i-1)][j]);
				//printf("2\n");	
				if(i+1 < board_width)	 			bl[i][j].addAdjacent(bl[(i+1)][j]);
				//printf("3\n");	
				if(j-1 >= 0)						bl[i][j].addAdjacent(bl[i][(j-1)]);
				//printf("4\n");	
				if(j+1 < board_height)				bl[i][j].addAdjacent(bl[i][(j+1)]);
				//printf("5\n");
				if(i-1 >= 0 && j-1 >= 0)			bl[i][j].addAdjacent(bl[(i-1)][(j-1)]);
				//printf("6\n");
				if(i+1 < board_width && j-1 >= 0)		bl[i][j].addAdjacent(bl[(i+1)][(j-1)]);
				//printf("7\n");
				if(j+1 < board_height && i-1 >= 0)	bl[i][j].addAdjacent(bl[(i-1)][(j+1)]);
				//printf("8\n");
				if(i+1 < board_width && j+1 < board_height)	bl[i][j].addAdjacent(bl[(i+1)][(j+1)]);
				//printf("9\n");	
				
			}
		} 

		for(int i = 0; i < board_width; i++)
		{
			for(int j = 0; j < board_height; j++)
			{
				bl[i][j].printAdjacent();
			}
		}	
	}

	// prior to solving a board, configure legal words
	void set_legal_words(
		// the legal words, alphabetically-sorted
		const std::vector<std::string> &all_words)
	{
		dict = all_words;
	}

	// find all words on the specified board
	std::vector<std::string> solve_board(
		// width of the board
		int board_width,
		// height of the board
		int board_height,
		// board width*height characters in row major order
		const char *board_letters)
	{
		// first checks if boggle board is valid
		if(board_width <= 0 || board_height <= 0 || 
			!checkAlpha(board_letters, board_height * board_width))
		{
			printf("Boggle board may only contain alphabetical characters\n");
			return vector<string>();
		}
		
		// sets up a 2d vector boggle board along with nodes that represent each die on
		// the boggle board.
		initializeBoard(board_height, board_width, board_letters);

		unordered_set<string> wordsFound;
		unordered_set<int> diceUsed;
		for(unsigned int i = 0; i < bl.size(); i++)
		{
			for(unsigned int j = 0; j < bl[i].size(); j++)
			{
				string s = "";
				bl[i][j].findWord(s, wordsFound, diceUsed, dict);
			}
		}
		


		return vector<string>();
	}
};

int main()
{
	// used to make dictionary
	string word;
	vector<string> myWords;
	ifstream myFile("words.txt");
	if (!myFile)
		cout << "Could not open file" << endl;
	while (myFile >> word)
		myWords.push_back(word);
	myFile.close();

	// provided  by bungie
	c_boggle my_boggle;
	std::vector<std::string> my_results;
	my_boggle.set_legal_words(myWords);
	my_results= my_boggle.solve_board(3, 3, "yoxrbaved");
	return 0;
}

// y o x
// r b a
// v e d