#include "Parser.h"

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
	};
};

#endif