#ifndef ERROR_CODE
#define ERROR_CODE

namespace lyrics
{
	enum class ErrorCode : unsigned int { NO_INPUT_FILES = 1, WRONG_CHARACTER = 3001, STRING_NOT_TERMINATED };
};

#endif