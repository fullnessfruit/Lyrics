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
		BlockNode *SemanticAnalysis(const string &fileName);
	};
}

#endif