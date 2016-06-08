#include "Tokenizer.h"

#include <string>
#include <new>

#include "WarningCode.h"
#include "ErrorCode.h"
#include "FatalErrorCode.h"
#include "ErrorHandler.h"
#include "TextLoader.h"

#include "Utility.h"

namespace lyrics
{
	using std::u32string;
	using std::bad_alloc;

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

	bool Tokenizer::Tokenize(const string &fileName, forward_list<Token> &tokenList)
	{
		mText = TextLoader().Load(fileName, mTextLength);

		Location currentLocation(fileName);

		mOffset = 0;

		mLastToken = tokenList.cbefore_begin();

		try
		{
			while (TokenizeUnicode(tokenList, currentLocation));
			mLastToken = tokenList.emplace_after(mLastToken, Token::Type::END_OF_FILE, currentLocation);
		}
		catch (const bad_alloc &e)
		{
			Utility::SafeArrayDelete(mText);
			throw FatalErrorCode::NOT_ENOUGH_MEMORY;
		}

		Utility::SafeArrayDelete(mText);

		return true;
	}

	// TODO: mTextLength is byte of mText, not number of characters.
	bool Tokenizer::TokenizeUnicode(forward_list<Token> &tokenList, Location &currentLocation)
	{
		char32_t tChar;

		if (mOffset < mTextLength)
		{
			tChar = mText[mOffset];
		}
		else
		{
			return false;
		}

		if ((U'A' <= tChar && tChar <= U'Z') || (U'a' <= tChar && tChar <= U'z') || tChar == U'_')
		{
			u32string *tStr = new u32string();
			unsigned int length;
			bool isIdentifier = tChar == U'_' ? true : false;

			do
			{
				tStr->push_back(tChar);

				if (++mOffset < mTextLength)
				{
					tChar = mText[mOffset];
				}
				else
				{
					break;
				}
			} while ((U'A' <= tChar && tChar <= U'Z') || (U'a' <= tChar && tChar <= U'z') || ((U'0' <= tChar && tChar <= U'9') || tChar == U'_' ? isIdentifier = true : false));	// If the string includs number or _, then the string is identifier.

			length = tStr->length();

			if (isIdentifier)
			{
				mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation);
			}
			else
			{
				if (2u > length || 7u < length)	// If the token is shorter than 2 or longer than 7, then the token is identifier.
				{
					mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation);
				}
				else
				{
					switch (tStr->length())
					{
					case 2u:
						if (*tStr == Tokenizer::IF)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IF, currentLocation);
						}
						else if (*tStr == Tokenizer::DO)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::DO, currentLocation);
						}
						else if (*tStr == Tokenizer::IN)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IN, currentLocation);
						}
						else
						{
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation);
						}
						break;

					case 3u:
						if (*tStr == Tokenizer::END)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::END, currentLocation);
						}
						else if (*tStr == Tokenizer::FOR)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::FOR, currentLocation);
						}
						else if (*tStr == Tokenizer::OUT)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::OUT, currentLocation);
						}
						else
						{
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation);
						}
						break;

					case 4u:
						if (*tStr == Tokenizer::ELSE)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::ELSE, currentLocation);
						}
						else if (*tStr == Tokenizer::NULL_TOKEN)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::NULL_LITERAL, currentLocation);
						}
						else if (*tStr == Tokenizer::TRUE)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, true, currentLocation);
						}
						else if (*tStr == Tokenizer::CASE)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::CASE, currentLocation);
						}
						else if (*tStr == Tokenizer::WHEN)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::WHEN, currentLocation);
						}
						else if (*tStr == Tokenizer::THIS)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::THIS, currentLocation);
						}
						else if (*tStr == Tokenizer::NEXT)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::NEXT, currentLocation);
						}
						else if (*tStr == Tokenizer::THEN)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::THEN, currentLocation);
						}
						else
						{
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation);
						}
						break;

					case 5u:
						if (*tStr == Tokenizer::WHILE)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::WHILE, currentLocation);
						}
						else if (*tStr == Tokenizer::BREAK)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::BREAK, currentLocation);
						}
						else if (*tStr == Tokenizer::FALSE)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, false, currentLocation);
						}
						else if (*tStr == Tokenizer::CLASS)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::CLASS, currentLocation);
						}
						else
						{
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation);
						}
						break;

					case 6u:
						if (*tStr == Tokenizer::ELSEIF)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::ELSEIF, currentLocation);
						}
						else if (*tStr == Tokenizer::IMPORT)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IMPORT, currentLocation);
						}
						else if (*tStr == Tokenizer::RETURN)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::RETURN, currentLocation);
						}
						else if (*tStr == Tokenizer::PUBLIC)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::PUBLIC, currentLocation);
						}
						else
						{
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation);
						}
						break;

					case 7u:
						if (*tStr == Tokenizer::ROUTINE)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::ROUTINE, currentLocation);
						}
						else if (*tStr == Tokenizer::PRIVATE)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::PRIVATE, currentLocation);
						}
						else if (*tStr == Tokenizer::PACKAGE)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::PACKAGE, currentLocation);
						}
						else if (*tStr == Tokenizer::FOREACH)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::FOREACH, currentLocation);
						}
						else if (*tStr == Tokenizer::INCLUDE)
						{
							delete tStr;
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::INCLUDE, currentLocation);
						}
						else
						{
							mLastToken = tokenList.emplace_after(mLastToken, Token::Type::IDENTIFIER, tStr, currentLocation);
						}
						break;
					}
				}
			}

			currentLocation.IncreaseColumn(length);
		}
		else if (tChar == U' ' || tChar == U'\t' || tChar == U'\r' || tChar == U'\v' || tChar == U'\f')
		{
			currentLocation.IncreaseColumn();
			mOffset++;

			TokenizeUnicode(tokenList, currentLocation);
		}
		else if (tChar == U'\n')
		{
			currentLocation.IncreaseLine();
			mOffset++;

			TokenizeUnicode(tokenList, currentLocation);
		}
		else if (tChar == U'.')
		{
			if (++mOffset < mTextLength)
			{
				tChar = mText[mOffset];

				if (tChar < U'0' || tChar > U'9')
				{
					mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'.'), currentLocation);
					currentLocation.IncreaseColumn();
				}
				else	// real
				{
					long long valueBelowDecimalPoint = 0;
					long long tenPowerDecimalPlace = 1;

					unsigned int length = 1;

					do
					{
						valueBelowDecimalPoint = (valueBelowDecimalPoint << 3) + (valueBelowDecimalPoint << 1);
						valueBelowDecimalPoint += tChar & 15;
						tenPowerDecimalPlace = (tenPowerDecimalPlace << 3) + (tenPowerDecimalPlace << 1);

						length++;

						if (++mOffset < mTextLength)
						{
							tChar = mText[mOffset];
						}
						else
						{
							break;
						}
					} while (U'0' <= tChar && tChar <= U'9');

					mLastToken = tokenList.emplace_after(mLastToken, double(valueBelowDecimalPoint) / tenPowerDecimalPlace, currentLocation);
					currentLocation.IncreaseColumn(length);
				}
			}
			else
			{
				mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'.'), currentLocation);
				currentLocation.IncreaseColumn();

				return false;
			}
		}
		else if (tChar == U'(' || tChar == U')' || tChar == U'+' || tChar == U'-' || tChar == U'*' || tChar == U'/' || tChar == U'[' || tChar == U']')
		{
			mOffset++;
			mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(tChar), currentLocation);
			currentLocation.IncreaseColumn();
		}
		else if (tChar == U'<')
		{
			if (++mOffset < mTextLength)
			{
				switch (mText[mOffset])
				{
				case U'<':
					mLastToken = tokenList.emplace_after(mLastToken, Token::Type::SHIFT_LEFT, currentLocation);
					currentLocation.IncreaseColumn(2);
					mOffset++;
					break;

				case U'=':
					mLastToken = tokenList.emplace_after(mLastToken, Token::Type::LESS_THAN_OR_EQUAL, currentLocation);
					currentLocation.IncreaseColumn(2);
					mOffset++;
					break;

				default:
					mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'<'), currentLocation);
					currentLocation.IncreaseColumn();
					break;
				}
			}
			else
			{
				mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'<'), currentLocation);
				currentLocation.IncreaseColumn();

				return false;
			}
		}
		else if (tChar == U'>')
		{
			if (++mOffset < mTextLength)
			{
				switch (mText[mOffset])
				{
				case U'>':
					mLastToken = tokenList.emplace_after(mLastToken, Token::Type::SHIFT_RIGHT, currentLocation);
					currentLocation.IncreaseColumn(2);
					mOffset++;
					break;

				case U'=':
					mLastToken = tokenList.emplace_after(mLastToken, Token::Type::GREATER_THAN_OR_EQUAL, currentLocation);
					currentLocation.IncreaseColumn(2);
					mOffset++;
					break;

				default:
					mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'>'), currentLocation);
					currentLocation.IncreaseColumn();
					break;
				}
			}
			else
			{
				mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'>'), currentLocation);
				currentLocation.IncreaseColumn();

				return false;
			}
		}
		else if (tChar == U'=')
		{
			if (++mOffset < mTextLength)
			{
				if (mText[mOffset] == U'=')
				{
					mLastToken = tokenList.emplace_after(mLastToken, Token::Type::EQUAL, currentLocation);
					currentLocation.IncreaseColumn(2);
					mOffset++;
				}
				else
				{
					mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'='), currentLocation);
					currentLocation.IncreaseColumn();
				}
			}
			else
			{
				mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'='), currentLocation);
				currentLocation.IncreaseColumn();

				return false;
			}
		}
		else if (tChar == U'!')
		{
			if (++mOffset < mTextLength)
			{
				if (mText[mOffset] == U'=')
				{
					mLastToken = tokenList.emplace_after(mLastToken, Token::Type::NOT_EQUAL, currentLocation);
					currentLocation.IncreaseColumn(2);
					mOffset++;
				}
				else
				{
					mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'!'), currentLocation);
					currentLocation.IncreaseColumn();
				}
			}
			else
			{
				mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'!'), currentLocation);
				currentLocation.IncreaseColumn();

				return false;
			}
		}
		else if (tChar == U'&')
		{
			if (++mOffset < mTextLength)
			{
				if (mText[mOffset] == U'&')
				{
					mLastToken = tokenList.emplace_after(mLastToken, Token::Type::AND, currentLocation);
					currentLocation.IncreaseColumn(2);
					mOffset++;
				}
				else
				{
					mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'&'), currentLocation);
					currentLocation.IncreaseColumn();
				}
			}
			else
			{
				mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'&'), currentLocation);
				currentLocation.IncreaseColumn();

				return false;
			}
		}
		else if (tChar == U'|')
		{
			if (++mOffset < mTextLength)
			{
				if (mText[mOffset] == U'|')
				{
					mLastToken = tokenList.emplace_after(mLastToken, Token::Type::OR, currentLocation);
					currentLocation.IncreaseColumn(2);
					mOffset++;
				}
				else
				{
					mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'|'), currentLocation);
					currentLocation.IncreaseColumn();
				}
			}
			else
			{
				mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(U'|'), currentLocation);
				currentLocation.IncreaseColumn();

				return false;
			}
		}
		else if (U'0' <= tChar && tChar <= U'9')
		{
			long long integer = 0;
			long long valueBelowDecimalPoint = 0;
			long long tenPowerDecimalPlace = 1;

			unsigned int length = 0;

			do
			{
				integer = (integer << 3) + (integer << 1);
				integer += tChar & 15;

				length++;

				if (++mOffset < mTextLength)
				{
					tChar = mText[mOffset];
				}
				else
				{
					break;
				}
			} while (U'0' <= tChar && tChar <= U'9');

			if (mOffset >= mTextLength || tChar != U'.')
			{
				mLastToken = tokenList.emplace_after(mLastToken, integer, currentLocation);
				currentLocation.IncreaseColumn(length);
			}
			else
			{
				length++;

				if (++mOffset < mTextLength)
				{
					tChar = mText[mOffset];

					if (U'0' <= tChar && tChar <= U'9')
					{
						do
						{
							valueBelowDecimalPoint = (valueBelowDecimalPoint << 3) + (valueBelowDecimalPoint << 1);
							valueBelowDecimalPoint += tChar & 15;
							tenPowerDecimalPlace = (tenPowerDecimalPlace << 3) + (tenPowerDecimalPlace << 1);

							length++;

							if (++mOffset < mTextLength)
							{
								tChar = mText[mOffset];
							}
							else
							{
								break;
							}
						} while (U'0' <= tChar && tChar <= U'9');
					}
				}

				mLastToken = tokenList.emplace_after(mLastToken, integer + double(valueBelowDecimalPoint) / tenPowerDecimalPlace, currentLocation);
				currentLocation.IncreaseColumn(length);
			}
		}
		else if (tChar == U'\"')
		{
			u32string *tStr;

			unsigned int lineFeed = 0;
			unsigned int length = 0;

			if (++mOffset < mTextLength)
			{
				tChar = mText[mOffset];
			}
			else
			{
				ErrorHandler::OnError(currentLocation, ErrorCode::STRING_NOT_TERMINATED);

				return false;
			}

			tStr = new u32string();

			length++;
			do
			{
				if (tChar == '\\')
				{
					if (++mOffset < mTextLength)
					{
						tChar = mText[mOffset];
					}
					else
					{
						ErrorHandler::OnError(currentLocation, ErrorCode::STRING_NOT_TERMINATED);
						delete tStr;

						return false;
					}

					switch (tChar)
					{
					case U'n':
						tStr->push_back(U'\n');
						length += 2;
						break;

					case U't':
						tStr->push_back(U'\t');
						length += 2;
						break;

					case U'0':
						tStr->push_back(U'\0');
						length += 2;
						break;

					case U'r':
						tStr->push_back(U'\r');
						length += 2;
						break;

					case U'\'':
						tStr->push_back(U'\'');
						length += 2;
						break;

					case U'\"':
						tStr->push_back(U'\"');
						length += 2;
						break;

					case U'\\':
						tStr->push_back(U'\\');
						length += 2;
						break;

					case U'a':
						tStr->push_back(U'\a');
						length += 2;
						break;

					case U'b':
						tStr->push_back(U'\b');
						length += 2;
						break;

					case U'f':
						tStr->push_back(U'\f');
						length += 2;
						break;

					case U'v':
						tStr->push_back(U'\v');
						length += 2;
						break;

					case U'\n':
						lineFeed++;
						length = 0;
						break;

					case U'\r':
						if (++mOffset < mTextLength)
						{
							tChar = mText[mOffset];
						}
						else
						{
							ErrorHandler::OnError(currentLocation, ErrorCode::STRING_NOT_TERMINATED);
							delete tStr;

							return false;
						}

						if (tChar == '\n')
						{
							lineFeed++;
							length = 0;
						}
						else
						{
							ErrorHandler::OnWarning(currentLocation, WarningCode::UNKNOWN_ESCAPE_SEQUENCE);
							tStr->push_back(tChar);
							length += 3;
						}
						break;

					default:
						ErrorHandler::OnWarning(currentLocation, WarningCode::UNKNOWN_ESCAPE_SEQUENCE);
						break;
					}
				}
				else if (tChar == '\r' || tChar == '\n')
				{
					ErrorHandler::OnError(currentLocation, ErrorCode::STRING_NOT_TERMINATED);
					delete tStr;

					return true;
				}
				else
				{
					tStr->push_back(tChar);
					length++;
				}

				if (++mOffset < mTextLength)
				{
					tChar = mText[mOffset];
				}
				else
				{
					ErrorHandler::OnError(currentLocation, ErrorCode::STRING_NOT_TERMINATED);
					delete tStr;

					return false;
				}
			} while (tChar != '\"');
			mOffset++;
			length++;

			mLastToken = tokenList.emplace_after(mLastToken, tStr, currentLocation);

			while (lineFeed--)
			{
				currentLocation.IncreaseLine();
			}
			currentLocation.IncreaseColumn(length);
		}
		else if (tChar == U'#')
		{
			do
			{
				if (++mOffset < mTextLength)
				{
					tChar = mText[mOffset];
				}
				else
				{
					return false;
				}
			} while (tChar != U'\r' && tChar != U'\n');

			TokenizeUnicode(tokenList, currentLocation);
		}
		else if (tChar == U'{' || tChar == U'}' || tChar == U',' || tChar == U':' || tChar == U'~' || tChar == U'%' || tChar == U'^')
		{
			mOffset++;

			mLastToken = tokenList.emplace_after(mLastToken, static_cast<Token::Type>(tChar), currentLocation);
			currentLocation.IncreaseColumn();
		}
		else if (tChar == Tokenizer::NO_BREAK_SPACE || tChar == Tokenizer::OGHAM_SPACE_MARK || tChar == Tokenizer::MONGOLIAN_VOWEL_SEPARATOR || (tChar >= Tokenizer::EN_QUAD && tChar <= Tokenizer::HAIR_SPACE) || tChar == Tokenizer::NARROW_NO_BREAK_SPACE || tChar == Tokenizer::MEDIUM_MATHEMATICAL_SPACE || tChar == Tokenizer::IDEOGRAPHIC_SPACE)	// Unicode class Zs except space character.
		{
			mOffset++;

			TokenizeUnicode(tokenList, currentLocation);
		}
		else if (tChar == Tokenizer::NEXT_LINE || tChar == Tokenizer::LINE_SEPARATOR || tChar == Tokenizer::PARAGRAPH_SEPARATOR)	// New line character.
		{
			mOffset++;
			currentLocation.IncreaseLine();

			TokenizeUnicode(tokenList, currentLocation);
		}
		else
		{
			ErrorHandler::OnError(currentLocation, ErrorCode::WRONG_CHARACTER);

			mOffset++;
			currentLocation.IncreaseColumn();
		}

		return true;
	}
}