#ifndef ERROR_HANDLER
#define ERROR_HANDLER

#include "Location.h"
#include "WarningCode.h"
#include "ErrorCode.h"
#include "FatalErrorCode.h"

namespace lyrics
{
	class ErrorHandler
	{
	private:
		ErrorHandler() = delete;

	public:
		static void OnWarning(const Location location, const WarningCode warningCode);
		static void OnError(const Location location, const ErrorCode errorCode);
		static void OnFatalError(const FatalErrorCode &fatalErrorCode);

	private:
		static constexpr char WARNING[] = "warning";
		static constexpr char ERROR[] = "error";
		static constexpr char FATAL_ERROR[] = "fatal error";
	};
}

#endif