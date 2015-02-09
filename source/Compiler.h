#include <string>

#include "Option.h"

#include "SemanticAnalyzer.h"

#include "FatalErrorCode.h"
#include "ErrorHandler.h"
#include "Logger.h"

#ifndef COMPILER
#define COMPILER

namespace lyrics
{
	using std::string;

	class Compiler
	{
	private:
		Compiler() = delete;

	public:
		static bool Compile( const Option &option )
		{
			if ( option.GetSourceFileName().empty() )
			{
				ErrorHandler::FatalError( FatalErrorCode::NO_INPUT_FILES );
				return false;
			}

			BlockNode *root = nullptr;

			if ( !SemanticAnalyzer().SemanticAnalysis( option.GetSourceFileName(), root ) )
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
