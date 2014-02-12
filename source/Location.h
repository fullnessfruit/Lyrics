#include <iostream>

#ifndef LOCATION
#define LOCATION

namespace lyrics
{
	using std::ostream;

	struct Location
	{
	public:
		explicit Location( const char * const fileName ) : mFileName( fileName ), mLine( 1 ), mColumn( 1 )
		{
		}

		void IncreaseLine()
		{
			mLine++;
			mColumn = 1;
		}

		void IncreaseColumn()
		{
			mColumn++;
		}

		void IncreaseColumn( unsigned int length )
		{
			mColumn += length;
		}

		friend ostream &operator<<( ostream &out, const Location &location );

	private:
		const char * const mFileName;
		unsigned int mLine;
		unsigned int mColumn;
	};

	ostream &operator<<( ostream &out, const Location &location )
	{
		return out << location.mFileName << ':' << location.mLine << ':' << location.mColumn << ':';
	}
}

#endif
