#include "SemanticAnalyzer.h"

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

			if ( !SemanticAnalyzer().SemanticAnalysis( fileName, root ) )
			{
				return false;
			}

			return true;
		}
	};
}

#endif
