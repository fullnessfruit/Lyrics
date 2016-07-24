#include "TextLoader.h"
#include "Compiler.h"

#include "Option.h"
#include "FatalErrorCode.h"
#include "ErrorHandler.h"

#include "Utility.h"

int main(int argc, char *argv[])
{
	using lyrics::Option;
	using lyrics::TextLoader;
	using lyrics::Compiler;
	using lyrics::Utility;
	using lyrics::FatalErrorCode;
	using lyrics::ErrorHandler;

	const Option option = Option(argc, argv);
	char32_t *text;

	try
	{
		unsigned int textLength;

		text = TextLoader().Load(option.SourceCodeFileName(), textLength);
		Compiler().Compile(option, text, textLength);
		Utility::SafeArrayDelete(text);
	}
	catch (const FatalErrorCode fatalErrorCode)
	{
		switch (fatalErrorCode)
		{
		case FatalErrorCode::NOT_ENOUGH_MEMORY:
			Utility::SafeArrayDelete(text);
			break;
		}

		ErrorHandler::OnFatalError(fatalErrorCode);
		return 1;
	}

	return 0;
}