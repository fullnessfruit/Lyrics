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
						mSourceFileName = argv[++i];
					}
				}
				else
				{
					mSourceFileName = argv[i];
				}
			}
		}

		const string GetSourceFileName() const
		{
			return mSourceFileName;
		}

	private:
		string mSourceFileName;
	};
}

#endif
