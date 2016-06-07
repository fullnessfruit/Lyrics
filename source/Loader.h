#ifndef LOADER
#define LOADER

#include <string>
#include <fstream>

namespace lyrics
{
	using std::string;
	using std::ifstream;

	class Loader
	{
	public:
		char *Load(const string &name, unsigned int &size) const;
	
	private:
		unsigned int IFStreamSize(ifstream &input) const;
	};
}

#endif