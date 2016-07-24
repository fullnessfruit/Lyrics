#include "LocalResolver.h"

#include "ErrorCode.h"
#include "ErrorHandler.h"

namespace lyrics
{
	bool LocalResolver::Resolve(const BlockNode * const node, Scope *&scope)
	{
		scope = new Scope(nullptr);
		mScopeStack.push(scope);

		return node->Accept(*this);
	}

	bool LocalResolver::Visit(const BlockNode * const node)
	{
		bool canProgress = true;

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const IdentifierNode * const node)
	{
		const Scope *scope = mScopeStack.top();

		while (!scope->IsExist(node->identifier))
		{
			scope = scope->Parent();

			if (!scope)
			{
				ErrorHandler::OnError(node->location, ErrorCode::USE_OF_UNDECLARED_IDENTIFIER);
				return false;
			}
		}

		return true;
	}

	bool LocalResolver::Visit(const ArrayLiteralNode * const node)
	{
		bool canProgress = true;

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const HashLiteralNode * const node)
	{
		bool canProgress = true;

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const HashNode * const node)
	{
		bool canProgress = true;

		if (node->key)
		{
			canProgress &= node->key->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->value)
		{
			canProgress &= node->value->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const FunctionLiteralNode * const node)
	{
		bool canProgress = true;

		mScopeStack.push(new Scope(mScopeStack.top()));

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		if (node->block)
		{
			canProgress &= node->block->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		mScopeStack.pop();

		return canProgress;
	}

	bool LocalResolver::Visit(const ValueParameterNode * const node)
	{
		bool canProgress = true;

		if (node->name)
		{
			canProgress &= node->name->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->defalutArgument)
		{
			canProgress &= node->defalutArgument->Accept(*this);
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const OutputParameterNode * const node)
	{
		bool canProgress = true;

		if (node->name)
		{
			canProgress &= node->name->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const ParenthesizedExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->expression)
		{
			canProgress &= node->expression->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const IndexReferenceNode * const node)
	{
		bool canProgress = true;

		if (node->expression)
		{
			canProgress &= node->expression->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->index)
		{
			canProgress &= node->index->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const FunctionCallNode * const node)
	{
		bool canProgress = true;

		if (node->expression)
		{
			canProgress &= node->expression->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const MemberReferenceNode * const node)
	{
		bool canProgress = true;

		if (node->expression)
		{
			canProgress &= node->expression->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->member)
		{
			canProgress &= node->member->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const UnaryExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->expression)
		{
			canProgress &= node->expression->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const MultiplicativeExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->left)
		{
			canProgress &= node->left->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->right)
		{
			canProgress &= node->right->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const AdditiveExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->left)
		{
			canProgress &= node->left->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->right)
		{
			canProgress &= node->right->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const ShiftExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->left)
		{
			canProgress &= node->left->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->right)
		{
			canProgress &= node->right->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const AndExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->left)
		{
			canProgress &= node->left->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->right)
		{
			canProgress &= node->right->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const OrExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->left)
		{
			canProgress &= node->left->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->right)
		{
			canProgress &= node->right->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const RelationalExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->left)
		{
			canProgress &= node->left->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->right)
		{
			canProgress &= node->right->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const EqualityExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->left)
		{
			canProgress &= node->left->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->right)
		{
			canProgress &= node->right->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const LogicalAndExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->left)
		{
			canProgress &= node->left->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->right)
		{
			canProgress &= node->right->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const LogicalOrExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->left)
		{
			canProgress &= node->left->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->right)
		{
			canProgress &= node->right->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const AssignmentExpressionNode * const node)
	{
		bool canProgress = true;

		if (node->lhs)
		{
			if (node->lhs->type == Node::Type::IDENTIFIER)
			{
				const Scope *scope = mScopeStack.top();
				const u32string * const identifier = static_cast<const IdentifierNode * const>(node->lhs)->identifier;

				while (!scope->IsExist(identifier))
				{
					scope = scope->Parent();

					if (!scope)
					{
						mScopeStack.top()->AddVariable(identifier);
						break;
					}
				}
			}
		}
		else
		{
			canProgress = false;
		}

		if (node->rhs)
		{
			canProgress &= node->rhs->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const ClassNode * const node)
	{
		bool canProgress = true;

		mScopeStack.push(new Scope(mScopeStack.top()));

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		if (node->baseClassConstructorCall)
		{
			canProgress &= node->baseClassConstructorCall->Accept(*this);
		}

		if (node->include)
		{
			canProgress &= node->include->Accept(*this);
		}

		if (node->block)
		{
			canProgress &= node->block->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		mScopeStack.pop();

		return canProgress;
	}

	bool LocalResolver::Visit(const BaseClassConstructorCallNode * const node)
	{
		bool canProgress = true;

		if (node->baseClass)
		{
			canProgress &= node->baseClass->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const IncludeNode * const node)
	{
		bool canProgress = true;

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const PackageNode * const node)
	{
		bool canProgress = true;

		if (node->block)
		{
			mScopeStack.push(new Scope(mScopeStack.top()));

			canProgress &= node->block->Accept(*this);

			mScopeStack.pop();
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const ImportNode * const node)
	{
		bool canProgress = true;

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const IfNode * const node)
	{
		bool canProgress = true;

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		if (node->block)
		{
			canProgress &= node->block->Accept(*this);
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const ElseIfNode * const node)
	{
		bool canProgress = true;

		if (node->condition)
		{
			canProgress &= node->condition->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->block)
		{
			canProgress &= node->block->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const CaseNode * const node)
	{
		bool canProgress = true;

		if (node->value)
		{
			canProgress &= node->value->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		for (auto i : node->list)
		{
			if (i)
			{
				canProgress &= i->Accept(*this);
			}
			else
			{
				canProgress = false;
			}
		}

		if (node->block)
		{
			canProgress &= node->block->Accept(*this);
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const WhenNode * const node)
	{
		bool canProgress = true;

		if (node->condition)
		{
			canProgress &= node->condition->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->block)
		{
			canProgress &= node->block->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const WhileNode * const node)
	{
		bool canProgress = true;

		if (node->condition)
		{
			canProgress &= node->condition->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->block)
		{
			canProgress &= node->block->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const ForNode * const node)
	{
		bool canProgress = true;

		if (node->initializer)
		{
			canProgress &= node->initializer->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->condition)
		{
			canProgress &= node->condition->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->iterator)
		{
			canProgress &= node->iterator->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->block)
		{
			canProgress &= node->block->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const ForEachNode * const node)
	{
		bool canProgress = true;

		if (node->variable)
		{
			canProgress &= node->variable->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->collection)
		{
			canProgress &= node->collection->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		if (node->block)
		{
			canProgress &= node->block->Accept(*this);
		}
		else
		{
			canProgress = false;
		}

		return canProgress;
	}

	bool LocalResolver::Visit(const ReturnNode * const node)
	{
		bool canProgress = true;

		if (node->value)
		{
			node->value->Accept(*this);
		}

		return canProgress;
	}
}