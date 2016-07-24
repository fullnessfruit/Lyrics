#include "Compiler.h"

#include "SemanticAnalyzer.h"

#include "FatalErrorCode.h"
#include "Logger.h"

#include "Utility.h"

namespace lyrics
{
	void Compiler::Compile(const Option &option) const
	{
		if (option.SourceCodeFileName().empty())
		{
			throw FatalErrorCode::NO_INPUT_FILE;
		}

		const BlockNode *root = nullptr;

		root = SemanticAnalyzer().SemanticAnalysis(option.SourceCodeFileName());
		Logger::CompilationTerminated();

		Utility::SafeDelete(root);
		return;
	}
}