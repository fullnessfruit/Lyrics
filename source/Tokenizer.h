#include <string>
#include <forward_list>
#include <new>

#include "Token.h"
#include "Location.h"
#include "WarningCode.h"
#include "ErrorCode.h"
#include "ErrorHandler.h"
#include "FatalErrorCode.h"
#include "TextLoader.h"

#ifndef TOKENIZER
#define TOKENIZER

namespace lyrics
{
	using std::string;
	using std::u16string;
	using std::forward_list;
	using std::bad_alloc;

	class Tokenizer
	{
	public:
		bool Tokenize( const string &fileName, forward_list<Token> &tokenList )
		{
			mText = TextLoader().Load( fileName, mTextLength );

			Location currentLocation( fileName );

			mOffset = 0;

			mLastToken = tokenList.cbefore_begin();

			try
			{
				while ( TokenizeUTF_16( tokenList, currentLocation ) );
				mLastToken = tokenList.emplace_after( mLastToken, Token::Type::END_OF_FILE, currentLocation );
			}
			catch ( const bad_alloc &e )
			{
				Utility::SafeArrayDelete( mText );
				throw FatalErrorCode::NOT_ENOUGH_MEMORY;
			}

			Utility::SafeArrayDelete( mText );

			return true;
		}

		// TODO: mTextLength is byte of mText, not number of characters.
		bool TokenizeUTF_16( forward_list<Token> &tokenList, Location &currentLocation )
		{
			char16_t tChar;

			if ( mOffset < mTextLength )
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
				bool isIdentifier = tChar == u'_' ? true : false;

				do
				{
					tStr->push_back( tChar );
					
					if ( ++mOffset < mTextLength )
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
					mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
				}
				else
				{
					if ( 2u > length || 7u < length )	// If the token is shorter than 2 or longer than 7, then the token is identifier.
					{
						mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
					}
					else
					{
						switch ( tStr->length() )
						{
						case 2u:
							if ( *tStr == Tokenizer::IF )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IF, currentLocation );
							}
							else if ( *tStr == Tokenizer::DO )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::DO, currentLocation );
							}
							else if ( *tStr == Tokenizer::IN )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IN, currentLocation );
							}
							else
							{
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 3u:
							if ( *tStr == Tokenizer::END )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::END, currentLocation );
							}
							else if ( *tStr == Tokenizer::FOR )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::FOR, currentLocation );
							}
							else if ( *tStr == Tokenizer::OUT )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::OUT, currentLocation );
							}
							else
							{
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 4u:
							if ( *tStr == Tokenizer::ELSE )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::ELSE, currentLocation );
							}
							else if ( *tStr == Tokenizer::NULL_TOKEN )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::NULL_LITERAL, currentLocation );
							}
							else if ( *tStr == Tokenizer::TRUE )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, true, currentLocation );
							}
							else if ( *tStr == Tokenizer::CASE )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::CASE, currentLocation );
							}
							else if ( *tStr == Tokenizer::WHEN )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::WHEN, currentLocation );
							}
							else if ( *tStr == Tokenizer::THIS )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::THIS, currentLocation );
							}
							else if ( *tStr == Tokenizer::NEXT )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::NEXT, currentLocation );
							}
							else if ( *tStr == Tokenizer::THEN )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::THEN, currentLocation );
							}
							else
							{
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 5u:
							if ( *tStr == Tokenizer::WHILE )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::WHILE, currentLocation );
							}
							else if ( *tStr == Tokenizer::BREAK )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::BREAK, currentLocation );
							}
							else if ( *tStr == Tokenizer::FALSE )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, false, currentLocation );
							}
							else if ( *tStr == Tokenizer::CLASS )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::CLASS, currentLocation );
							}
							else
							{
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 6u:
							if ( *tStr == Tokenizer::ELSEIF )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::ELSEIF, currentLocation );
							}
							else if ( *tStr == Tokenizer::IMPORT )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IMPORT, currentLocation );
							}
							else if ( *tStr == Tokenizer::RETURN )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::RETURN, currentLocation );
							}
							else if ( *tStr == Tokenizer::PUBLIC )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::PUBLIC, currentLocation );
							}
							else
							{
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 7u:
							if ( *tStr == Tokenizer::ROUTINE )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::ROUTINE, currentLocation );
							}
							else if ( *tStr == Tokenizer::PRIVATE )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::PRIVATE, currentLocation );
							}
							else if ( *tStr == Tokenizer::PACKAGE )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::PACKAGE, currentLocation );
							}
							else if ( *tStr == Tokenizer::FOREACH )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::FOREACH, currentLocation );
							}
							else if ( *tStr == Tokenizer::INCLUDE )
							{
								delete tStr;
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::INCLUDE, currentLocation );
							}
							else
							{
								mLastToken = tokenList.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
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
				mOffset++;

				TokenizeUTF_16( tokenList, currentLocation );
			}
			else if ( tChar == u'\n' )
			{
				currentLocation.IncreaseLine();
				mOffset++;

				TokenizeUTF_16( tokenList, currentLocation );
			}
			else if ( tChar == u'.' )
			{
				if ( ++mOffset < mTextLength )
				{
					tChar = mText[mOffset];

					if ( tChar < u'0' || tChar > u'9' )
					{
						mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'.' ), currentLocation );
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
							
							if ( ++mOffset < mTextLength )
							{
								tChar = mText[mOffset];
							}
							else
							{
								break;
							}
						}
						while ( u'0' <= tChar && tChar <= u'9' );

						mLastToken = tokenList.emplace_after( mLastToken, double( valueBelowDecimalPoint ) / tenPowerDecimalPlace, currentLocation );
						currentLocation.IncreaseColumn( length );
					}
				}
				else
				{
					mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'.' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'(' || tChar == u')' || tChar == u'+' || tChar == u'-' || tChar == u'*' || tChar == u'/' || tChar == u'[' || tChar == u']' )
			{
				mOffset++;
				mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( tChar ), currentLocation );
				currentLocation.IncreaseColumn();
			}
			else if ( tChar == u'<' )
			{
				if ( ++mOffset < mTextLength )
				{
					switch ( mText[mOffset] )
					{
					case u'<':
						mLastToken = tokenList.emplace_after( mLastToken, Token::Type::SHIFT_LEFT, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;

					case u'=':
						mLastToken = tokenList.emplace_after( mLastToken, Token::Type::LESS_THAN_OR_EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;
					
					default:
						mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'<' ), currentLocation );
						currentLocation.IncreaseColumn();
						break;
					}
				}
				else
				{
					mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'<' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'>' )
			{
				if ( ++mOffset < mTextLength )
				{
					switch ( mText[mOffset] )
					{
					case u'>':
						mLastToken = tokenList.emplace_after( mLastToken, Token::Type::SHIFT_RIGHT, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;

					case u'=':
						mLastToken = tokenList.emplace_after( mLastToken, Token::Type::GREATER_THAN_OR_EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;
					
					default:
						mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'>' ), currentLocation );
						currentLocation.IncreaseColumn();
						break;
					}
				}
				else
				{
					mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'>' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'=' )
			{
				if ( ++mOffset < mTextLength )
				{
					if ( mText[mOffset] == u'=' )
					{
						mLastToken = tokenList.emplace_after( mLastToken, Token::Type::EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'=' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'=' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'!' )
			{
				if ( ++mOffset < mTextLength )
				{
					if ( mText[mOffset] == u'=' )
					{
						mLastToken = tokenList.emplace_after( mLastToken, Token::Type::NOT_EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'!' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'!' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'&' )
			{
				if ( ++mOffset < mTextLength )
				{
					if ( mText[mOffset] == u'&' )
					{
						mLastToken = tokenList.emplace_after( mLastToken, Token::Type::AND, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'&' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'&' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'|' )
			{
				if ( ++mOffset < mTextLength )
				{
					if ( mText[mOffset] == u'|' )
					{
						mLastToken = tokenList.emplace_after( mLastToken, Token::Type::OR, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'|' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( u'|' ), currentLocation );
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
					
					if ( ++mOffset < mTextLength )
					{
						tChar = mText[mOffset];
					}
					else
					{
						break;
					}
				}
				while ( u'0' <= tChar && tChar <= u'9' );

				if ( mOffset >= mTextLength || tChar != u'.' )
				{
					mLastToken = tokenList.emplace_after( mLastToken, integer, currentLocation );
					currentLocation.IncreaseColumn( length );
				}
				else
				{
					length++;

					if ( ++mOffset < mTextLength )
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
								
								if ( ++mOffset < mTextLength )
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

					mLastToken = tokenList.emplace_after( mLastToken, integer + double( valueBelowDecimalPoint ) / tenPowerDecimalPlace, currentLocation );
					currentLocation.IncreaseColumn( length );
				}
			}
			else if ( tChar == u'\"' )
			{
				u16string *tStr;

				unsigned int lineFeed = 0;
				unsigned int length = 0;

				if ( ++mOffset < mTextLength )
				{
					tChar = mText[mOffset];
				}
				else
				{
					ErrorHandler::OnError( currentLocation, ErrorCode::STRING_NOT_TERMINATED );

					return false;
				}

				tStr = new u16string();

				length++;
				do
				{
					if ( tChar == '\\' )
					{
						if ( ++mOffset < mTextLength )
						{
							tChar = mText[mOffset];
						}
						else
						{
							ErrorHandler::OnError( currentLocation, ErrorCode::STRING_NOT_TERMINATED );
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
							if ( ++mOffset < mTextLength )
							{
								tChar = mText[mOffset];
							}
							else
							{
								ErrorHandler::OnError( currentLocation, ErrorCode::STRING_NOT_TERMINATED );
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
								ErrorHandler::OnWarning( currentLocation, WarningCode::UNKNOWN_ESCAPE_SEQUENCE );
								tStr->push_back( tChar );
								length += 3;
							}
							break;
						
						default:
							ErrorHandler::OnWarning( currentLocation, WarningCode::UNKNOWN_ESCAPE_SEQUENCE );
							break;
						}
					}
					else if ( tChar == '\r' || tChar == '\n' )
					{
						ErrorHandler::OnError( currentLocation, ErrorCode::STRING_NOT_TERMINATED );
						delete tStr;

						return true;
					}
					else
					{
						tStr->push_back( tChar );
						length++;
					}
					
					if ( ++mOffset < mTextLength )
					{
						tChar = mText[mOffset];
					}
					else
					{
						ErrorHandler::OnError( currentLocation, ErrorCode::STRING_NOT_TERMINATED );
						delete tStr;

						return false;
					}
				}
				while ( tChar != '\"' );
				mOffset++;
				length++;

				mLastToken = tokenList.emplace_after( mLastToken, tStr, currentLocation );

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
					if ( ++mOffset < mTextLength )
					{
						tChar = mText[mOffset];
					}
					else
					{
						return false;
					}
				}
				while ( tChar != u'\r' && tChar != u'\n' );

				TokenizeUTF_16( tokenList, currentLocation );
			}
			else if ( tChar == u'{' || tChar == u'}' || tChar == u',' || tChar == u':' || tChar == u'~' || tChar == u'%' || tChar == u'^' )
			{
				mOffset++;

				mLastToken = tokenList.emplace_after( mLastToken, static_cast<Token::Type>( tChar ), currentLocation );
				currentLocation.IncreaseColumn();
			}
			else if ( tChar == Tokenizer::NO_BREAK_SPACE || tChar == Tokenizer::OGHAM_SPACE_MARK || tChar == Tokenizer::MONGOLIAN_VOWEL_SEPARATOR || ( tChar >= Tokenizer::EN_QUAD && tChar <= Tokenizer::HAIR_SPACE ) || tChar == Tokenizer::NARROW_NO_BREAK_SPACE || tChar == Tokenizer::MEDIUM_MATHEMATICAL_SPACE || tChar == Tokenizer::IDEOGRAPHIC_SPACE )	// Unicode class Zs except space character.
			{
				mOffset++;

				TokenizeUTF_16( tokenList, currentLocation );
			}
			else if ( tChar == Tokenizer::NEXT_LINE || tChar == Tokenizer::LINE_SEPARATOR || tChar == Tokenizer::PARAGRAPH_SEPARATOR )	// New line character.
			{
				mOffset++;
				currentLocation.IncreaseLine();

				TokenizeUTF_16( tokenList, currentLocation );
			}
			else
			{
				ErrorHandler::OnError( currentLocation, ErrorCode::WRONG_CHARACTER );

				mOffset++;
				currentLocation.IncreaseColumn();
			}

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

		char16_t *mText;
		unsigned int mTextLength;
		unsigned int mOffset;

		forward_list<Token>::const_iterator mLastToken;
	};
}

#endif
