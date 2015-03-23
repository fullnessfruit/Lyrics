#include <cstring>
#include <string>

#include "Loader.h"
#include "TextEncoder.h"

#ifndef TEXT_LOADER
#define TEXT_LOADER

namespace lyrics
{
	using std::string;

	class TextLoader : public Loader
	{
	public:
		char16_t *Load( const string &name, unsigned int &length )
		{
			char *data = nullptr;
			unsigned int size = 0;

			data = Loader::Load( name, size );

			char16_t *text = TextEncoder().EncodeToUTF16( ( const unsigned char * const )data, size, length );

			delete [] data;
			if ( !text )
			{
				throw FatalErrorCode::NOT_ENOUGH_MEMORY;
			}

			return text;
		}
	};
}

#endif
