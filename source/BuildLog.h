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

			case ErrorCode::EXPECTED_RIGHT_PARENTHESIS:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected )." << endl;
				break;

			case ErrorCode::INCOMPLETE_ARRAY_LITERAL:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Incomplete array literal." << endl;
				break;

			case ErrorCode::EXPECTED_HASH:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected hash." << endl;
				break;

			case ErrorCode::INCOMPLETE_HASH_LITERAL:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Incomplete hash literal." << endl;
				break;

			case ErrorCode::EXPECTED_PRIMARY_EXPRESSION:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected primary expression." << endl;
				break;

			case ErrorCode::EXPECTED_INDEX:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected index." << endl;
				break;

			case ErrorCode::EXPECTED_FUNCTION_CALL:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected function call." << endl;
				break;

			case ErrorCode::EXPECTED_MEMBER:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected member." << endl;
				break;

			case ErrorCode::EXPECTED_LHS:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected lhs." << endl;
				break;

			case ErrorCode::EXPECTED_VARIABLE_NAME:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected variable name." << endl;
				break;

			case ErrorCode::EXPECTED_PARAMETER_NAME:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected parameter name." << endl;
				break;

			case ErrorCode::OUTPUT_PARAMETER_DEFAULT_ARGUMENT:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Output parameter cannot have default argument." << endl;
				break;

			case ErrorCode::INCOMPLETE_FUNCTION:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Incomplete function." << endl;
				break;

			case ErrorCode::EXPECTED_PARAMETER:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected parameter." << endl;
				break;

			case ErrorCode::EXPECTED_BASE_CLASS:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected base class." << endl;
				break;

			case ErrorCode::EXPECTED_CLASS_NAME:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected class name." << endl;
				break;

			case ErrorCode::EXPECTED_PACKAGE_NAME:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected package name." << endl;
				break;

			case ErrorCode::EXPECTED_PACKAGE:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected package." << endl;
				break;

			case ErrorCode::EXPECTED_END:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected end." << endl;
				break;

			case ErrorCode::EXPECTED_END_ELSE_ELSEIF:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected end or else or elseif." << endl;
				break;

			case ErrorCode::EXPECTED_WHEN_ELSE_ELSEIF:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected when or else or elseif." << endl;
				break;

			case ErrorCode::EXPECTED_WHEN:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Expected when." << endl;
				break;

			case ErrorCode::INCOMPLETE_FOR:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Incomplete for statement." << endl;
				break;

			case ErrorCode::INCOMPLETE_FOR_FOR_EACH:
				cout << BuildLog::ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Incomplete for or for each statement." << endl;
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

			case FatalErrorCode::TEXT_LOADING_FAILED:
				cout << BuildLog::FATAL_ERROR << ' ' << static_cast<unsigned int>( fatalErrorCode ) << ": " << "Cannot load text." << endl;
				break;

			default:
				cerr << BuildLog::FATAL_ERROR << ' ' << static_cast<unsigned int>( fatalErrorCode ) << endl;
				break;
			}
		}
	};
};

#endif