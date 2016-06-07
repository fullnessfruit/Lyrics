#include "Scope.h"

#include "Utility.h"

namespace lyrics
{
	Scope::Scope(Scope * const parent) : mParent(parent), mLastChild(mChildren.cbefore_begin())
	{
		if (parent)
		{
			parent->AddChlid(this);
		}
	}

	Scope::~Scope()
	{
		for (auto i : mChildren)
		{
			Utility::SafeDelete(i);
		}
	}

	void Scope::AddChlid(Scope * const child)
	{
		mLastChild = mChildren.insert_after(mLastChild, child);
	}

	bool Scope::IsExist(const u32string * const identifier) const
	{
		auto iterator = mEntities.find(*identifier);

		if (iterator != mEntities.cend() && *iterator == *identifier)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Scope::AddVariable(const u32string * const entity)
	{
		mEntities.insert(*entity);
	}
}