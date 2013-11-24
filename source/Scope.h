#include <string>
#include <unordered_map>

#ifndef SCOPE
#define SCOPE

namespace lyrics
{
	using std::u16string;
	using std::unordered_map;

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

		bool IsPublicExist( const u16string * const identifier ) const
		{
			auto iterator = mEntities.find( *identifier );

			if ( iterator != mEntities.cend() && iterator->second == true && iterator->first == *identifier )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool IsExist( const u16string * const identifier ) const
		{
			auto iterator = mEntities.find( *identifier );

			if ( iterator != mEntities.cend() && iterator->first == *identifier )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void AddPublic( const u16string * const entity )
		{
			mEntities[*entity] = true;
		}

		void AddPrivate( const u16string * const entity )
		{
			mEntities[*entity] = false;
		}

	private:
		const Scope * const mParent;
		unordered_map<u16string, bool> mEntities;
	};
}

#endif
