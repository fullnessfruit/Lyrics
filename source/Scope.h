#include <string>
#include <forward_list>
#include <unordered_set>

#include "Utility.h"

#ifndef SCOPE
#define SCOPE

namespace lyrics
{
	using std::u32string;
	using std::forward_list;
	using std::unordered_set;

	class Scope
	{
	public:
		explicit Scope( Scope * const parent ) : mParent( parent ), mLastChild( mChildren.cbefore_begin() )
		{
			if ( parent )
			{
				parent->AddChlid( this );
			}
		}

		~Scope()
		{
			for ( auto i : mChildren )
			{
				Utility::SafeDelete( i );
			}
		}

		const Scope *Parent() const
		{
			return mParent;
		}

		void AddChlid( Scope * const child )
		{
			mLastChild = mChildren.insert_after( mLastChild, child );
		}

		bool IsExist( const u32string * const identifier ) const
		{
			auto iterator = mEntities.find( *identifier );

			if ( iterator != mEntities.cend() && *iterator == *identifier )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void AddVariable( const u32string * const entity )
		{
			mEntities.insert( *entity );
		}

	private:
		const Scope * const mParent;
		forward_list<Scope *> mChildren;
		forward_list<Scope *>::const_iterator mLastChild;
		unordered_set<u32string> mEntities;
	};
}

#endif
