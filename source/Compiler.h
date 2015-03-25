#include "SemanticAnalyzer.h"

#include "Option.h"
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
				throw FatalErrorCode::NO_INPUT_FILE;
			}

			BlockNode *root = nullptr;

			if ( !SemanticAnalyzer().SemanticAnalysis( option.SourceCodeFileName(), root ) )
			{
				delete root;

				return false;
			}

			Logger::CompilationTerminated();

			delete root;

			return true;
		}
	};
}

#endif
