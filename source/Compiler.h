#include "SemanticAnalyzer.h"

#include "Option.h"
#include "FatalErrorCode.h"
#include "Logger.h"

#include "Utility.h"

#ifndef COMPILER
#define COMPILER

namespace lyrics
{
	class Compiler
	{
	public:
		bool Compile( const Option &option )
		{
			if ( option.SourceCodeFileName().empty() )
			{
				throw FatalErrorCode::NO_INPUT_FILE;
			}

			BlockNode *root = nullptr;

			if ( !SemanticAnalyzer().SemanticAnalysis( option.SourceCodeFileName(), root ) )
			{
				Utility::SafeDelete( root );

				return false;
			}

			Logger::CompilationTerminated();

			Utility::SafeDelete( root );

			return true;
		}
	};
}

#endif
