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
				cout << location << ' ' << BuildLog::WARNING << ' ' << static_cast<unsigned int>( warningCode ) << ": " << "Unknown escape sequence." << endl;
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
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Wrong character." << endl;
				break;

			case ErrorCode::STRING_NOT_TERMINATED:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "String not terminated." << endl;
				break;

			case ErrorCode::EXPECTED_RIGHT_PARENTHESIS:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected )." << endl;
				break;

			case ErrorCode::INCOMPLETE_ARRAY_LITERAL:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Incomplete array literal." << endl;
				break;

			case ErrorCode::EXPECTED_HASH:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected hash." << endl;
				break;

			case ErrorCode::INCOMPLETE_HASH_LITERAL:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Incomplete hash literal." << endl;
				break;

			case ErrorCode::EXPECTED_PRIMARY_EXPRESSION:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected primary expression." << endl;
				break;

			case ErrorCode::EXPECTED_INDEX:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected index." << endl;
				break;

			case ErrorCode::EXPECTED_FUNCTION_CALL:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected function call." << endl;
				break;

			case ErrorCode::EXPECTED_MEMBER:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected member." << endl;
				break;

			case ErrorCode::EXPECTED_LHS:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected lhs." << endl;
				break;

			case ErrorCode::EXPECTED_VARIABLE_NAME:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected variable name." << endl;
				break;

			case ErrorCode::EXPECTED_PARAMETER_NAME:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected parameter name." << endl;
				break;

			case ErrorCode::OUTPUT_PARAMETER_DEFAULT_ARGUMENT:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Output parameter cannot have default argument." << endl;
				break;

			case ErrorCode::INCOMPLETE_FUNCTION:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Incomplete function." << endl;
				break;

			case ErrorCode::EXPECTED_PARAMETER:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected parameter." << endl;
				break;

			case ErrorCode::EXPECTED_BASE_CLASS:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected base class." << endl;
				break;

			case ErrorCode::EXPECTED_CLASS_NAME:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected class name." << endl;
				break;

			case ErrorCode::EXPECTED_PACKAGE_NAME:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected package name." << endl;
				break;

			case ErrorCode::EXPECTED_PACKAGE:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected package." << endl;
				break;

			case ErrorCode::EXPECTED_END:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected end." << endl;
				break;

			case ErrorCode::EXPECTED_END_ELSE_ELSEIF:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected end or else or elseif." << endl;
				break;

			case ErrorCode::EXPECTED_WHEN_ELSE_ELSEIF:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected when or else or elseif." << endl;
				break;

			case ErrorCode::EXPECTED_WHEN:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Expected when." << endl;
				break;

			case ErrorCode::INCOMPLETE_FOR:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Incomplete for statement." << endl;
				break;

			case ErrorCode::INCOMPLETE_FOREACH:
				cout << location << ' ' << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "Incomplete foreach statement." << endl;
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

			case FatalErrorCode::NO_SUCH_FILE:
				cout << BuildLog::FATAL_ERROR << ' ' << static_cast<unsigned int>( fatalErrorCode ) << ": " << "No such file." << endl;
				break;

			default:
				cerr << BuildLog::FATAL_ERROR << ' ' << static_cast<unsigned int>( fatalErrorCode ) << endl;
				break;
			}
		}
	};
};

#endif
