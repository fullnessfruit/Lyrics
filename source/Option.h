#include <string>

#ifndef OPTION
#define OPTION

namespace lyrics
{
	using std::string;

	class Option
	{
	public:
		Option( const int argc, const char * const argv[] ) : mFlagInterprete( false )
		{
			for ( int i = 1; i < argc; i++ )
			{
				if ( argv[i][0] == '-' )
				{
					if ( argv[i][1] == 'i' && argv[i][2] == '\0' )
					{
						mFlagInterprete = true;
					}
					else if ( argv[i][1] == 's' && argv[i][2] == '\0' )
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

		bool GetFlagInterprete() const
		{
			return mFlagInterprete;
		}

	private:
		string mSourceFileName;
		bool mFlagInterprete;
	};
}

#endif
