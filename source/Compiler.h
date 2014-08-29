#include "SemanticAnalyzer.h"

#ifndef COMPILER
#define COMPILER

namespace lyrics
{
	class Compiler
	{
	public:
		bool Compile( const char * const sourceFileName )
		{
			BlockNode *root = nullptr;

			if ( !SemanticAnalyzer().SemanticAnalysis( sourceFileName, root ) )
			{
				delete root;

				return false;
			}

			return true;
		}
	};
}

#endif
