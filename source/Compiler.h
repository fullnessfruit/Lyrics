#include "Parser.h"
#include "Node.h"

#ifndef COMPILER
#define COMPILER

namespace lyrics
{
	class Compiler
	{
	public:
		bool Compile( const char * const fileName )
		{
			BlockNode *root;

			if ( !Parser().Parse( fileName, root ) )
			{
				return false;
			}

			return true;
		}
	};
};

#endif