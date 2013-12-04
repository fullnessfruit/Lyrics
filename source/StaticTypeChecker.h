#include "Visitor.h"
#include "Node.h"

#ifndef STATIC_TYPE_CHECKER
#define STATIC_TYPE_CHECKER

namespace lyrics
{
	class StaticTypeChecker : public Visitor
	{
	public:
		bool Check( const BlockNode * const node )
		{
			return node->Accept( *this );
		}

		virtual bool Visit( const BlockNode * const node )
		{
			bool canProgress = true;

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const IdentifierNode * const node )
		{
			return true;
		}

		virtual bool Visit( const SelfNode * const node )
		{
			return true;
		}

		virtual bool Visit( const LiteralNode * const node )
		{
			return true;
		}

		virtual bool Visit( const ArrayLiteralNode * const node )
		{
			bool canProgress = true;

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const HashLiteralNode * const node )
		{
			bool canProgress = true;

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const HashNode * const node )
		{
			bool canProgress = true;

			if ( node->key )
			{
				canProgress &= node->key->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->value )
			{
				canProgress &= node->value->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const FunctionLiteralNode * const node )
		{
			bool canProgress = true;

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const ValueParameterNode * const node )
		{
			bool canProgress = true;

			if ( node->name )
			{
				canProgress &= node->name->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->defalutArgument )
			{
				canProgress &= node->defalutArgument->Accept( *this );
			}

			return canProgress;
		}

		virtual bool Visit( const OutputParameterNode * const node )
		{
			bool canProgress = true;

			if ( node->name )
			{
				canProgress &= node->name->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const ParenthesizedExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const IndexReferenceNode * const node )
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->index )
			{
				canProgress &= node->index->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const FunctionCallNode * const node )
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const MemberReferenceNode * const node )
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->member )
			{
				canProgress &= node->member->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const UnaryExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const MultiplicativeExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const AdditiveExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const ShiftExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const AndExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const OrExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const RelationalExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const EqualityExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const LogicalAndExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const LogicalOrExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const AssignmentExpressionNode * const node )
		{
			bool canProgress = true;

			if ( node->lhs )
			{
				canProgress &= node->lhs->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->rhs )
			{
				canProgress &= node->rhs->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const ClassNode * const node )
		{
			bool canProgress = true;

			if ( node->base )
			{
				canProgress &= node->base->Accept( *this );
			}

			if ( node->include )
			{
				canProgress &= node->base->Accept( *this );
			}

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const IncludeNode * const node )
		{
			bool canProgress = true;

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const AccessSpecifiedBlockNode * const node )
		{
			bool canProgress = true;

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const PackageNode * const node )
		{
			bool canProgress = true;

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const ImportNode * const node )
		{
			bool canProgress = true;

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const IfNode * const node )
		{
			bool canProgress = true;

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}

			return canProgress;
		}

		virtual bool Visit( const ElseIfNode * const node )
		{
			bool canProgress = true;

			if ( node->condition )
			{
				canProgress &= node->condition->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const CaseNode * const node )
		{
			bool canProgress = true;

			if ( node->value )
			{
				canProgress &= node->value->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress &= false;
				}
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}

			return canProgress;
		}

		virtual bool Visit( const WhenNode * const node )
		{
			bool canProgress = true;

			if ( node->condition )
			{
				canProgress &= node->condition->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const WhileNode * const node )
		{
			bool canProgress = true;

			if ( node->condition )
			{
				canProgress &= node->condition->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const ForNode * const node )
		{
			bool canProgress = true;

			if ( node->initializer )
			{
				canProgress &= node->initializer->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->condition )
			{
				canProgress &= node->condition->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->iterator )
			{
				canProgress &= node->iterator->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const ForEachNode * const node )
		{
			bool canProgress = true;

			if ( node->variable )
			{
				canProgress &= node->variable->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->collection )
			{
				canProgress &= node->collection->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}
			else
			{
				canProgress &= false;
			}

			return canProgress;
		}

		virtual bool Visit( const RedoNode * const node )
		{
			return true;
		}

		virtual bool Visit( const BreakNode * const node )
		{
			return true;
		}

		virtual bool Visit( const ReturnNode * const node )
		{
			bool canProgress = true;

			if ( node->value )
			{
				node->value->Accept( *this );
			}

			return canProgress;
		}
	};
}

#endif
