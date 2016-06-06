#include <string>
#include <forward_list>

#include "Compiler.h"

#include "Option.h"
#include "Token.h"
#include "ErrorHandler.h"
#include "FatalErrorCode.h"

namespace lyrics
{
	using std::u32string;
	using std::forward_list;

	const u32string Tokenizer::BREAK = U"break";
	const u32string Tokenizer::CASE = U"case";
	const u32string Tokenizer::CLASS = U"class";
	const u32string Tokenizer::DO = U"do";
	const u32string Tokenizer::END = U"end";
	const u32string Tokenizer::ELSE = U"else";
	const u32string Tokenizer::ELSEIF = U"elseif";
	const u32string Tokenizer::FALSE = U"false";
	const u32string Tokenizer::FOR = U"for";
	const u32string Tokenizer::FOREACH = U"foreach";
	const u32string Tokenizer::IF = U"if";
	const u32string Tokenizer::IMPORT = U"import";
	const u32string Tokenizer::IN = U"in";
	const u32string Tokenizer::INCLUDE = U"include";
	const u32string Tokenizer::NEXT = U"next";
	const u32string Tokenizer::NULL_TOKEN = U"null";
	const u32string Tokenizer::OUT = U"out";
	const u32string Tokenizer::PACKAGE = U"package";
	const u32string Tokenizer::PRIVATE = U"private";
	const u32string Tokenizer::PUBLIC = U"public";
	const u32string Tokenizer::RETURN = U"return";
	const u32string Tokenizer::ROUTINE = U"routine";
	const u32string Tokenizer::THEN = U"then";
	const u32string Tokenizer::THIS = U"this";
	const u32string Tokenizer::TRUE = U"true";
	const u32string Tokenizer::WHEN = U"when";
	const u32string Tokenizer::WHILE = U"while";

	constexpr char ErrorHandler::WARNING[];
	constexpr char ErrorHandler::ERROR[];
	constexpr char ErrorHandler::FATAL_ERROR[];
}

int main( int argc, char *argv[] )
{
	using lyrics::Option;
	using lyrics::Compiler;

	const Option option = Option( argc, argv );

	try
	{
		if ( !Compiler().Compile( option ) )
		{
			return 1;
		}
	}
	catch ( const lyrics::FatalErrorCode fatalErrorCode )
	{
		lyrics::ErrorHandler::OnFatalError( fatalErrorCode );

		return 1;
	}

	return 0;
}
