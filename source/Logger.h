#include <iostream>

#include "Location.h"

#ifndef LOGGER
#define LOGGER

namespace lyrics
{
	using std::cout;
	using std::cerr;
	using std::endl;

	class Logger
	{
	private:
		Logger() = delete;

	public:
		static void Log( const Location location, const char logType[], const unsigned int code, const char * const message )
		{
			cout << location << ' ' << logType << ' ' << code << ": " << message << endl;
		}

		static void Log( const char logType[], const unsigned int code, const char * const message )
		{
			cout << logType << ' ' << code << ": " << message << endl;
		}

		static void StandardErrorLog( const Location location, const char logType[], const unsigned int code )
		{
			cerr << location << ' ' << logType << ' ' << code << endl;
		}

		static void StandardErrorLog( const char logType[], const unsigned int code )
		{
			cerr << logType << ' ' << code << endl;
		}

		static void BuildTerminated()
		{
			cout << "Build terminated." << endl;
		}
	};
}

#endif
