#include <string>
#include <fstream>

#ifndef LOADER
#define LOADER

namespace lyrics
{
	using std::string;
	using std::istream;

	class Loader
	{
	public:
		bool Load( const string name, char *&data, unsigned int &size )
		{
			using std::ifstream;
			using std::ios;
			using std::ios_base;

			ifstream input( name, ios::in | ios::binary );

			if ( !input )
			{
				// TODO:
				return false;
			}

			if ( !IStreamSize( input, size ) )
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

			input.read( data , size );
			input.close();

			return true;
		}
	
	private:
		bool IStreamSize( istream &input, unsigned int &size )
		{
			using std::ios_base;
			using std::streamoff;

			streamoff off;

			input.seekg( 0, ios_base::end );
			off = input.tellg();
			input.seekg( 0, ios_base::beg );

			if ( off == -1 )
			{
				return false;
			}

			size = off;

			return true;
		}
	};
}

#endif
