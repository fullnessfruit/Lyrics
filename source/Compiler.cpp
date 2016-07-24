#include "Compiler.h"

#include "SemanticAnalyzer.h"

#include "FatalErrorCode.h"
#include "Logger.h"

#include "Utility.h"

namespace lyrics
{
	bool Compiler::Compile(const Option &option) const
	{
		if (option.SourceCodeFileName().empty())
		{
			throw FatalErrorCode::NO_INPUT_FILE;
		}

		if (!SemanticAnalyzer().SemanticAnalysis(option.SourceCodeFileName()))
		{
			return false;
		}

		Logger::CompilationTerminated();

		return true;
	}
}