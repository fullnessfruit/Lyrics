#ifndef FATAL_ERROR_CODE
#define FATAL_ERROR_CODE

namespace lyrics
{
	enum class FatalErrorCode: unsigned int
	{
		NO_INPUT_FILE = 1001,
		NO_SUCH_FILE = 2001
	};
}

#endif
