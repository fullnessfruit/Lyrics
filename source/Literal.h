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
		enum struct Type: unsigned int { NULL_LITERAL, BOOLEAN, INTEGER, REAL, STRING, ARRAY, HASH, DEF, IMAGE, TEXT, SOUND, VIDEO, OBJECT, REFERENCE };

		union Value
		{
			bool boolean;
			long long integer;
			double real;
			u16string *string;
			vector<Literal> *array;
			unordered_map<u16string, Literal> *hash;
			char *function;
			char *object;
			u16string *reference;
		};

		Literal( const bool boolean ) : type( Type::BOOLEAN )
		{
			value.boolean = boolean;
		}

		Literal( const long long integer ) : type( Type::INTEGER )
		{
			value.integer = integer;
		}

		Literal( const double real ) : type( Type::REAL )
		{
			value.real = real;
		}

		Literal( u16string * const string ) : type( Type::STRING )
		{
			value.string = string;
		}

		Literal( vector<Literal> * const array ) : type( Type::ARRAY )
		{
			value.array = array;
		}

		Literal( unordered_map<u16string, Literal> * const hash ) : type( Type::HASH )
		{
			value.hash = hash;
		}

		Literal( char * const function ) : type( Type::DEF )
		{
			value.function = function;
		}

		Literal( char * const object ) : type( Type::OBJECT )
		{
			value.object = object;
		}

		Literal( char * const reference ) : type( Type::REFERENCE )
		{
			value.reference = reference;
		}

		const Type type;
		Value value;
	};
}

#endif
