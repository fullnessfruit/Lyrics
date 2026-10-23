#include "Compiler.h"

#include <memory>

#include "TextLoader.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"

#include "FatalErrorCode.h"
#include "Logger.h"

namespace lyrics
{
	void Compiler::Compile(const Option &option) const
	{
		if (option.SourceCodeFileName().empty())
		{
			throw FatalErrorCode::NO_INPUT_FILE;
		}

		{
			unsigned int textLength;

			std::unique_ptr<char32_t[]> text(TextLoader().Load(option.SourceCodeFileName(), textLength));

			std::unique_ptr<forward_list<Token>> tokenList(Tokenizer().Tokenize(option.SourceCodeFileName(), text.get(), textLength));
			text.reset();

			std::unique_ptr<BlockNode> root(Parser().Parse(tokenList.get()));
			tokenList.reset();

			SemanticAnalyzer().SemanticAnalysis(root.get());
		}
		Logger::CompilationTerminated();

		return;
	}
}
