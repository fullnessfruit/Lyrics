#include <string>
#include <forward_list>

#include "Token.h"
#include "Location.h"
#include "TextLoader.h"

#include "WarningCode.h"
#include "ErrorCode.h"
#include "FatalErrorCode.h"
#include "ErrorHandler.h"

#ifndef TOKENIZER
#define TOKENIZER

namespace lyrics
{
	using std::string;
	using std::u16string;
	using std::forward_list;

	class Tokenizer
	{
	private:
		Tokenizer() = delete;

	public:
		static bool Tokenize( const string fileName, forward_list<Token> &tokenList )
		{
			if ( !TextLoader().LoadText( fileName, Tokenizer::mText, Tokenizer::mTextLength ) )
			{
				ErrorHandler::FatalError( FatalErrorCode::NO_SUCH_FILE );
				return false;
			}

			Location currentLocation( fileName );

			Tokenizer::mOffset = 0;

			Tokenizer::mLastToken = tokenList.cbefore_begin();

			while ( Tokenizer::Scan( tokenList, currentLocation ) );
			Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::END_OF_FILE, currentLocation );

			delete [] Tokenizer::mText;

			return true;
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
		static const u16string FOREACH;
		static const u16string IF;
		static const u16string IMPORT;
		static const u16string IN;
		static const u16string INCLUDE;
		static const u16string NEXT;
		static const u16string NULL_TOKEN;
		static const u16string OUT;
		static const u16string PACKAGE;
		static const u16string PRIVATE;
		static const u16string PUBLIC;
		static const u16string RETURN;
		static const u16string ROUTINE;
		static const u16string THEN;
		static const u16string THIS;
		static const u16string TRUE;
		static const u16string WHEN;
		static const u16string WHILE;

		// Unicode class Zs
//		static constexpr char16_t SPACE = 0x0020u;
		static constexpr char16_t NO_BREAK_SPACE = 0x00A0u;
		static constexpr char16_t OGHAM_SPACE_MARK = 0x1680u;
		static constexpr char16_t MONGOLIAN_VOWEL_SEPARATOR = 0x180Eu;
		static constexpr char16_t EN_QUAD = 0x2000u;
//		static constexpr char16_t EM_QUAD = 0x2001u;
//		static constexpr char16_t EN_SPACE = 0x2002u;
//		static constexpr char16_t EM_SPACE = 0x2003u;
//		static constexpr char16_t THREE_PER_EM_SPACE = 0x2004u;
//		static constexpr char16_t FOUR_PER_EM_SPACE = 0x2005u;
//		static constexpr char16_t SIX_PER_EM_SPACE = 0x2006u;
//		static constexpr char16_t FIGURE_SPACE = 0x2007u;
//		static constexpr char16_t PUNCTUATION_SPACE = 0x2008u;
//		static constexpr char16_t THIN_SPACE = 0x2009u;
		static constexpr char16_t HAIR_SPACE = 0x200Au;
		static constexpr char16_t NARROW_NO_BREAK_SPACE = 0x202Fu;
		static constexpr char16_t MEDIUM_MATHEMATICAL_SPACE = 0x205Fu;
		static constexpr char16_t IDEOGRAPHIC_SPACE = 0x3000u;

		// New line character
//		static constexpr char16_t CARRIAGE_RETURN = 0x000Du;
//		static constexpr char16_t LINE_FEED = 0x000Au;
		static constexpr char16_t NEXT_LINE = 0x0085u;
		static constexpr char16_t LINE_SEPARATOR = 0x2028u;
		static constexpr char16_t PARAGRAPH_SEPARATOR = 0x2029u;

		static char16_t *mText;
		static unsigned int mTextLength;
		static unsigned int mOffset;

		static forward_list<Token>::const_iterator mLastToken;

		// TODO: Tokenizer::mTextLength is byte of Tokenizer::mText, not number of characters.
		static bool Scan( forward_list<Token> &tokenList, Location &currentLocation )
		{
			char16_t tChar;

			if ( Tokenizer::mOffset < Tokenizer::mTextLength )
			{
				tChar = Tokenizer::mText[Tokenizer::mOffset];
			}
			else
			{
				return false;
			}

			if ( ( u'A' <= tChar && tChar <= u'Z' ) || ( u'a' <= tChar && tChar <= u'z' ) || tChar == u'_' )
			{
				u16string *tStr = new u16string();
				unsigned int length;
				bool isIdentifier = tChar == u'_' ? true : false;

				do
				{
					tStr->push_back( tChar );
					
					if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
					{
						tChar = Tokenizer::mText[Tokenizer::mOffset];
					}
					else
					{
						break;
					}
				}
				while ( ( u'A' <= tChar && tChar <= u'Z' ) || ( u'a' <= tChar && tChar <= u'z' ) || ( ( u'0' <= tChar && tChar <= u'9' ) || tChar == u'_' ? isIdentifier = true : false ) );	// If the string includs number or _, then the string is identifier.

				length = tStr->length();

				if ( isIdentifier )
				{
					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
				}
				else
				{
					if ( 2u > length || 7u < length )	// If the token is shorter than 2 or longer than 7, then the token is identifier.
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
					}
					else
					{
						switch ( tStr->length() )
						{
						case 2u:
							if ( *tStr == Tokenizer::IF )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IF, currentLocation );
							}
							else if ( *tStr == Tokenizer::DO )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::DO, currentLocation );
							}
							else if ( *tStr == Tokenizer::IN )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IN, currentLocation );
							}
							else
							{
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 3u:
							if ( *tStr == Tokenizer::END )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::END, currentLocation );
							}
							else if ( *tStr == Tokenizer::FOR )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::FOR, currentLocation );
							}
							else if ( *tStr == Tokenizer::OUT )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::OUT, currentLocation );
							}
							else
							{
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 4u:
							if ( *tStr == Tokenizer::ELSE )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::ELSE, currentLocation );
							}
							else if ( *tStr == Tokenizer::NULL_TOKEN )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::NULL_LITERAL, currentLocation );
							}
							else if ( *tStr == Tokenizer::TRUE )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, true, currentLocation );
							}
							else if ( *tStr == Tokenizer::CASE )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::CASE, currentLocation );
							}
							else if ( *tStr == Tokenizer::WHEN )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::WHEN, currentLocation );
							}
							else if ( *tStr == Tokenizer::THIS )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::THIS, currentLocation );
							}
							else if ( *tStr == Tokenizer::NEXT )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::NEXT, currentLocation );
							}
							else if ( *tStr == Tokenizer::THEN )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::THEN, currentLocation );
							}
							else
							{
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 5u:
							if ( *tStr == Tokenizer::WHILE )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::WHILE, currentLocation );
							}
							else if ( *tStr == Tokenizer::BREAK )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::BREAK, currentLocation );
							}
							else if ( *tStr == Tokenizer::FALSE )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, false, currentLocation );
							}
							else if ( *tStr == Tokenizer::CLASS )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::CLASS, currentLocation );
							}
							else
							{
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 6u:
							if ( *tStr == Tokenizer::ELSEIF )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::ELSEIF, currentLocation );
							}
							else if ( *tStr == Tokenizer::IMPORT )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IMPORT, currentLocation );
							}
							else if ( *tStr == Tokenizer::RETURN )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::RETURN, currentLocation );
							}
							else if ( *tStr == Tokenizer::PUBLIC )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::PUBLIC, currentLocation );
							}
							else
							{
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 7u:
							if ( *tStr == Tokenizer::ROUTINE )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::ROUTINE, currentLocation );
							}
							else if ( *tStr == Tokenizer::PRIVATE )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::PRIVATE, currentLocation );
							}
							else if ( *tStr == Tokenizer::PACKAGE )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::PACKAGE, currentLocation );
							}
							else if ( *tStr == Tokenizer::FOREACH )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::FOREACH, currentLocation );
							}
							else if ( *tStr == Tokenizer::INCLUDE )
							{
								delete tStr;
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::INCLUDE, currentLocation );
							}
							else
							{
								Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;
						}
					}
				}

				currentLocation.IncreaseColumn( length );
			}
			else if ( tChar == u' ' || tChar == u'\t' || tChar == u'\r' || tChar == u'\v' || tChar == u'\f' )
			{
				currentLocation.IncreaseColumn();
				Tokenizer::mOffset++;

				Tokenizer::Scan( tokenList, currentLocation );
			}
			else if ( tChar == u'\n' )
			{
				currentLocation.IncreaseLine();
				Tokenizer::mOffset++;

				Tokenizer::Scan( tokenList, currentLocation );
			}
			else if ( tChar == u'.' )
			{
				if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
				{
					tChar = Tokenizer::mText[Tokenizer::mOffset];

					if ( tChar < u'0' || tChar > u'9' )
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'.' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
					else	// real
					{
						long long valueBelowDecimalPoint = 0;
						long long tenPowerDecimalPlace = 1;

						unsigned int length = 1;

						do
						{
							valueBelowDecimalPoint = ( valueBelowDecimalPoint << 3 ) + ( valueBelowDecimalPoint << 1 );
							valueBelowDecimalPoint += tChar & 15;
							tenPowerDecimalPlace = ( tenPowerDecimalPlace << 3 ) + ( tenPowerDecimalPlace << 1 );

							length++;
							
							if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
							{
								tChar = Tokenizer::mText[Tokenizer::mOffset];
							}
							else
							{
								break;
							}
						}
						while ( u'0' <= tChar && tChar <= u'9' );

						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, double( valueBelowDecimalPoint ) / tenPowerDecimalPlace, currentLocation );
						currentLocation.IncreaseColumn( length );
					}
				}
				else
				{
					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'.' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'(' || tChar == u')' || tChar == u'+' || tChar == u'-' || tChar == u'*' || tChar == u'/' || tChar == u'[' || tChar == u']' )
			{
				Tokenizer::mOffset++;
				Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( tChar ), currentLocation );
				currentLocation.IncreaseColumn();
			}
			else if ( tChar == u'<' )
			{
				if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
				{
					switch ( Tokenizer::mText[Tokenizer::mOffset] )
					{
					case u'<':
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::SHIFT_LEFT, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						Tokenizer::mOffset++;
						break;

					case u'=':
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::LESS_THAN_OR_EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						Tokenizer::mOffset++;
						break;
					
					default:
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'<' ), currentLocation );
						currentLocation.IncreaseColumn();
						break;
					}
				}
				else
				{
					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'<' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'>' )
			{
				if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
				{
					switch ( Tokenizer::mText[Tokenizer::mOffset] )
					{
					case u'>':
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::SHIFT_RIGHT, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						Tokenizer::mOffset++;
						break;

					case u'=':
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::GREATER_THAN_OR_EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						Tokenizer::mOffset++;
						break;
					
					default:
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'>' ), currentLocation );
						currentLocation.IncreaseColumn();
						break;
					}
				}
				else
				{
					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'>' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'=' )
			{
				if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
				{
					if ( Tokenizer::mText[Tokenizer::mOffset] == u'=' )
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						Tokenizer::mOffset++;
					}
					else
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'=' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'=' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'!' )
			{
				if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
				{
					if ( Tokenizer::mText[Tokenizer::mOffset] == u'=' )
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::NOT_EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						Tokenizer::mOffset++;
					}
					else
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'!' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'!' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'&' )
			{
				if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
				{
					if ( Tokenizer::mText[Tokenizer::mOffset] == u'&' )
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::AND, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						Tokenizer::mOffset++;
					}
					else
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'&' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'&' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'|' )
			{
				if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
				{
					if ( Tokenizer::mText[Tokenizer::mOffset] == u'|' )
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, Token::Type::OR, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						Tokenizer::mOffset++;
					}
					else
					{
						Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'|' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( u'|' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( u'0' <= tChar && tChar <= u'9' )
			{
				long long integer = 0;
				long long valueBelowDecimalPoint = 0;
				long long tenPowerDecimalPlace = 1;

				unsigned int length = 0;

				do
				{
					integer = ( integer << 3 ) + ( integer << 1 );
					integer += tChar & 15;

					length++;
					
					if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
					{
						tChar = Tokenizer::mText[Tokenizer::mOffset];
					}
					else
					{
						break;
					}
				}
				while ( u'0' <= tChar && tChar <= u'9' );

				if ( Tokenizer::mOffset >= Tokenizer::mTextLength || tChar != u'.' )
				{
					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, integer, currentLocation );
					currentLocation.IncreaseColumn( length );
				}
				else
				{
					length++;

					if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
					{
						tChar = Tokenizer::mText[Tokenizer::mOffset];

						if ( u'0' <= tChar && tChar <= u'9' )
						{
							do
							{
								valueBelowDecimalPoint = ( valueBelowDecimalPoint << 3 ) + ( valueBelowDecimalPoint << 1 );
								valueBelowDecimalPoint += tChar & 15;
								tenPowerDecimalPlace = ( tenPowerDecimalPlace << 3 ) + ( tenPowerDecimalPlace << 1 );

								length++;
								
								if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
								{
									tChar = Tokenizer::mText[Tokenizer::mOffset];
								}
								else
								{
									break;
								}
							}
							while ( u'0' <= tChar && tChar <= u'9' );
						}
					}

					Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, integer + double( valueBelowDecimalPoint ) / tenPowerDecimalPlace, currentLocation );
					currentLocation.IncreaseColumn( length );
				}
			}
			else if ( tChar == u'\"' )
			{
				u16string *tStr;

				unsigned int lineFeed = 0;
				unsigned int length = 0;

				if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
				{
					tChar = Tokenizer::mText[Tokenizer::mOffset];
				}
				else
				{
					ErrorHandler::Error( currentLocation, ErrorCode::STRING_NOT_TERMINATED );

					return false;
				}

				tStr = new u16string();

				length++;
				do
				{
					if ( tChar == '\\' )
					{
						if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
						{
							tChar = Tokenizer::mText[Tokenizer::mOffset];
						}
						else
						{
							ErrorHandler::Error( currentLocation, ErrorCode::STRING_NOT_TERMINATED );
							delete tStr;

							return false;
						}

						switch ( tChar )
						{
						case u'n':
							tStr->push_back( u'\n' );
							length += 2;
							break;

						case u't':
							tStr->push_back( u'\t' );
							length += 2;
							break;

						case u'0':
							tStr->push_back( u'\0' );
							length += 2;
							break;

						case u'r':
							tStr->push_back( u'\r' );
							length += 2;
							break;

						case u'\'':
							tStr->push_back( u'\'' );
							length += 2;
							break;

						case u'\"':
							tStr->push_back( u'\"' );
							length += 2;
							break;

						case u'\\':
							tStr->push_back( u'\\' );
							length += 2;
							break;

						case u'a':
							tStr->push_back( u'\a' );
							length += 2;
							break;

						case u'b':
							tStr->push_back( u'\b' );
							length += 2;
							break;

						case u'f':
							tStr->push_back( u'\f' );
							length += 2;
							break;

						case u'v':
							tStr->push_back( u'\v' );
							length += 2;
							break;

						case u'\n':
							lineFeed++;
							length = 0;
							break;

						case u'\r':
							if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
							{
								tChar = Tokenizer::mText[Tokenizer::mOffset];
							}
							else
							{
								ErrorHandler::Error( currentLocation, ErrorCode::STRING_NOT_TERMINATED );
								delete tStr;

								return false;
							}

							if ( tChar == '\n' )
							{
								lineFeed++;
								length = 0;
							}
							else
							{
								ErrorHandler::Warning( currentLocation, WarningCode::UNKNOWN_ESCAPE_SEQUENCE );
								tStr->push_back( tChar );
								length += 3;
							}
							break;
						
						default:
							ErrorHandler::Warning( currentLocation, WarningCode::UNKNOWN_ESCAPE_SEQUENCE );
							break;
						}
					}
					else if ( tChar == '\r' || tChar == '\n' )
					{
						ErrorHandler::Error( currentLocation, ErrorCode::STRING_NOT_TERMINATED );
						delete tStr;

						return true;
					}
					else
					{
						tStr->push_back( tChar );
						length++;
					}
					
					if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
					{
						tChar = Tokenizer::mText[Tokenizer::mOffset];
					}
					else
					{
						ErrorHandler::Error( currentLocation, ErrorCode::STRING_NOT_TERMINATED );
						delete tStr;

						return false;
					}
				}
				while ( tChar != '\"' );
				Tokenizer::mOffset++;
				length++;

				Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, tStr, currentLocation );

				while ( lineFeed-- )
				{
					currentLocation.IncreaseLine();
				}
				currentLocation.IncreaseColumn( length );
			}
			else if ( tChar == u'#' )
			{
				do
				{
					if ( ++Tokenizer::mOffset < Tokenizer::mTextLength )
					{
						tChar = Tokenizer::mText[Tokenizer::mOffset];
					}
					else
					{
						return false;
					}
				}
				while ( tChar != u'\r' && tChar != u'\n' );

				Tokenizer::Scan( tokenList, currentLocation );
			}
			else if ( tChar == u'{' || tChar == u'}' || tChar == u',' || tChar == u':' || tChar == u'~' || tChar == u'%' || tChar == u'^' )
			{
				Tokenizer::mOffset++;

				Tokenizer::mLastToken = tokenList.emplace_after( Tokenizer::mLastToken, static_cast<Token::Type>( tChar ), currentLocation );
				currentLocation.IncreaseColumn();
			}
			else if ( tChar == Tokenizer::NO_BREAK_SPACE || tChar == Tokenizer::OGHAM_SPACE_MARK || tChar == Tokenizer::MONGOLIAN_VOWEL_SEPARATOR || ( tChar >= Tokenizer::EN_QUAD && tChar <= Tokenizer::HAIR_SPACE ) || tChar == Tokenizer::NARROW_NO_BREAK_SPACE || tChar == Tokenizer::MEDIUM_MATHEMATICAL_SPACE || tChar == Tokenizer::IDEOGRAPHIC_SPACE )	// Unicode class Zs except space character.
			{
				Tokenizer::mOffset++;

				Tokenizer::Scan( tokenList, currentLocation );
			}
			else if ( tChar == Tokenizer::NEXT_LINE || tChar == Tokenizer::LINE_SEPARATOR || tChar == Tokenizer::PARAGRAPH_SEPARATOR )	// New line character.
			{
				Tokenizer::mOffset++;
				currentLocation.IncreaseLine();

				Tokenizer::Scan( tokenList, currentLocation );
			}
			else
			{
				ErrorHandler::Error( currentLocation, ErrorCode::WRONG_CHARACTER );

				Tokenizer::mOffset++;
				currentLocation.IncreaseColumn();
			}

			return true;
		}
	};
}

#endif
