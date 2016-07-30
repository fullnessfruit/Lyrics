#ifndef COMPILER
#define COMPILER

#include "Option.h"

namespace lyrics
{
	class Compiler
	{
	public:
		void Compile(const Option &option) const;
	};
}

#endif