#include <string>
#include <vector>
#include <unordered_map>

#ifndef STRUCT_LITERAL
#define STRUCT_LITERAL

namespace lyrics
{
	using std::u16string;
	using std::vector;
	using std::unordered_map;

	struct Literal
	{
		enum struct Type: unsigned int { NULL_LITERAL, BOOLEAN, INTEGER, REAL, STRING, ARRAY, HASH, DEF, IMAGE, TEXT, SOUND, VIDEO, OBJECT, REFERENCE } type;

		union Value
		{
			bool boolean;
			long long integer;
			double real;
			u16string *string;
			char *function;
			vector<Literal> *array;
			unordered_map<u16string, Literal> *hash;
			char *object;
			u16string *reference;
		} value;
	};
}

#endif
