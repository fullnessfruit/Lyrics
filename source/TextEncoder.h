#ifndef TEXT_ENCODER
#define TEXT_ENCODER

namespace lyrics
{
	class TextEncoder
	{
	public:
		char32_t *DecodeUnicode(const unsigned char * const data, const unsigned int size, unsigned int &length);

	private:
		static const unsigned int SIZE_UTF_8_BOM = 3;
		static const unsigned int SIZE_UTF_16_BOM = 2;
		static const unsigned int SIZE_UTF_32_BOM = 4;

		char32_t *DecodeUTF_8(const unsigned char * const str, const unsigned int length, unsigned int &decodedLength);
		char32_t *DecodeUTF_16(const char16_t * const str, const unsigned int length, unsigned int &decodedLength);
	};
}

#endif