#include "Compiler.h"

#include "TextLoader.h"
#include "Tokenizer.h"
#include "Parser.h"
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

		char32_t *text = nullptr;
		forward_list<Token> *tokenList = nullptr;
		BlockNode *root = nullptr;

		try
		{
			unsigned int textLength;

			text = TextLoader().Load(option.SourceCodeFileName(), textLength);

			tokenList = Tokenizer().Tokenize(option.SourceCodeFileName(), text, textLength);
			Utility::SafeArrayDelete(text);

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
				Utility::SafeArrayDelete(text);
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