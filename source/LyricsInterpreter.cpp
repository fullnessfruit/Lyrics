#include <string>

#include "Option.h"

#include "Interpreter.h"

#include "FatalErrorCode.h"
#include "ErrorHandler.h"

namespace lyrics
{
	using std::u16string;

	const u16string Tokenizer::BREAK = u"break";
	const u16string Tokenizer::CASE = u"case";
	const u16string Tokenizer::CLASS = u"class";
	const u16string Tokenizer::DEF = u"def";
	const u16string Tokenizer::DO = u"do";
	const u16string Tokenizer::END = u"end";
	const u16string Tokenizer::ELSE = u"else";
	const u16string Tokenizer::ELSEIF = u"elseif";
	const u16string Tokenizer::FALSE = u"false";
	const u16string Tokenizer::FOR = u"for";
	const u16string Tokenizer::FOREACH = u"foreach";
	const u16string Tokenizer::IF = u"if";
	const u16string Tokenizer::IMPORT = u"import";
	const u16string Tokenizer::IN = u"in";
	const u16string Tokenizer::INCLUDE = u"include";
	const u16string Tokenizer::NEXT = u"next";
	const u16string Tokenizer::NULL_TOKEN = u"null";
	const u16string Tokenizer::OUT = u"out";
	const u16string Tokenizer::PACKAGE = u"package";
	const u16string Tokenizer::PRIVATE = u"private";
	const u16string Tokenizer::PROTECTED = u"protected";
	const u16string Tokenizer::PUBLIC = u"public";
	const u16string Tokenizer::RETURN = u"return";
	const u16string Tokenizer::THEN = u"then";
	const u16string Tokenizer::THIS = u"this";
	const u16string Tokenizer::TRUE = u"true";
	const u16string Tokenizer::WHEN = u"when";
	const u16string Tokenizer::WHILE = u"while";

	forward_list<Token>::const_iterator Interpreter::mToken;
	unordered_map<u16string, Literal> Interpreter::mSymbolTable;

	constexpr char ErrorHandler::WARNING[];
	constexpr char ErrorHandler::ERROR[];
	constexpr char ErrorHandler::FATAL_ERROR[];
}

int main( int argc, char *argv[] )
{
	const lyrics::Option option = lyrics::Option( argc, argv );

	if ( option.GetSourceFileName().empty() )
	{
		lyrics::Logger::ErrorHandler( lyrics::FatalErrorCode::NO_INPUT_FILES );
		return 0;
	}

	if ( !lyrics::Interpreter::Interpret( option.GetSourceFileName() ) )
	{
		// TODO:
		return 0;
	}

	return 0;
}
