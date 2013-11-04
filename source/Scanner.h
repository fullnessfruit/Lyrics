#include <iostream>
#include <string>

#include "Token.h"
#include "WarningCode.h"
#include "ErrorCode.h"
#include "Location.h"

#ifndef SCANNER
#define SCANNER

namespace lyrics
{
	using std::u16string;

	class Scanner
	{
	public:
		// TODO: length is byte of text, not number of characters.
		void Scan( const char16_t * const text, const unsigned int length, unsigned int &offset, Location &location, Token &token ) const
		{
			char16_t tChar;

			if ( offset < length )
			{
				tChar = text[offset];
			}
			else
			{
				token.type = Token::Type::END_OF_FILE;
				token.location = location;
				return;
			}

			if ( ( u'A' <= tChar && tChar <= u'Z' ) || ( u'a' <= tChar && tChar <= u'z' ) || tChar == u'_' )
			{
				u16string *tStr = new u16string();
				bool isIdentifier = false;

				token.location = location;

				do
				{
					tStr->push_back( tChar );
					
					if ( ++offset < length )
					{
						tChar = text[offset];
					}
					else
					{
						break;
					}
				}
				while ( ( u'A' <= tChar && tChar <= u'Z' ) || ( u'a' <= tChar && tChar <= u'z' ) || ( ( u'0' <= tChar && tChar <= u'9' ) || tChar == u'_' ? isIdentifier = true : false ) );	// If the string includs number or _, then the string is identifier.

				location.increaseColumn( tStr->length() );

				if ( isIdentifier )
				{
					token.type = Token::Type::IDENTIFIER;
					token.value.identifier = tStr;
				}
				else
				{
					if ( 2 > token.value.identifier->length() || 7 < token.value.identifier->length() )	// If the token is shorter than 2 or longer than 7, then the token is itentifier.
					{
						token.type = Token::Type::IDENTIFIER;
						token.value.identifier = tStr;
					}
					else
					{
						switch ( tStr->length() )
						{
						case 2:
							if ( *tStr == Scanner::IF )
							{
								delete tStr;
								token.type = Token::Type::IF;
							}
							else if ( *tStr == Scanner::DO )
							{
								delete tStr;
								token.type = Token::Type::DO;
							}
							else if ( *tStr == Scanner::IN )
							{
								delete tStr;
								token.type = Token::Type::IN;
							}
							else
							{
								token.type = Token::Type::IDENTIFIER;
								token.value.identifier = tStr;
							}
							break;

						case 3:
							if ( *tStr == Scanner::END )
							{
								delete tStr;
								token.type = Token::Type::END;
							}
							else if ( *tStr == Scanner::FOR )
							{
								delete tStr;
								token.type = Token::Type::FOR;
							}
							else if ( *tStr == Scanner::NIL )
							{
								delete tStr;
								token.type = Token::Type::NIL_LITERAL;
							}
							else
							{
								token.type = Token::Type::IDENTIFIER;
								token.value.identifier = tStr;
							}
							break;

						case 4:
							if ( *tStr == Scanner::ELSE )
							{
								delete tStr;
								token.type = Token::Type::ELSE;
							}
							else if ( *tStr == Scanner::PROC )
							{
								delete tStr;
								token.type = Token::Type::PROC;
							}
							else if ( *tStr == Scanner::TRUE )
							{
								delete tStr;
								token.type = Token::Type::BOOLEAN_LITERAL;
								token.value.boolean = true;
							}
							else if ( *tStr == Scanner::CASE )
							{
								delete tStr;
								token.type = Token::Type::CASE;
							}
							else if ( *tStr == Scanner::WHEN )
							{
								delete tStr;
								token.type = Token::Type::WHEN;
							}
							else if ( *tStr == Scanner::REDO )
							{
								delete tStr;
								token.type = Token::Type::REDO;
							}
							else if ( *tStr == Scanner::THEN )
							{
								delete tStr;
								token.type = Token::Type::THEN;
							}
							else
							{
								token.type = Token::Type::IDENTIFIER;
								token.value.identifier = tStr;
							}
							break;

						case 5:
							if ( *tStr == Scanner::WHILE )
							{
								delete tStr;
								token.type = Token::Type::WHILE;
							}
							else if ( *tStr == Scanner::BREAK )
							{
								delete tStr;
								token.type = Token::Type::BREAK;
							}
							else if ( *tStr == Scanner::FALSE )
							{
								delete tStr;
								token.type = Token::Type::BOOLEAN_LITERAL;
								token.value.boolean = false;
							}
							else if ( *tStr == Scanner::CLASS )
							{
								delete tStr;
								token.type = Token::Type::CLASS;
							}
							else
							{
								token.type = Token::Type::IDENTIFIER;
								token.value.identifier = tStr;
							}
							break;

						case 6:
							if ( *tStr == Scanner::ELSEIF )
							{
								delete tStr;
								token.type = Token::Type::ELSEIF;
							}
							else if ( *tStr == Scanner::IMPORT )
							{
								delete tStr;
								token.type = Token::Type::IMPORT;
							}
							else if ( *tStr == Scanner::RETURN )
							{
								delete tStr;
								token.type = Token::Type::RETURN;
							}
							else
							{
								token.type = Token::Type::IDENTIFIER;
								token.value.identifier = tStr;
							}
							break;

						case 7:
							if ( *tStr == Scanner::PACKAGE )
							{
								delete tStr;
								token.type = Token::Type::PACKAGE;
							}
							else
							{
								token.type = Token::Type::IDENTIFIER;
								token.value.identifier = tStr;
							}
							break;
						}
					}
				}
			}
			else if ( tChar == u' ' || tChar == u'\t' || tChar == u'\r' || tChar == u'\v' || tChar == u'\f' )
			{
				offset++;

				Scan( text, length, offset, location, token );
			}
			else if ( tChar == u'\n' )
			{
				location.increaseLine();
				location.resetColumn();
				offset++;

				Scan( text, length, offset, location, token );
			}
			else if ( tChar == u'.' )
			{
				token.location = location;

				location.increaseColumn();

				if ( ++offset < length )
				{
					tChar = text[offset];

					if ( tChar < u'0' || tChar > u'9' )
					{
						token.type = static_cast<Token::Type>( u'.' );
					}
					else	// real
					{
						long long valueBelowDecimalPoint = 0;
						long long tenPowerDecimalPlace = 1;

						do
						{
							valueBelowDecimalPoint = ( valueBelowDecimalPoint << 3 ) + ( valueBelowDecimalPoint << 1 );
							valueBelowDecimalPoint += tChar & 15;
							tenPowerDecimalPlace = ( tenPowerDecimalPlace << 3 ) + ( tenPowerDecimalPlace << 1 );

							location.increaseColumn();
							
							if ( ++offset < length )
							{
								tChar = text[offset];
							}
							else
							{
								break;
							}
						}
						while ( u'0' <= tChar && tChar <= u'9' );

						token.type = Token::Type::REAL_LITERAL;
						token.value.real = double( valueBelowDecimalPoint ) / tenPowerDecimalPlace;
					}
				}
				else
				{
					token.type = static_cast<Token::Type>( u'.' );
				}
			}
			else if ( tChar == u'(' || tChar == u')' || tChar == u'+' || tChar == u'-' || tChar == u'*' || tChar == u'/' || tChar == u'[' || tChar == u']' )
			{
				offset++;
				token.type = static_cast<Token::Type>( tChar );
				token.location = location;
				location.increaseColumn();
			}
			else if ( tChar == u'<' )
			{
				token.location = location;

				if ( ++offset < length )
				{
					switch ( text[offset] )
					{
					case u'<':
						token.type = Token::Type::SHIFT_LEFT;
						location.increaseColumn( 2 );
						offset++;
						break;

					case u'=':
						token.type = Token::Type::LESS_THAN_OR_EQUAL;
						location.increaseColumn( 2 );
						offset++;
						break;
					
					default:
						token.type = static_cast<Token::Type>( u'<' );
						location.increaseColumn();
						break;
					}
				}
				else
				{
					token.type = static_cast<Token::Type>( u'<' );
					location.increaseColumn();
				}
			}
			else if ( tChar == u'>' )
			{
				token.location = location;

				if ( ++offset < length )
				{
					switch ( text[offset] )
					{
					case u'>':
						token.type = Token::Type::SHIFT_RIGHT;
						location.increaseColumn( 2 );
						offset++;
						break;

					case u'=':
						token.type = Token::Type::GREATER_THAN_OR_EQUAL;
						location.increaseColumn( 2 );
						offset++;
						break;
					
					default:
						token.type = static_cast<Token::Type>( u'>' );
						location.increaseColumn();
						break;
					}
				}
				else
				{
					token.type = static_cast<Token::Type>( u'>' );
					location.increaseColumn();
				}
			}
			else if ( tChar == u'=' )
			{
				token.location = location;

				if ( ++offset < length )
				{
					if ( text[offset] == u'=' )
					{
						token.type = Token::Type::EQUAL;
						location.increaseColumn( 2 );
						offset++;
					}
					else
					{
						token.type = static_cast<Token::Type>( u'=' );
						location.increaseColumn();
					}
				}
				else
				{
					token.type = static_cast<Token::Type>( u'=' );
					location.increaseColumn();
				}
			}
			else if ( tChar == u'!' )
			{
				token.location = location;

				if ( ++offset < length )
				{
					if ( text[offset] == u'=' )
					{
						token.type = Token::Type::NOT_EQUAL;
						location.increaseColumn( 2 );
						offset++;
					}
					else
					{
						token.type = static_cast<Token::Type>( u'!' );
						location.increaseColumn();
					}
				}
				else
				{
					token.type = static_cast<Token::Type>( u'!' );
					location.increaseColumn();
				}
			}
			else if ( tChar == u'&' )
			{
				token.location = location;

				if ( ++offset < length )
				{
					if ( text[offset] == u'&' )
					{
						token.type = Token::Type::AND;
						location.increaseColumn( 2 );
						offset++;
					}
					else
					{
						token.type = static_cast<Token::Type>( u'&' );
						location.increaseColumn();
					}
				}
				else
				{
					token.type = static_cast<Token::Type>( u'&' );
					location.increaseColumn();
				}
			}
			else if ( tChar == u'|' )
			{
				token.location = location;

				if ( ++offset < length )
				{
					if ( text[offset] == u'|' )
					{
						token.type = Token::Type::OR;
						location.increaseColumn( 2 );
						offset++;
					}
					else
					{
						token.type = static_cast<Token::Type>( u'|' );
						location.increaseColumn();
					}
				}
				else
				{
					token.type = static_cast<Token::Type>( u'|' );
					location.increaseColumn();
				}
			}
			else if ( u'0' <= tChar && tChar <= u'9' )
			{
				long long integer = 0;
				long long valueBelowDecimalPoint = 0;
				long long tenPowerDecimalPlace = 1;

				token.location = location;

				do
				{
					integer = ( integer << 3 ) + ( integer << 1 );
					integer += tChar & 15;

					location.increaseColumn();
					
					if ( ++offset < length )
					{
						tChar = text[offset];
					}
					else
					{
						break;
					}
				}
				while ( u'0' <= tChar && tChar <= u'9' );

				if ( offset >= length || tChar != u'.' )
				{
					token.type = Token::Type::INTEGER_LITERAL;
					token.value.integer = integer;
					location.increaseColumn();
				}
				else
				{
					if ( ++offset < length )
					{
						tChar = text[offset];

						if ( u'0' <= tChar && tChar <= u'9' )
						{
							do
							{
								valueBelowDecimalPoint = ( valueBelowDecimalPoint << 3 ) + ( valueBelowDecimalPoint << 1 );
								valueBelowDecimalPoint += tChar & 15;
								tenPowerDecimalPlace = ( tenPowerDecimalPlace << 3 ) + ( tenPowerDecimalPlace << 1 );

								location.increaseColumn();
								
								if ( ++offset < length )
								{
									tChar = text[offset];
								}
								else
								{
									break;
								}
							}
							while ( u'0' <= tChar && tChar <= u'9' );
						}
					}

					token.type = Token::Type::REAL_LITERAL;
					token.value.real = integer + double( valueBelowDecimalPoint ) / tenPowerDecimalPlace;
				}
			}
			else if ( tChar == u'\"' )
			{
				u16string *tStr;

				token.location = location;

				if ( ++offset < length )
				{
					tChar = text[offset];
				}
				else
				{
					Error( ErrorCode::STRING_NOT_TERMINATED, location );
					return;
				}

				tStr = new u16string();

				location.increaseColumn();
				do
				{
					if ( tChar == '\\' )
					{
						if ( ++offset < length )
						{
							tChar = text[offset];
						}
						else
						{
							Error( ErrorCode::STRING_NOT_TERMINATED, location );
							return;
						}

						switch ( tChar )
						{
						case u'n':
							tStr->push_back( u'\n' );
							location.increaseColumn( 2 );
							break;

						case u't':
							tStr->push_back( u'\t' );
							location.increaseColumn( 2 );
							break;

						case u'0':
							tStr->push_back( u'\0' );
							location.increaseColumn( 2 );
							break;

						case u'r':
							tStr->push_back( u'\r' );
							location.increaseColumn( 2 );
							break;

						case u'\'':
							tStr->push_back( u'\'' );
							location.increaseColumn( 2 );
							break;

						case u'\"':
							tStr->push_back( u'\"' );
							location.increaseColumn( 2 );
							break;

						case u'\\':
							tStr->push_back( u'\\' );
							location.increaseColumn( 2 );
							break;

						case u'a':
							tStr->push_back( u'\a' );
							location.increaseColumn( 2 );
							break;

						case u'b':
							tStr->push_back( u'\b' );
							location.increaseColumn( 2 );
							break;

						case u'f':
							tStr->push_back( u'\f' );
							location.increaseColumn( 2 );
							break;

						case u'v':
							tStr->push_back( u'\v' );
							location.increaseColumn( 2 );
							break;

						case u'\n':
							location.increaseLine();
							location.resetColumn();
							break;

						case u'\r':
							if ( ++offset < length )
							{
								tChar = text[offset];
							}
							else
							{
								Error( ErrorCode::STRING_NOT_TERMINATED, location );
								return;
							}

							if ( tChar == '\n' )
							{
								location.increaseLine();
								location.resetColumn();
							}
							else
							{
								Warning( WarningCode::UNKNOWN_ESCAPE_SEQUENCE, location );
								tStr->push_back( tChar );
								location.increaseColumn( 3 );
							}
							break;
						
						default:
							Warning( WarningCode::UNKNOWN_ESCAPE_SEQUENCE, location );
							break;
						}
					}
					else if ( tChar == '\r' || tChar == '\n' )
					{
						Error( ErrorCode::STRING_NOT_TERMINATED, location );
						return;
					}
					else
					{
						tStr->push_back( tChar );
						location.increaseColumn();
					}
					
					if ( ++offset < length )
					{
						tChar = text[offset];
					}
					else
					{
						Error( ErrorCode::STRING_NOT_TERMINATED, location );
						return;
					}
				}
				while ( tChar != '\"' );
				location.increaseColumn();

				token.type = Token::Type::STRING_LITERAL;
				token.value.string = tStr;
			}
			else if ( tChar == u'#' )
			{
				do
				{
					if ( ++offset < length )
					{
						tChar = text[offset];
					}
					else
					{
						token.type = Token::Type::END_OF_FILE;
					}
				}
				while ( tChar != u'\r' && tChar != u'\n' );

				Scan( text, length, offset, location, token );
			}
			else if ( tChar == u'{' || tChar == u'}' || tChar == u',' || tChar == u':' || tChar == u'~' || tChar == u'%' || tChar == u'^' || tChar == u'?' )
			{
				offset++;
				token.type = static_cast<Token::Type>( tChar );
				token.location = location;
				location.increaseColumn();
			}
			else if ( tChar == Scanner::NO_BREAK_SPACE || tChar == Scanner::OGHAM_SPACE_MARK || tChar == Scanner::MONGOLIAN_VOWEL_SEPARATOR || ( tChar >= Scanner::EN_QUAD && tChar <= Scanner::HAIR_SPACE ) || tChar == Scanner::NARROW_NO_BREAK_SPACE || tChar == Scanner::MEDIUM_MATHEMATICAL_SPACE || tChar == Scanner::IDEOGRAPHIC_SPACE )	// Unicode class Zs except space character.
			{
				offset++;

				Scan( text, length, offset, location, token );
			}
			else
			{
				Error( ErrorCode::WRONG_CHARACTER, location );
			}
		}
	
	private:
		static const u16string BREAK;
		static const u16string CASE;
		static const u16string CLASS;
		static const u16string DO;
		static const u16string END;
		static const u16string ELSE;
		static const u16string ELSEIF;
		static const u16string FALSE;
		static const u16string FOR;
		static const u16string IF;
		static const u16string IMPORT;
		static const u16string IN;
		static const u16string NIL;
		static const u16string PACKAGE;
		static const u16string PROC;
		static const u16string REDO;
		static const u16string RETURN;
		static const u16string THEN;
		static const u16string TRUE;
		static const u16string WHEN;
		static const u16string WHILE;

		// Unicode class Zs
//		static const char16_t SPACE = 0x0020u;
		static const char16_t NO_BREAK_SPACE = 0x00A0u;
		static const char16_t OGHAM_SPACE_MARK = 0x1680u;
		static const char16_t MONGOLIAN_VOWEL_SEPARATOR = 0x180Eu;
		static const char16_t EN_QUAD = 0x2000u;
//		static const char16_t EM_QUAD = 0x2001u;
//		static const char16_t EN_SPACE = 0x2002u;
//		static const char16_t EM_SPACE = 0x2003u;
//		static const char16_t THREE_PER_EM_SPACE = 0x2004u;
//		static const char16_t FOUR_PER_EM_SPACE = 0x2005u;
//		static const char16_t SIX_PER_EM_SPACE = 0x2006u;
//		static const char16_t FIGURE_SPACE = 0x2007u;
//		static const char16_t PUNCTUATION_SPACE = 0x2008u;
//		static const char16_t THIN_SPACE = 0x2009u;
		static const char16_t HAIR_SPACE = 0x200Au;
		static const char16_t NARROW_NO_BREAK_SPACE = 0x202Fu;
		static const char16_t MEDIUM_MATHEMATICAL_SPACE = 0x205Fu;
		static const char16_t IDEOGRAPHIC_SPACE = 0x3000u;

		static void Warning( const WarningCode warningCode, const Location location )
		{
			using std::cerr;

			constexpr char WARNING[] = "Warning";

			switch ( warningCode )
			{
			case WarningCode::UNKNOWN_ESCAPE_SEQUENCE:
				cerr << WARNING << ' ' << static_cast<unsigned int>( warningCode ) << ' ' << location << ' ' << "Unknown escape sequence.";
				break;
				
			default:
				cerr << WARNING << ' ' << static_cast<unsigned int>( warningCode );
				break;
			}
		}

		static void Error( const ErrorCode errorCode, const Location location )
		{
			using std::cerr;

			constexpr char ERROR[] = "Error";

			switch ( errorCode )
			{
			case ErrorCode::WRONG_CHARACTER:
				cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Wrong character.";
				break;

			case ErrorCode::STRING_NOT_TERMINATED:
				cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "String not terminated.";
				break;
				
			default:
				cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode );
				break;
			}
		}
	};
};

#endif