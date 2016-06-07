#include "TextEncoder.h"

#include <cstring>

namespace lyrics
{
	char32_t *TextEncoder::DecodeUnicode(const unsigned char * const data, const unsigned int size, unsigned int &length)
	{
		using std::size_t;
		using std::memcpy;

		if (data == nullptr)
		{
			return nullptr;
		}

		switch (data[0])
		{
		case 0xEF:
			if (data[1] == 0xBB && data[2] == 0xBF)
			{
				return DecodeUTF_8(data + TextEncoder::SIZE_UTF_8_BOM, size - TextEncoder::SIZE_UTF_8_BOM, length);
			}
			else
			{
				return DecodeUTF_8(data, size, length);
			}
			break;

		case 0xFE:
			if (data[1] == 0xFF)
			{
				// TODO: UTF-16 big endian
			}
			else
			{
				return DecodeUTF_8(data, size, length);
			}
			break;

		case 0xFF:
			if (data[1] == 0xFE)
			{
				if (data[2] != 0x00 || data[3] != 0x00)
				{
					return DecodeUTF_16((char16_t *)(data + TextEncoder::SIZE_UTF_16_BOM), (size - TextEncoder::SIZE_UTF_16_BOM) >> 1, length);
				}
				else
				{
					char32_t *tStr;
					const unsigned int sizeWithoutBOM = size - TextEncoder::SIZE_UTF_32_BOM;

					length = sizeWithoutBOM >> 2;
					tStr = new char32_t[length];
					memcpy(tStr, data + TextEncoder::SIZE_UTF_32_BOM, sizeWithoutBOM);

					return tStr;
				}
			}
			else
			{
				return DecodeUTF_8(data, size, length);
			}
			break;

		case 0x00:
			if (data[1] == 0x00 && data[2] == 0xFE && data[3] == 0xFF)
			{
				// TODO: UTF-32 big endian
			}
			else
			{
				return DecodeUTF_8(data, size, length);
			}
			break;

		default:
			return DecodeUTF_8(data, size, length);
		}

		return nullptr;
	}

	char32_t *TextEncoder::DecodeUTF_8(const unsigned char * const str, const unsigned int length, unsigned int &decodedLength)
	{
		if (str == nullptr)
		{
			return nullptr;
		}

		char32_t *tStr;

		decodedLength = 0;
		for (unsigned int i = 0; i < length; i++)
		{
			decodedLength++;

			if (str[i] >= 192u)
			{
				if (str[i] < 224u)
				{
					i++;
				}
				else
				{
					if (str[i] < 240u)
					{
						i += 2;
					}
					else
					{
						i += 3;
					}
				}
			}
		}

		tStr = new char32_t[decodedLength];

		for (unsigned int i = 0, j = 0; i < length; i++)
		{
			if (str[i] < 192u)
			{
				tStr[j++] = str[i];
			}
			// 하위 바이트들이 10으로 시작하는지 여부는 굳이 확인하지 않음. ( str[i + 1] & 192u ) == 128u
			else if (str[i] < 224u)
			{
				tStr[j++] = (str[i] & 31u) << 6 | (str[i + 1] & 63u);
				i++;
			}
			else if (str[i] < 240u)
			{
				tStr[j++] = (str[i] & 15u) << 12 | (str[i + 1] & 63u) << 6 | (str[i + 2] & 63u);
				i += 2;
			}
			else
			{
				tStr[j++] = (str[i] & 7u) << 18 | (str[i + 1] & 63u) << 12 | (str[i + 2] & 63u) << 6 | (str[i + 3] & 63u);
				i += 3;
			}
		}

		return tStr;
	}

	char32_t *TextEncoder::DecodeUTF_16(const char16_t * const str, const unsigned int length, unsigned int &decodedLength)
	{
		if (str == nullptr)
		{
			return nullptr;
		}

		char32_t *tStr;

		decodedLength = 0;
		for (unsigned int i = 0; i < length; i++)
		{
			decodedLength++;

			if ((str[i] & 64512u) == 55296u)
			{
				i++;
			}
		}

		tStr = new char32_t[decodedLength];

		for (unsigned int i = 0, j = 0; i < length; i++)
		{
			if ((str[i] & 64512u) != 55296u)
			{
				tStr[j++] = str[i];
			}
			else
			{
				// 하위 16비트가 110111로 시작하는지 여부는 굳이 확인하지 않음. ( str[i + 1] & 64512u ) == 56320u
				tStr[j++] = (((str[i] & 1023u) >> 6 << 16) + 1) | ((str[i] & 63u) << 10) | (str[i + 1] & 1023u);
				i++;
			}
		}

		return tStr;
	}
}