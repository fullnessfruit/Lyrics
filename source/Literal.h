#include <string>
#include <vector>
#include <unordered_map>

#ifndef LITERAL
#define LITERAL

namespace lyrics
{
	using std::u16string;
	using std::vector;
	using std::unordered_map;

	struct Literal
	{
		enum struct Type : unsigned int { NIL, BOOLEAN, INTEGER, REAL, STRING, PROC, ARRAY, HASH, IMAGE, TEXT, SOUND, VIDEO, OBJECT } type;

		union
		{
			bool boolean;
			long long integer;
			double real;
			u16string *string;
			char *proc;
			vector<Literal> *array;
			unordered_map<u16string, Literal> *hash;
			char *object;
		} value;
	};
};

#endif