#include "SemanticAnalyzer.h"

#include <new>

#include "Scope.h"
#include "Parser.h"
#include "LocalResolver.h"
#include "DereferenceChecker.h"
#include "StaticTypeChecker.h"

#include "FatalErrorCode.h"

namespace lyrics
{
	bool SemanticAnalyzer::SemanticAnalysis(const string &fileName)
	{
		using std::bad_alloc;

		const BlockNode *root = Parser().Parse(fileName);
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

		canProgress &= DereferenceChecker().Check(root);

		canProgress &= StaticTypeChecker().Check(root);

		Utility::SafeDelete(top);
		return canProgress;
	}
}