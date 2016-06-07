#ifndef SCOPE
#define SCOPE

#include <string>
#include <forward_list>
#include <unordered_set>

namespace lyrics
{
	using std::u32string;
	using std::forward_list;
	using std::unordered_set;

	class Scope
	{
	public:
		explicit Scope(Scope * const parent);
		~Scope();

		const Scope *Parent() const
		{
			return mParent;
		}

		void AddChlid(Scope * const child);
		bool IsExist(const u32string * const identifier) const;
		void AddVariable(const u32string * const entity);

	private:
		const Scope * const mParent;
		forward_list<Scope *> mChildren;
		forward_list<Scope *>::const_iterator mLastChild;
		unordered_set<u32string> mEntities;
	};
}

#endif