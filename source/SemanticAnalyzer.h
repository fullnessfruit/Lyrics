#include "Parser.h"

#include "LocalResolver.h"
#include "DereferenceChecker.h"
#include "StaticTypeChecker.h"

#include "Node.h"
#include "Scope.h"

#ifndef SEMANTIC_ANALYZER
#define SEMANTIC_ANALYZER

namespace lyrics
{
	class SemanticAnalyzer
	{
	public:
		bool SemanticAnalysis( const char * const fileName, BlockNode *&root )
		{
			Scope *top;

			if ( !Parser().Parse( fileName, root ) )
			{
				return false;
			}

			bool canProgress = true;

			canProgress &= LocalResolver().Resolve( root, top );
			canProgress &= DereferenceChecker().Check( root );
			canProgress &= StaticTypeChecker().Check( root );

			return canProgress;
		}
	};
}

#endif
