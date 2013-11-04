#include <iostream>
#include <list>

#include "TextLoader.h"
#include "Scanner.h"
#include "Parser.h"
#include "ByteCode.h"
#include "ErrorCode.h"
#include "Location.h"

#ifndef COMPILER
#define COMPILER

namespace lyrics
{
	using std::list;

	class Compiler
	{
	public:
		bool Compile( const char * const fileName )
		{
			char16_t *text;
			unsigned int length;

			if ( !TextLoader::LoadText( fileName, text, length ) )
			{
				// TODO:
				return false;
			}

			Scanner scanner;
			unsigned int offset = 0;
			Location location( fileName );
			Token tToken;
			list<Token> token;

			scanner.Scan( text, length, offset, location, tToken );
			while ( tToken.type != Token::Type::END_OF_FILE )
			{
				token.push_back( tToken );
				scanner.Scan( text, length, offset, location, tToken );
			}
			delete [] text;

			Parser parser;
			parser.Parse( token );
			token.clear();

			return true;
		}
	
	private:
		static void Error( const ErrorCode errorCode )
		{
			using std::cout;
			using std::cerr;

			constexpr char ERROR[] = "Error";

			switch ( errorCode )
			{
			default:
				cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode );
				break;
			}
		}
	};
};

#endif