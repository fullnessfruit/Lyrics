#include <iostream>

#include "Compiler.h"
#include "ErrorCode.h"

namespace lyrics
{
	const u16string Scanner::BREAK = u"break";
	const u16string Scanner::CASE = u"case";
	const u16string Scanner::CLASS = u"class";
	const u16string Scanner::DO = u"do";
	const u16string Scanner::END = u"end";
	const u16string Scanner::ELSE = u"else";
	const u16string Scanner::ELSEIF = u"elseif";
	const u16string Scanner::FALSE = u"false";
	const u16string Scanner::FOR = u"for";
	const u16string Scanner::IF = u"if";
	const u16string Scanner::IMPORT = u"import";
	const u16string Scanner::IN = u"in";
	const u16string Scanner::NIL = u"nil";
	const u16string Scanner::PACKAGE = u"package";
	const u16string Scanner::PROC = u"proc";
	const u16string Scanner::REDO = u"redo";
	const u16string Scanner::RETURN = u"return";
	const u16string Scanner::THEN = u"then";
	const u16string Scanner::TRUE = u"true";
	const u16string Scanner::WHEN = u"when";
	const u16string Scanner::WHILE = u"while";
};

namespace
{
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

		constexpr char ERROR[] = "Error";

		switch ( errorCode )
		{
		case lyrics::ErrorCode::NO_INPUT_FILE:
			cout << ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ": " << "No input file.";
			break;

		default:
			cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode );
			break;
		}
	}
};