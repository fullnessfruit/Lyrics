#include "Compiler.h"

#include "Option.h"
#include "FatalErrorCode.h"
#include "ErrorLogger.h"

#include "Utility.h"

int main(int argc, char *argv[])
{
	using lyrics::Option;
	using lyrics::Compiler;
	using lyrics::Utility;
	using lyrics::FatalErrorCode;
	using lyrics::ErrorLogger;

	const Option option = Option(argc, argv);

	try
	{
		Compiler().Compile(option);
	}
	catch (const FatalErrorCode fatalErrorCode)
	{
		ErrorLogger::FatalError(fatalErrorCode);
		return 1;
	}

	return 0;
}