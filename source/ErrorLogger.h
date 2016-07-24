#ifndef ERROR_LOGGER
#define ERROR_LOGGER

#include "Location.h"
#include "WarningCode.h"
#include "ErrorCode.h"
#include "FatalErrorCode.h"

namespace lyrics
{
	class ErrorLogger
	{
	private:
		ErrorLogger() = delete;

	public:
		static void Warning(const Location location, const WarningCode warningCode);
		static void Error(const Location location, const ErrorCode errorCode);
		static void FatalError(const FatalErrorCode &fatalErrorCode);

	private:
		static constexpr char WARNING[] = "warning";
		static constexpr char ERROR[] = "error";
		static constexpr char FATAL_ERROR[] = "fatal error";
	};
}

#endif