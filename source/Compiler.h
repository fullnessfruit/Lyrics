#include <string>

#include "SemanticAnalyzer.h"

#ifndef COMPILER
#define COMPILER

namespace lyrics
{
	using std::string;

	class Compiler
	{
	private:
		Compiler() = delete;

	public:
		static bool Compile( const string sourceFileName )
		{
			BlockNode *root = nullptr;

			if ( !SemanticAnalyzer::SemanticAnalysis( sourceFileName, root ) )
			{
				delete root;

				return false;
			}

			return true;
		}
	};
}

#endif
