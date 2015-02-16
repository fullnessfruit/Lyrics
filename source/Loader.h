#include <string>
#include <fstream>

#ifndef LOADER
#define LOADER

namespace lyrics
{
	using std::string;
	using std::ifstream;

	class Loader
	{
	public:
		bool Load( const string &name, char *&data, unsigned int &size )
		{
			using std::ios;
			using std::ios_base;

			ifstream input( name, ios::in | ios::binary );

			if ( input.rdstate() == ios_base::failbit )
			{
				// TODO:
				return false;
			}

			if ( !IFStreamSize( input, size ) )
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
		bool IFStreamSize( ifstream &input, unsigned int &size )
		{
			using std::ios_base;

			input.seekg( 0, ios_base::end );
			auto off = input.tellg();
			input.seekg( 0, ios_base::beg );

			if ( off == decltype( off )( -1 ) )
			{
				return false;
			}

			size = off;

			return true;
		}
	};
}

#endif
