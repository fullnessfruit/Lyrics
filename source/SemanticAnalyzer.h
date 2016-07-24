#ifndef SEMANTIC_ANALYZER
#define SEMANTIC_ANALYZER

#include <string>

#include "Node.h"

namespace lyrics
{
	using std::string;

	class SemanticAnalyzer
	{
	public:
		BlockNode *SemanticAnalysis(BlockNode *root);
	};
}

#endif