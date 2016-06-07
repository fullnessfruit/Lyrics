#ifndef ELEMENT
#define ELEMENT

#include "Visitor.h"

namespace lyrics
{
	class Element
	{
	public:
		virtual ~Element()
		{
		}

		virtual bool Accept(Visitor &visitor) const = 0;
	};
}

#endif