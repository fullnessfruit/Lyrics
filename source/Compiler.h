#ifndef COMPILER
#define COMPILER

#include "Option.h"

namespace lyrics
{
	class Compiler
	{
	public:
		bool Compile(const Option &option) const;
	};
}

#endif