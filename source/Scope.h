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

		const Scope *GetParent() const
		{
			return mParent;
		}

		bool IsExist( const u16string * const entity ) const
		{
			unordered_set<u16string>::const_iterator i = mEntities.find( *entity );

			if ( i != mEntities.cend() && *i == *entity )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void AddEntity( const u16string * const entity )
		{
			mEntities.insert( *entity );
		}

	private:
		const Scope * const mParent;
		unordered_set<u16string> mEntities;
	};
}

#endif
