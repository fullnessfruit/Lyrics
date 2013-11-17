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
			CHAR40 = 40, CHAR91 = 91, CHAR123 = 123,

			BREAK = 256, CASE, CLASS, DO, END, ELSE, ELSEIF, EXTENDS, FOR, IF, IMPORT, IN, OUT, PRIVATE, PACKAGE, PROC, PUBLIC, REDO, RETURN, THEN, WHEN, WHILE,
			SHIFT_LEFT, SHIFT_RIGHT, LESS_THAN_OR_EQUAL, GREATER_THAN_OR_EQUAL, EQUAL, NOT_EQUAL, AND, OR,
			IDENTIFIER,
			NIL_LITERAL, BOOLEAN_LITERAL, INTEGER_LITERAL, REAL_LITERAL, STRING_LITERAL
		};

		union Value
		{
			bool boolean;
			long long integer;
			double real;
			u16string *string;
			u16string *identifier;
		};

		Token( const Type type, const bool boolean, const Location &location ) : type( type ), location( location )
		{
			value.boolean = boolean;
		}

		Token( const Type type, const long long integer, const Location &location ) : type( type ), location( location )
		{
			value.integer = integer;
		}

		Token( const Type type, const double real, const Location &location ) : type( type ), location( location )
		{
			value.real = real;
		}

		Token( const Type type, u16string * const string, const Location &location ) : type( type ), location( location )
		{
			value.string = string;
		}

		Token( const Type type, const Location &location ) : type( type ), location( location )
		{
		}

		const Type type;
		Value value;
		const Location location;
	};
};

#endif