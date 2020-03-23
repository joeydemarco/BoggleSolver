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


class c_boggle
{
private:
	// dictionary of all words
	vector<string> dict;
	// width of the board
	int board_width;
	// height of the board
	int board_height;
	// boggle board
	

public:
	// checks to make sure the buffer only consists of alphabet characters
	bool checkAlpha(const char * buffer, int len){
		  	for(int i = 0; i < len; i++){
		    	if(!isalpha(buffer[i])){
		      		return false;
		    	}
		  	}
		  	return true;
	}

	// prior to solving a board, configure legal words
	void set_legal_words(
		// the legal words, alphabetically-sorted
		const std::vector<std::string> &all_words)
	{
		dict = all_words;
	}

	bool checkWord(string word)
	{
		for(unsigned int i = 0; i < dict.size(); i++)
		{
			if(word.compare(dict[i]) == 0)
				return true;
		}
		return false;
	}

	void findWord(
		// the string used to recursively construct potential words
		string word, 
		// boggle board
		const char *board_letters,
		// used to store words found
		unordered_set<string> &wordsFound,
		// a set used to track which indexes of the boggle board have already been used
		set<int> used, 
		// current x index
		int x, 
		// current y index
		int y)
	{
		used.insert(x * board_height + y);
		word.push_back(board_letters[x * board_height + y]);
		
		if(checkWord(word))
		{
			wordsFound.insert(word);
		}

		for(int i = x-1; i < x+2; i++)
		{
			for(int j = y-1; j < y+2; j++)
			{
				if(x >= 0 && y >= 0 && x+1 < board_width && y+1 < board_height
					&& used.find(i * board_height + j) == used.end())
				{
					findWord(word, board_letters, wordsFound, used, i, j);
				}
			}
		}

		word.pop_back();
		used.erase(x * board_height + y);
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
		
		// used to store words found
		unordered_set<string> wordsFound;
		// a set used to track which indexes of the boggle board have already been used
		set<int> used;
		// sets the initial string
		string s = "";
		// starting from each index, recursively find all possible words 
		for(int i = 0; i < board_width; i++)
		{
			for(int j = 0; j < board_height; j++)
			{
				findWord(s, board_letters, wordsFound, used, i, j);
			}
		}
		
		for(string s : wordsFound)
		{
			cout << s << endl;
		}
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