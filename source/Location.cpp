#include "Location.h"

#include <ostream>

namespace lyrics
{
	ostream &operator<<(ostream &out, const Location &location)
	{
		return out << location.mFileName << ':' << location.mLine << ':' << location.mColumn << ':';
	}
}