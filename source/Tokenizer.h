#include <iostream>
#include <string>
#include <forward_list>

#include "TextLoader.h"
#include "Token.h"
#include "Location.h"
#include "WarningCode.h"
#include "ErrorCode.h"

#ifndef TOKENIZER
#define TOKENIZER

namespace lyrics
{
	using std::u16string;
	using std::forward_list;

	class Tokenizer
	{
	public:
		Tokenizer( const char * const fileName ) : mCurrentLocation( fileName )
		{
		}

		bool Tokenize( forward_list<Token> &token )
		{
			if ( !TextLoader::LoadText( mCurrentLocation.GetFileName(), mText, mLength ) )
			{
				// TODO:
				return false;
			}

			mOffset = 0;

			mLastToken = token.cend();

			while ( Scan( token ) );

			delete [] mText;

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
		static const u16string IF;
		static const u16string IMPORT;
		static const u16string IN;
		static const u16string NIL;
		static const u16string OUT;
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

		char16_t *mText;
		unsigned int mLength;
		unsigned int mOffset;

		Token::Type mType;
		Token::Value mValue;
		Location mCurrentLocation;
		unsigned int mLine;
		unsigned int mColumn;

		forward_list<Token>::const_iterator mLastToken;

		// TODO: mLength is byte of mText, not number of characters.
		bool Scan( forward_list<Token> &token )
		{
			char16_t tChar;

			if ( mOffset < mLength )
			{
				tChar = mText[mOffset];
			}
			else
			{
				return false;
			}

			if ( ( u'A' <= tChar && tChar <= u'Z' ) || ( u'a' <= tChar && tChar <= u'z' ) || tChar == u'_' )
			{
				u16string *tStr = new u16string();
				unsigned int length;
				bool isIdentifier = false;

				do
				{
					tStr->push_back( tChar );
					
					if ( ++mOffset < mLength )
					{
						tChar = mText[mOffset];
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
					mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, mCurrentLocation );
				}
				else
				{
					if ( 2 > length || 7 < length )	// If the token is shorter than 2 or longer than 7, then the token is itentifier.
					{
						mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, mCurrentLocation );
					}
					else
					{
						switch ( tStr->length() )
						{
						case 2:
							if ( *tStr == Tokenizer::IF )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::IF, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::DO )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::DO, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::IN )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::IN, mCurrentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, mCurrentLocation );
							}
							break;

						case 3:
							if ( *tStr == Tokenizer::END )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::END, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::FOR )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::FOR, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::OUT )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::OUT, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::NIL )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::NIL_LITERAL, mCurrentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, mCurrentLocation );
							}
							break;

						case 4:
							if ( *tStr == Tokenizer::ELSE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::ELSE, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::PROC )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::PROC, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::TRUE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::BOOLEAN_LITERAL, true, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::CASE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::CASE, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::WHEN )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::WHEN, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::REDO )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::REDO, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::THEN )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::THEN, mCurrentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, mCurrentLocation );
							}
							break;

						case 5:
							if ( *tStr == Tokenizer::WHILE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::WHILE, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::BREAK )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::BREAK, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::FALSE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::BOOLEAN_LITERAL, false, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::CLASS )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::CLASS, mCurrentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, mCurrentLocation );
							}
							break;

						case 6:
							if ( *tStr == Tokenizer::ELSEIF )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::ELSEIF, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::IMPORT )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::IMPORT, mCurrentLocation );
							}
							else if ( *tStr == Tokenizer::RETURN )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::RETURN, mCurrentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, mCurrentLocation );
							}
							break;

						case 7:
							if ( *tStr == Tokenizer::PACKAGE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::PACKAGE, mCurrentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, mCurrentLocation );
							}
							break;
						}
					}
				}

				mCurrentLocation.IncreaseColumn( length );
			}
			else if ( tChar == u' ' || tChar == u'\t' || tChar == u'\r' || tChar == u'\v' || tChar == u'\f' )
			{
				mCurrentLocation.IncreaseColumn();
				mOffset++;

				Scan( token );
			}
			else if ( tChar == u'\n' )
			{
				mCurrentLocation.IncreaseLine();
				mOffset++;

				Scan( token );
			}
			else if ( tChar == u'.' )
			{
				unsigned int length = 1;

				if ( ++mOffset < mLength )
				{
					tChar = mText[mOffset];

					if ( tChar < u'0' || tChar > u'9' )
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'.' ), mCurrentLocation );
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

							length++;
							
							if ( ++mOffset < mLength )
							{
								tChar = mText[mOffset];
							}
							else
							{
								break;
							}
						}
						while ( u'0' <= tChar && tChar <= u'9' );

						mLastToken = token.emplace_after( mLastToken, Token::Type::REAL_LITERAL, double( valueBelowDecimalPoint ) / tenPowerDecimalPlace, mCurrentLocation );
					}

					mCurrentLocation.IncreaseColumn( length );
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'.' ), mCurrentLocation );
					mCurrentLocation.IncreaseColumn();
				}
			}
			else if ( tChar == u'(' || tChar == u')' || tChar == u'+' || tChar == u'-' || tChar == u'*' || tChar == u'/' || tChar == u'[' || tChar == u']' )
			{
				mOffset++;
				mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( tChar ), mCurrentLocation );
				mCurrentLocation.IncreaseColumn();
			}
			else if ( tChar == u'<' )
			{
				if ( ++mOffset < mLength )
				{
					switch ( mText[mOffset] )
					{
					case u'<':
						mLastToken = token.emplace_after( mLastToken, Token::Type::SHIFT_LEFT, mCurrentLocation );
						mCurrentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;

					case u'=':
						mLastToken = token.emplace_after( mLastToken, Token::Type::LESS_THAN_OR_EQUAL, mCurrentLocation );
						mCurrentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;
					
					default:
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'<' ), mCurrentLocation );
						mCurrentLocation.IncreaseColumn();
						break;
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'<' ), mCurrentLocation );
					mCurrentLocation.IncreaseColumn();
				}
			}
			else if ( tChar == u'>' )
			{
				if ( ++mOffset < mLength )
				{
					switch ( mText[mOffset] )
					{
					case u'>':
						mLastToken = token.emplace_after( mLastToken, Token::Type::SHIFT_RIGHT, mCurrentLocation );
						mCurrentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;

					case u'=':
						mLastToken = token.emplace_after( mLastToken, Token::Type::GREATER_THAN_OR_EQUAL, mCurrentLocation );
						mCurrentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;
					
					default:
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'>' ), mCurrentLocation );
						mCurrentLocation.IncreaseColumn();
						break;
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'>' ), mCurrentLocation );
					mCurrentLocation.IncreaseColumn();
				}
			}
			else if ( tChar == u'=' )
			{
				if ( ++mOffset < mLength )
				{
					if ( mText[mOffset] == u'=' )
					{
						mLastToken = token.emplace_after( mLastToken, Token::Type::EQUAL, mCurrentLocation );
						mCurrentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'=' ), mCurrentLocation );
						mCurrentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'=' ), mCurrentLocation );
					mCurrentLocation.IncreaseColumn();
				}
			}
			else if ( tChar == u'!' )
			{
				if ( ++mOffset < mLength )
				{
					if ( mText[mOffset] == u'=' )
					{
						mLastToken = token.emplace_after( mLastToken, Token::Type::NOT_EQUAL, mCurrentLocation );
						mCurrentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'!' ), mCurrentLocation );
						mCurrentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'!' ), mCurrentLocation );
					mCurrentLocation.IncreaseColumn();
				}
			}
			else if ( tChar == u'&' )
			{
				if ( ++mOffset < mLength )
				{
					if ( mText[mOffset] == u'&' )
					{
						mLastToken = token.emplace_after( mLastToken, Token::Type::AND, mCurrentLocation );
						mCurrentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'&' ), mCurrentLocation );
						mCurrentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'&' ), mCurrentLocation );
					mCurrentLocation.IncreaseColumn();
				}
			}
			else if ( tChar == u'|' )
			{
				if ( ++mOffset < mLength )
				{
					if ( mText[mOffset] == u'|' )
					{
						mLastToken = token.emplace_after( mLastToken, Token::Type::OR, mCurrentLocation );
						mCurrentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'|' ), mCurrentLocation );
						mCurrentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'|' ), mCurrentLocation );
					mCurrentLocation.IncreaseColumn();
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
					
					if ( ++mOffset < mLength )
					{
						tChar = mText[mOffset];
					}
					else
					{
						break;
					}
				}
				while ( u'0' <= tChar && tChar <= u'9' );

				if ( mOffset >= mLength || tChar != u'.' )
				{
					mLastToken = token.emplace_after( mLastToken, Token::Type::INTEGER_LITERAL, integer, mCurrentLocation );
					mCurrentLocation.IncreaseColumn( length );
				}
				else
				{
					length++;

					if ( ++mOffset < mLength )
					{
						tChar = mText[mOffset];

						if ( u'0' <= tChar && tChar <= u'9' )
						{
							do
							{
								valueBelowDecimalPoint = ( valueBelowDecimalPoint << 3 ) + ( valueBelowDecimalPoint << 1 );
								valueBelowDecimalPoint += tChar & 15;
								tenPowerDecimalPlace = ( tenPowerDecimalPlace << 3 ) + ( tenPowerDecimalPlace << 1 );

								length++;
								
								if ( ++mOffset < mLength )
								{
									tChar = mText[mOffset];
								}
								else
								{
									break;
								}
							}
							while ( u'0' <= tChar && tChar <= u'9' );
						}
					}

					mLastToken = token.emplace_after( mLastToken, Token::Type::REAL_LITERAL, integer + double( valueBelowDecimalPoint ) / tenPowerDecimalPlace, mCurrentLocation );
					mCurrentLocation.IncreaseColumn( length );
				}
			}
			else if ( tChar == u'\"' )
			{
				u16string *tStr;

				unsigned int lineFeed = 0;
				unsigned int length = 0;

				if ( ++mOffset < mLength )
				{
					tChar = mText[mOffset];
				}
				else
				{
					Error( ErrorCode::STRING_NOT_TERMINATED, mCurrentLocation );
					return true;
				}

				tStr = new u16string();

				length++;
				do
				{
					if ( tChar == '\\' )
					{
						if ( ++mOffset < mLength )
						{
							tChar = mText[mOffset];
						}
						else
						{
							Error( ErrorCode::STRING_NOT_TERMINATED, mCurrentLocation );
							return true;
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
							if ( ++mOffset < mLength )
							{
								tChar = mText[mOffset];
							}
							else
							{
								Error( ErrorCode::STRING_NOT_TERMINATED, mCurrentLocation );
								return true;
							}

							if ( tChar == '\n' )
							{
								lineFeed++;
								length = 0;
							}
							else
							{
								Warning( WarningCode::UNKNOWN_ESCAPE_SEQUENCE, mCurrentLocation );
								tStr->push_back( tChar );
								length += 3;
							}
							break;
						
						default:
							Warning( WarningCode::UNKNOWN_ESCAPE_SEQUENCE, mCurrentLocation );
							break;
						}
					}
					else if ( tChar == '\r' || tChar == '\n' )
					{
						Error( ErrorCode::STRING_NOT_TERMINATED, mCurrentLocation );
						return true;
					}
					else
					{
						tStr->push_back( tChar );
						length++;
					}
					
					if ( ++mOffset < mLength )
					{
						tChar = mText[mOffset];
					}
					else
					{
						Error( ErrorCode::STRING_NOT_TERMINATED, mCurrentLocation );
						return true;
					}
				}
				while ( tChar != '\"' );
				length++;

				mLastToken = token.emplace_after( mLastToken, Token::Type::STRING_LITERAL, tStr, mCurrentLocation );

				while ( lineFeed-- )
				{
					mCurrentLocation.IncreaseLine();
				}
				mCurrentLocation.IncreaseColumn( length );
			}
			else if ( tChar == u'#' )
			{
				do
				{
					if ( ++mOffset < mLength )
					{
						tChar = mText[mOffset];
					}
					else
					{
						return false;
					}
				}
				while ( tChar != u'\r' && tChar != u'\n' );

				Scan( token );
			}
			else if ( tChar == u'{' || tChar == u'}' || tChar == u',' || tChar == u':' || tChar == u'~' || tChar == u'%' || tChar == u'^' || tChar == u'?' )
			{
				mOffset++;

				mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( tChar ), mCurrentLocation );
			}
			else if ( tChar == Tokenizer::NO_BREAK_SPACE || tChar == Tokenizer::OGHAM_SPACE_MARK || tChar == Tokenizer::MONGOLIAN_VOWEL_SEPARATOR || ( tChar >= Tokenizer::EN_QUAD && tChar <= Tokenizer::HAIR_SPACE ) || tChar == Tokenizer::NARROW_NO_BREAK_SPACE || tChar == Tokenizer::MEDIUM_MATHEMATICAL_SPACE || tChar == Tokenizer::IDEOGRAPHIC_SPACE )	// Unicode class Zs except space character.
			{
				mOffset++;

				Scan( token );
			}
			else
			{
				Error( ErrorCode::WRONG_CHARACTER, mCurrentLocation );
			}

			return true;
		}

		static void Warning( const WarningCode warningCode, const Location location )
		{
			using std::cout;
			using std::cerr;

			constexpr char WARNING[] = "Warning";

			switch ( warningCode )
			{
			case WarningCode::UNKNOWN_ESCAPE_SEQUENCE:
				cout << WARNING << ' ' << static_cast<unsigned int>( warningCode ) << ' ' << location << ' ' << "Unknown escape sequence.";
				break;
				
			default:
				cerr << WARNING << ' ' << static_cast<unsigned int>( warningCode );
				break;
			}
		}

		static void Error( const ErrorCode errorCode, const Location location )
		{
			using std::cout;
			using std::cerr;

			constexpr char ERROR[] = "Error";

			switch ( errorCode )
			{
			case ErrorCode::WRONG_CHARACTER:
				cout << ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Wrong character.";
				break;

			case ErrorCode::STRING_NOT_TERMINATED:
				cout << ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "String not terminated.";
				break;
				
			default:
				cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode );
				break;
			}
		}
	};
};

#endif