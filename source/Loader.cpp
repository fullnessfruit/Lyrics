#include "Loader.h"

#include <new>

#include "FatalErrorCode.h"

#include "Utility.h"

namespace lyrics
{
	char *Loader::Load(const string &name, unsigned int &size) const
	{
		using std::ios;
		using std::ios_base;
		using std::bad_alloc;

		ifstream input(name, ios::in | ios::binary);

		if (input.rdstate() == ios_base::failbit)
		{
			throw FatalErrorCode::CANNOT_OPEN_FILE;
		}

		size = IFStreamSize(input);

		char *data;

		try
		{
			data = new char[size];
		}
		catch (const bad_alloc &e)
		{
			throw FatalErrorCode::NOT_ENOUGH_MEMORY;
		}

		try
		{
			input.read(data, size);
		}
		catch (const ios_base::failure &e)
		{
			Utility::SafeArrayDelete(data);
			throw FatalErrorCode::CANNOT_READ_FILE;
		}

		input.close();
		if (input.rdstate() == ios_base::failbit)
		{
			Utility::SafeArrayDelete(data);
			throw FatalErrorCode::CANNOT_CLOSE_FILE;
		}

		return data;
	}

	unsigned int Loader::IFStreamSize(ifstream &input) const
	{
		using std::ios_base;

		decltype(input.tellg()) off;

		try
		{
			input.seekg(0, ios_base::end);
			off = input.tellg();
			input.seekg(0, ios_base::beg);
		}
		catch (const ios_base::failure &e)
		{
			throw FatalErrorCode::CANNOT_READ_FILE;
		}

		if (off == decltype(off)(-1))
		{
			throw FatalErrorCode::CANNOT_READ_FILE;
		}

		return off;
	}
}