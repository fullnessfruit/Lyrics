#include <string>

#include "Compiler.h"

#include "FatalErrorCode.h"
#include "BuildLog.h"

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
	const u16string Tokenizer::NIL = u"nil";
	const u16string Tokenizer::OUT = u"out";
	const u16string Tokenizer::PACKAGE = u"package";
	const u16string Tokenizer::PRIVATE = u"private";
	const u16string Tokenizer::PUBLIC = u"public";
	const u16string Tokenizer::REDO = u"redo";
	const u16string Tokenizer::RETURN = u"return";
	const u16string Tokenizer::THEN = u"then";
	const u16string Tokenizer::TRUE = u"true";
	const u16string Tokenizer::WHEN = u"when";
	const u16string Tokenizer::WHILE = u"while";

	constexpr char BuildLog::WARNING[];
	constexpr char BuildLog::ERROR[];
	constexpr char BuildLog::FATAL_ERROR[];
};

int main( int argc, char *argv[] )
{
	if ( argc < 2 )
	{
		lyrics::BuildLog::FatalError( lyrics::FatalErrorCode::NO_INPUT_FILES );
		return 0;
	}

	lyrics::Compiler compiler;

	for ( int i = 1; i < argc; i++ )
	{
		if ( !compiler.Compile( argv[i] ) )
		{
			// TODO:
			return 0;
		}
	}

	return 0;
}
