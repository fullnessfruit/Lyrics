#include <iostream>

#include "Parser.h"
#include "ErrorCode.h"

#ifndef COMPILER
#define COMPILER

namespace lyrics
{
	class Compiler
	{
	public:
		bool Compile( const char * const fileName )
		{
			Parser parser;
			parser.Parse( fileName );

			return true;
		}
	
	private:
		static void Error( const ErrorCode errorCode )
		{
			using std::cout;
			using std::cerr;
			using std::endl;

			constexpr char ERROR[] = "Error";

			switch ( errorCode )
			{
			default:
				cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode ) << endl;
				break;
			}
		}
	};
};

#endif