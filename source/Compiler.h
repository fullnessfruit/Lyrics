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
			BlockNode *root = nullptr;

			if ( !SemanticAnalyzer().SemanticAnalysis( fileName, root ) )
			{
				delete root;

				return false;
			}

			return true;
		}
	};
}

#endif
