#include <string>

#include "Location.h"

#ifndef TOKEN
#define TOKEN

namespace lyrics
{
	using std::u16string;

	struct Token
	{
		enum struct Type
		{
			END_OF_FILE,

			CHAR40 = 40, CHAR91 = 91, CHAR123 = 123,

			BREAK = 256, CASE, CLASS, DO, END, ELSE, ELSEIF, FOR, IF, IMPORT, IN, OUT, PACKAGE, PROC, REDO, RETURN, THEN, WHEN, WHILE,
			SHIFT_LEFT, SHIFT_RIGHT, LESS_THAN_OR_EQUAL, GREATER_THAN_OR_EQUAL, EQUAL, NOT_EQUAL, AND, OR,
			IDENTIFIER,
			NIL_LITERAL, BOOLEAN_LITERAL, INTEGER_LITERAL, REAL_LITERAL, STRING_LITERAL
		} type;

		union
		{
			bool boolean;
			long long integer;
			double real;
			u16string *string;
			u16string *identifier;
		} value;

		Location location;
	};
};

#endif