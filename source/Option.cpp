#include "Option.h"

namespace lyrics
{
	Option::Option(const int argc, const char * const argv[])
	{
		for (int i = 1; i < argc; i++)
		{
			if (argv[i][0] == '-')
			{
				if (argv[i][1] == 's' && argv[i][2] == '\0')
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
}