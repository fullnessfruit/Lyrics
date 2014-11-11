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

			BREAK = 256, CASE, CLASS, DO, END, ELSE, ELSEIF, FOR, FOREACH, IF, IMPORT, IN, INCLUDE, NEXT, OUT, PACKAGE, PRIVATE, PUBLIC, RETURN, ROUTINE, THEN, THIS, WHEN, WHILE,
			SHIFT_LEFT, SHIFT_RIGHT, LESS_THAN_OR_EQUAL, GREATER_THAN_OR_EQUAL, EQUAL, NOT_EQUAL, AND, OR,
			IDENTIFIER,
			NULL_LITERAL, BOOLEAN_LITERAL, INTEGER_LITERAL, REAL_LITERAL, STRING_LITERAL
		};

		union Value
		{
			bool boolean;
			long long integer;
			double real;
			u16string *string;
			u16string *identifier;
		};

		Token( const Type type, const Location &location ) : type( type ), location( location )
		{
		}

		Token( const bool boolean, const Location &location ) : type( Type::BOOLEAN_LITERAL ), location( location )
		{
			value.boolean = boolean;
		}

		Token( const long long integer, const Location &location ) : type( Type::INTEGER_LITERAL ), location( location )
		{
			value.integer = integer;
		}

		Token( const double real, const Location &location ) : type( Type::REAL_LITERAL ), location( location )
		{
			value.real = real;
		}

		Token( u16string * const string, const Location &location ) : type( Type::STRING_LITERAL ), location( location )
		{
			value.string = string;
		}

		Token( const Type type, u16string * const identifier, const Location &location ) : type( type ), location( location )
		{
			value.identifier = identifier;
		}

		const Type type;
		Value value;
		const Location location;
	};
}

#endif
