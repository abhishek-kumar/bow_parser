#ifndef bow_parser_h
#define bow_parser_h

typedef std::map<std::string, std::pair<int, std::vector<int> > > dictionary;

void processFile(char *);
dictionary parse(std::istream&);
void print(dictionary&);

/*
 * Create a locale that treats all non-alphanumeric
 * characters as whitespace.
 */
struct bow_ctype : std::ctype<char> {
	bow_ctype() : std::ctype<char>(get_table()) {}

	static std::ctype_base::mask const * get_table() {
		static std::vector<std::ctype_base::mask>
		       rc(std::ctype<char>::table_size, std::ctype_base::space);
		std::fill(&rc['0'], &rc['9'], std::ctype_base::digit);
		std::fill(&rc['a'], &rc['z'], std::ctype_base::lower);
		std::fill(&rc['A'], &rc['Z'], std::ctype_base::upper);
		return &rc['0'];
	}
};

/*
 * check if a character is a valid part of a word.
 */
bool isValid(char c) {
	if (c >= '0' && c <= '9')	// this is a digit
		return true;
	if (c >= 'a' && c <= 'z')	// lower case letter
		return true;
	if (c >= 'A' && c <= 'Z')	// upper case letter
		return true;
	if (c == '-' || c == '#') // hyphen and hash are also valid
		return true;
	return false;
}

#endif
