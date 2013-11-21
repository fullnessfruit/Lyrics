#include <string>
#include <unordered_set>

#ifndef SCOPE
#define SCOPE

namespace lyrics
{
	using std::u16string;
	using std::unordered_set;

	class Scope
	{
	public:
		Scope( const Scope * const parent ) : mParent( parent )
		{
		}

		bool IsExist( u16string *entity )
		{
			// TODO: hash collision
			if ( *mEntities.find( *entity ) == *entity )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void AddEntity( u16string *entity )
		{
			mEntities.insert( *entity );
		}

	private:
		const Scope * const mParent;
		unordered_set<u16string> mEntities;
	};
}

#endif
