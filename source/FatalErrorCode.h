#ifndef FATAL_ERROR_CODE
#define FATAL_ERROR_CODE

namespace lyrics
{
	enum class FatalErrorCode: unsigned int
	{
		NO_INPUT_FILE = 1001,
		CANNOT_OPEN_FILE,
		CANNOT_READ_FILE,
		CANNOT_CLOSE_FILE,
		NOT_ENOUGH_MEMORY,
	};
}

#endif
