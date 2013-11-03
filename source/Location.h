#include <iostream>
#include <string>

#ifndef LOCATION
#define LOCATION

namespace lyrics
{
	using std::string;

	struct Location
	{
	public:
		Location()
		{
		}

		Location( const char * const fileName ) : mFileName( fileName ), mLine( 1 ), mColumn( 0 )
		{
		}

		void increaseLine()
		{
			mLine++;
		}

		void increaseColumn()
		{
			mColumn++;
		}

		void increaseColumn( unsigned int length )
		{
			mColumn += length;
		}

		void resetColumn()
		{
			mColumn = 0;
		}

		friend std::ostream &operator<< ( std::ostream &out, const Location &location );

	private:
		string mFileName;
		unsigned int mLine;
		unsigned int mColumn;
	};

	std::ostream &operator<< ( std::ostream &out, const Location &location )
	{
		return out << location.mFileName << ':' << location.mLine << ':' << location.mColumn << ':';
	}
};

#endif