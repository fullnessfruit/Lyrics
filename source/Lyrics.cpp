#include <iostream>
#include <iomanip>
#include <string>

#include "Compiler.h"
#include "ErrorCode.h"

namespace lyrics
{
	const u16string Tokenizer::BREAK = u"break";
	const u16string Tokenizer::CASE = u"case";
	const u16string Tokenizer::CLASS = u"class";
	const u16string Tokenizer::DO = u"do";
	const u16string Tokenizer::END = u"end";
	const u16string Tokenizer::ELSE = u"else";
	const u16string Tokenizer::ELSEIF = u"elseif";
	const u16string Tokenizer::FALSE = u"false";
	const u16string Tokenizer::FOR = u"for";
	const u16string Tokenizer::IF = u"if";
	const u16string Tokenizer::IMPORT = u"import";
	const u16string Tokenizer::IN = u"in";
	const u16string Tokenizer::NIL = u"nil";
	const u16string Tokenizer::OUT = u"out";
	const u16string Tokenizer::PACKAGE = u"package";
	const u16string Tokenizer::PROC = u"proc";
	const u16string Tokenizer::REDO = u"redo";
	const u16string Tokenizer::RETURN = u"return";
	const u16string Tokenizer::THEN = u"then";
	const u16string Tokenizer::TRUE = u"true";
	const u16string Tokenizer::WHEN = u"when";
	const u16string Tokenizer::WHILE = u"while";
};

namespace
{
	using std::setw;
	using std::setfill;

	void Error( const lyrics::ErrorCode errorCode );
};

int main( int argc, char *argv[] )
{
	if ( argc < 2 )
	{
		Error( lyrics::ErrorCode::NO_INPUT_FILE );
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

namespace
{
	void Error( const lyrics::ErrorCode errorCode )
	{
		using std::cout;
		using std::cerr;
		using std::endl;

		constexpr char FATAL_ERROR[] = "fatal error";

		switch ( errorCode )
		{
		case lyrics::ErrorCode::NO_INPUT_FILE:
			cout << FATAL_ERROR << ' ' << std::setw( 4 ) << std::setfill( '0' ) << static_cast<unsigned int>( errorCode ) << ": " << "No input files." << endl;
			break;

		default:
			cerr << FATAL_ERROR << ' ' << std::setw( 4 ) << std::setfill( '0' ) << static_cast<unsigned int>( errorCode ) << endl;
			break;
		}
	}
};