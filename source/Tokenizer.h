#ifndef TOKENIZER
#define TOKENIZER

#include <string>
#include <forward_list>

#include "Token.h"
#include "Location.h"

namespace lyrics
{
	using std::string;
	using std::u32string;
	using std::forward_list;

	class Tokenizer
	{
	public:
		bool Tokenize(const string &fileName, forward_list<Token> &tokenList);
		bool TokenizeUnicode(forward_list<Token> &tokenList, Location &currentLocation);

	private:
		static const u32string BREAK;
		static const u32string CASE;
		static const u32string CLASS;
		static const u32string DO;
		static const u32string END;
		static const u32string ELSE;
		static const u32string ELSEIF;
		static const u32string FALSE;
		static const u32string FOR;
		static const u32string FOREACH;
		static const u32string IF;
		static const u32string IMPORT;
		static const u32string IN;
		static const u32string INCLUDE;
		static const u32string NEXT;
		static const u32string NULL_TOKEN;
		static const u32string OUT;
		static const u32string PACKAGE;
		static const u32string PRIVATE;
		static const u32string PUBLIC;
		static const u32string RETURN;
		static const u32string ROUTINE;
		static const u32string THEN;
		static const u32string THIS;
		static const u32string TRUE;
		static const u32string WHEN;
		static const u32string WHILE;

		// Unicode class Zs
//		static constexpr char32_t SPACE = 0x0020u;
		static constexpr char32_t NO_BREAK_SPACE = 0x00A0u;
		static constexpr char32_t OGHAM_SPACE_MARK = 0x1680u;
		static constexpr char32_t MONGOLIAN_VOWEL_SEPARATOR = 0x180Eu;
		static constexpr char32_t EN_QUAD = 0x2000u;
//		static constexpr char32_t EM_QUAD = 0x2001u;
//		static constexpr char32_t EN_SPACE = 0x2002u;
//		static constexpr char32_t EM_SPACE = 0x2003u;
//		static constexpr char32_t THREE_PER_EM_SPACE = 0x2004u;
//		static constexpr char32_t FOUR_PER_EM_SPACE = 0x2005u;
//		static constexpr char32_t SIX_PER_EM_SPACE = 0x2006u;
//		static constexpr char32_t FIGURE_SPACE = 0x2007u;
//		static constexpr char32_t PUNCTUATION_SPACE = 0x2008u;
//		static constexpr char32_t THIN_SPACE = 0x2009u;
		static constexpr char32_t HAIR_SPACE = 0x200Au;
		static constexpr char32_t NARROW_NO_BREAK_SPACE = 0x202Fu;
		static constexpr char32_t MEDIUM_MATHEMATICAL_SPACE = 0x205Fu;
		static constexpr char32_t IDEOGRAPHIC_SPACE = 0x3000u;

		// New line character
//		static constexpr char32_t CARRIAGE_RETURN = 0x000Du;
//		static constexpr char32_t LINE_FEED = 0x000Au;
		static constexpr char32_t NEXT_LINE = 0x0085u;
		static constexpr char32_t LINE_SEPARATOR = 0x2028u;
		static constexpr char32_t PARAGRAPH_SEPARATOR = 0x2029u;

		char32_t *mText;
		unsigned int mTextLength;
		unsigned int mOffset;

		forward_list<Token>::const_iterator mLastToken;
	};
}

#endif