#include <iostream>

#include "Location.h"

#include "WarningCode.h"
#include "ErrorCode.h"
#include "FatalErrorCode.h"

#ifndef BUILD_LOG
#define BUILD_LOG

namespace lyrics
{
	using std::cout;
	using std::cerr;
	using std::endl;

	class BuildLog
	{
	private:
		BuildLog()
		{
		}

		static constexpr char WARNING[] = "warning";
		static constexpr char ERROR[] = "error";
		static constexpr char FATAL_ERROR[] = "fatal error";

	public:
		static void Warning( const WarningCode warningCode, const Location location )
		{
			switch ( warningCode )
			{
			case WarningCode::UNKNOWN_ESCAPE_SEQUENCE:
				cout << BuildLog::WARNING << ' ' << static_cast<unsigned int>( warningCode ) << ' ' << location << ' ' << "Unknown escape sequence." << endl;
				break;

			default:
				cerr << BuildLog::WARNING << ' ' << static_cast<unsigned int>( warningCode ) << endl;
				break;
			}
		}

		static void Error( const ErrorCode errorCode, const Location location )
		{
			switch ( errorCode )
			{
			case ErrorCode::WRONG_CHARACTER:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Wrong character." << endl;
				break;

			case ErrorCode::STRING_NOT_TERMINATED:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "String not terminated." << endl;
				break;

			default:
				cerr << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << endl;
				break;
			}
		}

		static void FatalError( const FatalErrorCode fatalErrorCode )
		{
			switch ( fatalErrorCode )
			{
			case FatalErrorCode::NO_INPUT_FILES:
				cout << BuildLog::FATAL_ERROR << ' ' << static_cast<unsigned int>( fatalErrorCode ) << ": " << "No input files." << endl;
				break;

			default:
				cerr << BuildLog::FATAL_ERROR << ' ' << static_cast<unsigned int>( fatalErrorCode ) << endl;
				break;
			}
		}
	};
};

#endif