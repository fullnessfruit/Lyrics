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

		BlockNode *root = nullptr;

		if (!SemanticAnalyzer().SemanticAnalysis(option.SourceCodeFileName(), root))
		{
			Utility::SafeDelete(root);

			return false;
		}

		Logger::CompilationTerminated();

		Utility::SafeDelete(root);

		return true;
	}
}