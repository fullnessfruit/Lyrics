#include <iostream>

#ifndef LOCATION
#define LOCATION

namespace lyrics
{
	using std::ostream;

	struct Location
	{
	public:
		Location( const char * const fileName ) : mFileName( fileName ), mLine( 1 ), mColumn( 0 )
		{
		}

		Location( const Location &location ) : mFileName( location.mFileName ), mLine( location.mLine ), mColumn( location.mColumn )
		{
		}

		void IncreaseLine()
		{
			mLine++;
			mColumn = 0;
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
};

#endif