#include <string>

#include "Node.h"
#include "Scope.h"
#include "Parser.h"
#include "LocalResolver.h"
#include "DereferenceChecker.h"
#include "StaticTypeChecker.h"

#ifndef SEMANTIC_ANALYZER
#define SEMANTIC_ANALYZER

namespace lyrics
{
	using std::string;

	class SemanticAnalyzer
	{
	public:
		bool SemanticAnalysis( const string &fileName, BlockNode *&root )
		{
			Scope *top = nullptr;

			if ( !Parser().Parse( fileName, root ) )
			{
				return false;
			}

			bool canProgress = true;

			try
			{
				canProgress &= LocalResolver().Resolve( root, top );
			}
			catch ( const bad_alloc &e )
			{
				Utility::SafeDelete( root );
				Utility::SafeDelete( top );
				throw FatalErrorCode::NOT_ENOUGH_MEMORY;
			}

			canProgress &= DereferenceChecker().Check( root );

			canProgress &= StaticTypeChecker().Check( root );

			Utility::SafeDelete( top );

			return canProgress;
		}
	};
}

#endif
