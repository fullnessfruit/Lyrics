#include "Compiler.h"

#include "Tokenizer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"

#include "FatalErrorCode.h"
#include "Logger.h"

#include "Utility.h"

namespace lyrics
{
	void Compiler::Compile(const Option &option, const char32_t * const text, const unsigned int textLength) const
	{
		if (option.SourceCodeFileName().empty())
		{
			throw FatalErrorCode::NO_INPUT_FILE;
		}

		forward_list<Token> *tokenList = nullptr;
		BlockNode *root = nullptr;

		try
		{
			tokenList = Tokenizer().Tokenize(option.SourceCodeFileName(), text, textLength);

			root = Parser().Parse(tokenList);
			Utility::SafeDelete(tokenList);

			root = SemanticAnalyzer().SemanticAnalysis(root);
			Utility::SafeDelete(root);
		}
		catch (const FatalErrorCode fatalErrorCode)
		{
			switch (fatalErrorCode)
			{
			case FatalErrorCode::NOT_ENOUGH_MEMORY:
				Utility::SafeDelete(tokenList);
				Utility::SafeDelete(root);
				break;

			default:
				break;
			}

			throw fatalErrorCode;
		}
		Logger::CompilationTerminated();

		return;
	}
}