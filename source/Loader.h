#include <fstream>

#ifndef LOADER
#define LOADER

namespace lyrics
{
	class Loader
	{
	private:
		Loader() = delete;

	public:
		static bool LoadData( const char * const fileName, char *&data, unsigned int &size )
		{
			using std::ifstream;
			using std::ios;
			using std::ios_base;

			ifstream input;

			input.open( fileName, ios::in | ios::binary );

			if ( !input )
			{
				// TODO:
				return false;
			}

			if ( !Loader::GetInputStreamSize( input, size ) )
			{
				// TODO:
				return false;
			}

			data = new char [size];
			if ( data == nullptr )
			{
				// TODO:
				return false;
			}

			input.seekg( 0, ios_base::beg );
			input.read( data , size );
			input.close();

			return true;
		}
	
	private:
		static bool GetInputStreamSize( std::ifstream &input, unsigned int &size )
		{
			using std::ios_base;
			using std::streamoff;

			streamoff off;

			input.seekg( 0, ios_base::end );
			off = input.tellg();

			if ( off == -1 )
			{
				return false;
			}
			else
			{
				size = off;

				return true;
			}
		}
	};
}

#endif
