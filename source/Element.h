#include "Visitor.h"

#ifndef ELEMENT
#define ELEMENT

namespace lyrics
{
	class Element
	{
	public:
		virtual ~Element()
		{
		}

		virtual bool Accept( Visitor &visitor ) const = 0;
	};
}

#endif
