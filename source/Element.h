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

		virtual void Accept( const Visitor &visitor ) const = 0;
	};
}

#endif
