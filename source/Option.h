#ifndef OPTION
#define OPTION

#include <string>

namespace lyrics
{
	using std::string;

	class Option
	{
	public:
		Option(const int argc, const char * const argv[]);

		const string SourceCodeFileName() const
		{
			return mSourceCodeFileName;
		}

	private:
		string mSourceCodeFileName;
	};
}

#endif