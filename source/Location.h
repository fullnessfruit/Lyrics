#include <iostream>
#include <string>

#ifndef LOCATION
#define LOCATION

namespace lyrics
{
	using std::ostream;
	using std::string;

	struct Location
	{
	public:
		explicit Location( const string fileName ) : mFileName( fileName ), mLine( 1 ), mColumn( 1 )
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
		const string mFileName;
		unsigned int mLine;
		unsigned int mColumn;
	};

	ostream &operator<<( ostream &out, const Location &location )
	{
		return out << location.mFileName << ':' << location.mLine << ':' << location.mColumn << ':';
	}
}

#endif
