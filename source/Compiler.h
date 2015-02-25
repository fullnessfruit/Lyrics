#include "Option.h"

#include "SemanticAnalyzer.h"

#include "FatalErrorCode.h"
#include "Logger.h"

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
				throw FatalErrorCode::NO_INPUT_FILES;
			}

			BlockNode *root = nullptr;

			if ( !SemanticAnalyzer().SemanticAnalysis( option.SourceCodeFileName(), root ) )
			{
				delete root;

				return false;
			}

			Logger::BuildTerminated();

			return true;
		}
	};
}

#endif
