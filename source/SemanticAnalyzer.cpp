#include "SemanticAnalyzer.h"

#include <new>

#include "Scope.h"
#include "LocalResolver.h"
#include "DereferenceChecker.h"
#include "StaticTypeChecker.h"

#include "ErrorCode.h"
#include "FatalErrorCode.h"

#include "Utility.h"

namespace lyrics
{
	BlockNode *SemanticAnalyzer::SemanticAnalysis(BlockNode *root)
	{
		using std::bad_alloc;

		Scope *top = nullptr;
		bool canProgress = true;

		try
		{
			canProgress &= LocalResolver().Resolve(root, top);
		}
		catch (const bad_alloc &e)
		{
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