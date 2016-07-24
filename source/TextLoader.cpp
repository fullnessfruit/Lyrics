#include "TextLoader.h"

#include <new>

#include "TextEncoder.h"

#include "FatalErrorCode.h"

#include "Utility.h"

namespace lyrics
{
	char32_t *TextLoader::Load(const string &name, unsigned int &length)
	{
		using std::bad_alloc;

		char *data = nullptr;
		unsigned int size = 0;

		data = Loader::Load(name, size);

		char32_t *text;

		try
		{
			text = TextEncoder().DecodeUnicode((const unsigned char * const)data, size, length);
		}
		catch (const bad_alloc &e)
		{
			Utility::SafeArrayDelete(data);
			throw FatalErrorCode::NOT_ENOUGH_MEMORY;
		}

		Utility::SafeArrayDelete(data);
		return text;
	}
}