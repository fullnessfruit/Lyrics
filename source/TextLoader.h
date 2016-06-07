#ifndef TEXT_LOADER
#define TEXT_LOADER

#include <string>

#include "Loader.h"

namespace lyrics
{
	using std::string;

	class TextLoader : public Loader
	{
	public:
		char32_t *Load(const string &name, unsigned int &length);
	};
}

#endif