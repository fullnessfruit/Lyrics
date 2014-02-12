#include <cstring>

#include "Loader.h"

#ifndef TEXT_LOADER
#define TEXT_LOADER

namespace lyrics
{
	class TextLoader
	{
	private:
		TextLoader() = delete;
		TextLoader( TextLoader & ) = delete;

	public:
		static bool LoadText( const char * const fileName, char16_t *&text, unsigned int &length )
		{
			char *data = nullptr;
			unsigned int size = 0;

			if ( !Loader::LoadData( fileName, data, size ) )
			{
				// TODO:
				return false;
			}

			text = TextLoader::ConvertToUTF16( ( unsigned char * )data, size, length );
			delete [] data;
			if ( text == false )
			{
				// TODO:
				return false;
			}

			return true;
		}
	
	private:
		static char16_t *ConvertToUTF16( const unsigned char * const data, const unsigned int size, unsigned int &length )
		{
			using std::size_t;
			using std::memcpy;

			if ( data == nullptr )
			{
				return nullptr;
			}

			switch ( data[0] )
			{
			case 0xEF:
				if ( data[1] == 0xBB && data[2] == 0xBF )
				{
					return TextLoader::UTF8ToUTF16( data + 3, size - 3, length );
				}
				else
				{
					return TextLoader::UTF8ToUTF16( data, size, length );
				}
				break;

			case 0xFE:
				if ( data[1] == 0xFF )
				{
					// TODO: UTF-16 big endian
				}
				else
				{
					return TextLoader::UTF8ToUTF16( data, size, length );
				}
				break;

			case 0xFF:
				if ( data[1] == 0xFE )
				{
					if ( data[2] != 0x00 || data[3] != 0x00 )
					{
						char16_t *tStr;
						size_t tLength;

						for ( tLength = 1; ( ( char16_t * )data )[tLength] != 0 ; tLength++ );

						tStr = new char16_t [tLength];

						memcpy( tStr, data + 2, tLength );

						return tStr;
					}
					else
					{
						return TextLoader::UTF32ToUTF16( ( char32_t * )( data + 4 ), ( size - 4 ) >> 2, length );
					}
				}
				else
				{
					return TextLoader::UTF8ToUTF16( data, size, length );
				}
				break;

			case 0x00:
				if ( data[1] == 0x00 && data[2] == 0xFE && data[3] == 0xFF )
				{
					// TODO: UTF-32 big endian
				}
				else
				{
					return TextLoader::UTF8ToUTF16( data, size, length );
				}
				break;
			
			default:
				return TextLoader::UTF8ToUTF16( data, size, length );
			}

			return nullptr;
		}

		static char16_t *UTF8ToUTF16( const unsigned char * const str, const unsigned int length, unsigned int &convertedStrLength )
		{
			if ( str == nullptr )
			{
				return nullptr;
			}

			char16_t *tStr;

			convertedStrLength = 0;
			for ( unsigned int i = 0; i < length; i++ )
			{
				if ( str[i] < 192u )
				{
					convertedStrLength++;
				}
				else if ( str[i] < 224u )
				{
					convertedStrLength++;
					i++;
				}
				else if ( str[i] < 240u )
				{
					convertedStrLength++;
					i += 2;
				}
				else
				{
					convertedStrLength += 2;
					i += 3;
				}
			}

			tStr = new char16_t [convertedStrLength];

			for ( unsigned int i = 0, j = 0; i < length; i++ )
			{
				if ( str[i] < 192u )
				{
					tStr[j++] = str[i];
				}
				else if ( str[i] < 224u )	// && ( str[i + 1] & 192u ) == 128u
				{
					tStr[j++] = ( str[i] & 31u ) << 6 | ( str[i + 1] & 63u );
					i++;
				}
				else if ( str[i] < 240u )
				{
					tStr[j++] = ( str[i] & 15u ) << 12 | ( str[i + 1] & 63u ) << 6 | ( str[i + 2] & 63u );
					i += 2;
				}
				else
				{
					tStr[j++] = 55296u | ( ( ( str[i] & 7u ) << 2 | ( str[i + 1] & 48u ) >> 4 ) - 1 ) << 6 | ( str[i + 1] & 15u ) << 2 | ( str[i + 2] & 3u );
					tStr[j++] = 56320u | ( str[i + 2] & 15u ) << 6 | ( str[i + 3] & 63u );
					i += 3;
				}
			}

			return tStr;
		}

		static char16_t *UTF32ToUTF16( const char32_t * const str, const unsigned int length, unsigned int &convertedStrLength )
		{
			if ( str == nullptr )
			{
				return nullptr;
			}

			char16_t *tStr;

			convertedStrLength = 0;
			for ( unsigned int i = 0; i < length; i++ )
			{
				if ( *( ( char16_t * )&str[i] + 1 ) == 0 )
				{
					convertedStrLength++;
				}
				else
				{
					convertedStrLength += 2;
				}
			}

			tStr = new char16_t [convertedStrLength];

			for ( unsigned int i = 0, j = 0; i < length; i++ )
			{
				if ( *( ( char16_t * )&str[i] + 1 ) == 0 )
				{
					tStr[j++] = *( char16_t * )&str[i];
				}
				else
				{
					tStr[j++] = 55296u | ( *( ( char16_t * )&str[i] + 1 ) - 1 ) << 6 | *( char16_t * )&str[i] >> 10;
					tStr[j++] = 56320u | ( *( char16_t * )&str[i] & 1023u );
				}
			}

			return tStr;
		}
	};
}

#endif
