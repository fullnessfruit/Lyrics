#include "Parser.h"

#include "LocalResolver.h"
#include "TypeResolver.h"
#include "TypeTable.h"
#include "DereferenceChecker.h"
#include "TypeChecker.h"

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
			canProgress &= root->Accept( TypeResolver() );
			canProgress &= root->Accept( TypeTable() );
			canProgress &= root->Accept( DereferenceChecker() );
			canProgress &= root->Accept( TypeChecker() );

			return canProgress;
		}
	};
}

#endif
