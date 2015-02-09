#include <string>

#ifndef OPTION
#define OPTION

namespace lyrics
{
	using std::string;

	class Option
	{
	public:
		Option( const int argc, const char * const argv[] )
		{
			for ( int i = 1; i < argc; i++ )
			{
				if ( argv[i][0] == '-' )
				{
					if ( argv[i][1] == 's' && argv[i][2] == '\0' )
					{
						mSourceCodeFileName = argv[++i];
					}
				}
				else
				{
					mSourceCodeFileName = argv[i];
				}
			}
		}

		const string SourceCodeFileName() const
		{
			return mSourceCodeFileName;
		}

	private:
		string mSourceCodeFileName;
	};
}

#endif
