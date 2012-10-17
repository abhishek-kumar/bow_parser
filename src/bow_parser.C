#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include "bow_parser.h"

using namespace std;

/*
 * Main entry point of application.
 * How to run: bow_parser <filename>
 */
int main(int argc, char **argv) {
	if (argc < 2) {
		cerr << "Please provide a file to process. Syntax: " << endl
		     << "bow_parser <filename>" << endl;
		return 1;
	}
	processFile(argv[1]);
}

/*
 * This function parses a given file,
 * converts it to bag-of-words representation
 * and prints this on to standard output
 */
void processFile(char * filename) {
	dictionary dict;
	ifstream inputFile;
	inputFile.open(filename, ifstream::in);
	if (inputFile.is_open()){
		if (inputFile.good() ){
			dict = parse(inputFile);
		}
		inputFile.close();
	} else {
		cerr << "Error opening file '" << filename 
		     << ". Aborting." << endl;
	}
	print(dict);
}

/*
 * Given a word, add it to the dictionary, increment its count
 * and add the line number on which it was found.
 */
void processWord(string& word, dictionary& dict, int lineNumber) {
	if (word.size() > 0) {
		dictionary::iterator itr = dict.find(word);
		if (itr == dict.end()) {
			dict[word] = pair<int, vector<int> >(1, vector<int>(1, lineNumber));
		} else {
			pair<int, vector<int> > value = itr->second;
			++(value.first);
			value.second.push_back(lineNumber);

			// Update the dictionary reference
			dict[word] = value;
		}
	}
}

/*
 * Parse a given stream character by character, and add
 * newly found words to a running dictionary.
 */
dictionary parse(istream& stream) {
	// A stream to store the currently read word
	std::stringstream wordStream;

	// Characters read from the file, and its line number.
	char c;
	int lineNumber = 1;

	// A map that stores words, their frequencies and line numbers.
	// (key, value) = (word, <freq, list of line numbers>)
	dictionary dict;

	// Read the stream, one character at a time.
	while (stream.good()) {
		stream.get(c);
		if (isValid(c)) {
			wordStream << c;
		} else {
			string word = wordStream.str();

			// If we found a new word, process it.
			processWord(word, dict, lineNumber);

			// Clear the word stringstream for the next word.
			wordStream.str("");

			// Check if we're on a new line.
			if (c == '\n')
				++lineNumber;
		}
	}

	// Process the last word read.
	string word = wordStream.str();
	processWord(word, dict, lineNumber);

	// Return by copying is better than creating an object on heap 
	// and returning it byref, to prevent memory leaks.
	return dict; 
}

/*
 * Given a dictionary of words, their frequency and line number of occurence,
 * print it in alphabetical order, in the format:
 * word    {3:1,2,3} 
 */
void print(dictionary& dict) {
	// Sort all the words in the dictionary
	vector<string> words;
	for(dictionary::iterator itr = dict.begin(); itr != dict.end(); ++itr)
		words.push_back(itr->first);
	sort(words.begin(), words.end());

	// Go through the sorted list of words and print out the concordance.
	for (vector<string>::iterator i = words.begin(); i != words.end(); ++i) {
		cout << *i << "\t{";
		pair<int, vector<int> > value = dict[*i];
		cout << value.first << ":";
		string comma = "";
		for (vector<int>::iterator j = value.second.begin(); j != value.second.end(); ++j) {
			cout << comma << *j;
			comma = ",";
		}
		cout << "}" << endl;
	}
}
