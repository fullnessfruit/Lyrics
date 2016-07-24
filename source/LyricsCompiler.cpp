#include "Compiler.h"

#include "Option.h"
#include "FatalErrorCode.h"
#include "ErrorHandler.h"

int main(int argc, char *argv[])
{
	using lyrics::Option;
	using lyrics::Compiler;

	const Option option = Option(argc, argv);

	try
	{
		if (!Compiler().Compile(option))
		{
			return 1;
		}
	}
	catch (const lyrics::FatalErrorCode fatalErrorCode)
	{
		lyrics::ErrorHandler::OnFatalError(fatalErrorCode);
		return 1;
	}

	return 0;
}