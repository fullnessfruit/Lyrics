#include "Parser.h"
#include "Node.h"

#ifndef SEMANTIC_ANALYZER
#define SEMANTIC_ANALYZER

namespace lyrics
{
	class SemanticAnalyzer
	{
	public:
		bool SemanticAnalysis( const char * const fileName, BlockNode *&root )
		{
			if ( !Parser().Parse( fileName, root ) )
			{
				return false;
			}

			return true;
		}
	};
}

#endif
