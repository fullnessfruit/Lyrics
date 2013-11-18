#include <string>
#include <forward_list>

#include "TextLoader.h"
#include "Token.h"
#include "Location.h"

#include "WarningCode.h"
#include "ErrorCode.h"
#include "FatalErrorCode.h"
#include "BuildLog.h"

#ifndef TOKENIZER
#define TOKENIZER

namespace lyrics
{
	using std::u16string;
	using std::forward_list;

	class Tokenizer
	{
	public:
		bool Tokenize( const char * const fileName, forward_list<Token> &token )
		{
			if ( !TextLoader::LoadText( fileName, mText, mTextLength ) )
			{
				BuildLog::FatalError( FatalErrorCode::TEXT_LOADING_FAILED );
				return false;
			}

			Location currentLocation( fileName );

			mOffset = 0;

			mLastToken = token.cbefore_begin();

			while ( Scan( token, currentLocation ) );

			delete [] mText;

			return true;
		}
	
	private:
		static const u16string BREAK;
		static const u16string CASE;
		static const u16string CLASS;
		static const u16string DEF;
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
		static const u16string PRIVATE;
		static const u16string PACKAGE;
		static const u16string PUBLIC;
		static const u16string REDO;
		static const u16string RETURN;
		static const u16string THEN;
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

		// TODO: mTextLength is byte of mText, not number of characters.
		bool Scan( forward_list<Token> &token, Location &currentLocation )
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
					mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
				}
				else
				{
					if ( 2 > length || 7 < length )	// If the token is shorter than 2 or longer than 7, then the token is identifier.
					{
						mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
					}
					else
					{
						switch ( tStr->length() )
						{
						case 2:
							if ( *tStr == Tokenizer::IF )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::IF, currentLocation );
							}
							else if ( *tStr == Tokenizer::DO )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::DO, currentLocation );
							}
							else if ( *tStr == Tokenizer::IN )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::IN, currentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 3:
							if ( *tStr == Tokenizer::END )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::END, currentLocation );
							}
							else if ( *tStr == Tokenizer::FOR )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::FOR, currentLocation );
							}
							else if ( *tStr == Tokenizer::DEF )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::DEF, currentLocation );
							}
							else if ( *tStr == Tokenizer::NIL )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::NIL_LITERAL, currentLocation );
							}
							else if ( *tStr == Tokenizer::OUT )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::OUT, currentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 4:
							if ( *tStr == Tokenizer::ELSE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::ELSE, currentLocation );
							}
							else if ( *tStr == Tokenizer::TRUE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::BOOLEAN_LITERAL, true, currentLocation );
							}
							else if ( *tStr == Tokenizer::CASE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::CASE, currentLocation );
							}
							else if ( *tStr == Tokenizer::WHEN )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::WHEN, currentLocation );
							}
							else if ( *tStr == Tokenizer::REDO )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::REDO, currentLocation );
							}
							else if ( *tStr == Tokenizer::THEN )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::THEN, currentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 5:
							if ( *tStr == Tokenizer::WHILE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::WHILE, currentLocation );
							}
							else if ( *tStr == Tokenizer::BREAK )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::BREAK, currentLocation );
							}
							else if ( *tStr == Tokenizer::FALSE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::BOOLEAN_LITERAL, false, currentLocation );
							}
							else if ( *tStr == Tokenizer::CLASS )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::CLASS, currentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 6:
							if ( *tStr == Tokenizer::ELSEIF )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::ELSEIF, currentLocation );
							}
							else if ( *tStr == Tokenizer::IMPORT )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::IMPORT, currentLocation );
							}
							else if ( *tStr == Tokenizer::RETURN )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::RETURN, currentLocation );
							}
							else if ( *tStr == Tokenizer::PUBLIC )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::PUBLIC, currentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
							}
							break;

						case 7:
							if ( *tStr == Tokenizer::PACKAGE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::PACKAGE, currentLocation );
							}
							else if ( *tStr == Tokenizer::PRIVATE )
							{
								delete tStr;
								mLastToken = token.emplace_after( mLastToken, Token::Type::PRIVATE, currentLocation );
							}
							else
							{
								mLastToken = token.emplace_after( mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation );
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

				Scan( token, currentLocation );
			}
			else if ( tChar == u'\n' )
			{
				currentLocation.IncreaseLine();
				mOffset++;

				Scan( token, currentLocation );
			}
			else if ( tChar == u'.' )
			{
				unsigned int length = 1;

				if ( ++mOffset < mTextLength )
				{
					tChar = mText[mOffset];

					if ( tChar < u'0' || tChar > u'9' )
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'.' ), currentLocation );
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

						mLastToken = token.emplace_after( mLastToken, Token::Type::REAL_LITERAL, double( valueBelowDecimalPoint ) / tenPowerDecimalPlace, currentLocation );
					}

					currentLocation.IncreaseColumn( length );
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'.' ), currentLocation );
					currentLocation.IncreaseColumn();

					return false;
				}
			}
			else if ( tChar == u'(' || tChar == u')' || tChar == u'+' || tChar == u'-' || tChar == u'*' || tChar == u'/' || tChar == u'[' || tChar == u']' )
			{
				mOffset++;
				mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( tChar ), currentLocation );
				currentLocation.IncreaseColumn();
			}
			else if ( tChar == u'<' )
			{
				if ( ++mOffset < mTextLength )
				{
					switch ( mText[mOffset] )
					{
					case u'<':
						mLastToken = token.emplace_after( mLastToken, Token::Type::SHIFT_LEFT, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;

					case u'=':
						mLastToken = token.emplace_after( mLastToken, Token::Type::LESS_THAN_OR_EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;
					
					default:
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'<' ), currentLocation );
						currentLocation.IncreaseColumn();
						break;
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'<' ), currentLocation );
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
						mLastToken = token.emplace_after( mLastToken, Token::Type::SHIFT_RIGHT, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;

					case u'=':
						mLastToken = token.emplace_after( mLastToken, Token::Type::GREATER_THAN_OR_EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
						break;
					
					default:
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'>' ), currentLocation );
						currentLocation.IncreaseColumn();
						break;
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'>' ), currentLocation );
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
						mLastToken = token.emplace_after( mLastToken, Token::Type::EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'=' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'=' ), currentLocation );
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
						mLastToken = token.emplace_after( mLastToken, Token::Type::NOT_EQUAL, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'!' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'!' ), currentLocation );
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
						mLastToken = token.emplace_after( mLastToken, Token::Type::AND, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'&' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'&' ), currentLocation );
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
						mLastToken = token.emplace_after( mLastToken, Token::Type::OR, currentLocation );
						currentLocation.IncreaseColumn( 2 );
						mOffset++;
					}
					else
					{
						mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'|' ), currentLocation );
						currentLocation.IncreaseColumn();
					}
				}
				else
				{
					mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( u'|' ), currentLocation );
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
					mLastToken = token.emplace_after( mLastToken, Token::Type::INTEGER_LITERAL, integer, currentLocation );
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

					mLastToken = token.emplace_after( mLastToken, Token::Type::REAL_LITERAL, integer + double( valueBelowDecimalPoint ) / tenPowerDecimalPlace, currentLocation );
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
					BuildLog::Error( ErrorCode::STRING_NOT_TERMINATED, currentLocation );

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
							BuildLog::Error( ErrorCode::STRING_NOT_TERMINATED, currentLocation );

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
								BuildLog::Error( ErrorCode::STRING_NOT_TERMINATED, currentLocation );

								return false;
							}

							if ( tChar == '\n' )
							{
								lineFeed++;
								length = 0;
							}
							else
							{
								BuildLog::Warning( WarningCode::UNKNOWN_ESCAPE_SEQUENCE, currentLocation );
								tStr->push_back( tChar );
								length += 3;
							}
							break;
						
						default:
							BuildLog::Warning( WarningCode::UNKNOWN_ESCAPE_SEQUENCE, currentLocation );
							break;
						}
					}
					else if ( tChar == '\r' || tChar == '\n' )
					{
						BuildLog::Error( ErrorCode::STRING_NOT_TERMINATED, currentLocation );

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
						BuildLog::Error( ErrorCode::STRING_NOT_TERMINATED, currentLocation );

						return false;
					}
				}
				while ( tChar != '\"' );
				mOffset++;
				length++;

				mLastToken = token.emplace_after( mLastToken, Token::Type::STRING_LITERAL, tStr, currentLocation );

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

				Scan( token, currentLocation );
			}
			else if ( tChar == u'{' || tChar == u'}' || tChar == u',' || tChar == u':' || tChar == u'~' || tChar == u'%' || tChar == u'^' || tChar == u'?' )
			{
				mOffset++;

				mLastToken = token.emplace_after( mLastToken, static_cast<Token::Type>( tChar ), currentLocation );
			}
			else if ( tChar == Tokenizer::NO_BREAK_SPACE || tChar == Tokenizer::OGHAM_SPACE_MARK || tChar == Tokenizer::MONGOLIAN_VOWEL_SEPARATOR || ( tChar >= Tokenizer::EN_QUAD && tChar <= Tokenizer::HAIR_SPACE ) || tChar == Tokenizer::NARROW_NO_BREAK_SPACE || tChar == Tokenizer::MEDIUM_MATHEMATICAL_SPACE || tChar == Tokenizer::IDEOGRAPHIC_SPACE )	// Unicode class Zs except space character.
			{
				mOffset++;

				Scan( token, currentLocation );
			}
			else if ( tChar == Tokenizer::NEXT_LINE || tChar == Tokenizer::LINE_SEPARATOR || tChar == Tokenizer::PARAGRAPH_SEPARATOR )	// New line character.
			{
				currentLocation.IncreaseLine();
				mOffset++;

				Scan( token, currentLocation );
			}
			else
			{
				BuildLog::Error( ErrorCode::WRONG_CHARACTER, currentLocation );
			}

			return true;
		}
	};
};

#endif