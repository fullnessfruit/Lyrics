#include "SemanticAnalyzer.h"

#include <new>

#include "Scope.h"
#include "Parser.h"
#include "LocalResolver.h"
#include "DereferenceChecker.h"
#include "StaticTypeChecker.h"

#include "ErrorCode.h"
#include "FatalErrorCode.h"

namespace lyrics
{
	BlockNode *SemanticAnalyzer::SemanticAnalysis(const string &fileName)
	{
		using std::bad_alloc;

		BlockNode *root = Parser().Parse(fileName);
		Scope *top = nullptr;
		bool canProgress = true;

		try
		{
			canProgress &= LocalResolver().Resolve(root, top);
		}
		catch (const bad_alloc &e)
		{
			Utility::SafeDelete(root);
			Utility::SafeDelete(top);
			throw FatalErrorCode::NOT_ENOUGH_MEMORY;
		}
		Utility::SafeDelete(top);

		canProgress &= DereferenceChecker().Check(root);

		canProgress &= StaticTypeChecker().Check(root);

		if (!canProgress)
		{
			throw ErrorCode::SEMANTIC_ERROR;
		}

		return root;
	}
}