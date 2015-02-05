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
	private:
		SemanticAnalyzer() = delete;

	public:
		static bool SemanticAnalysis( const string fileName, BlockNode *&root )
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
