#include "Visitor.h"
#include "Node.h"

#ifndef LOCAL_RESOLVER
#define LOCAL_RESOLVER

namespace lyrics
{
	class LocalResolver : public Visitor
	{
		virtual void Visit( const BlockNode * const node ) const
		{
			for ( auto i : node->list )
			{
				i->Accept( *this );
			}
		}

		virtual void Visit( const IdentifierNode * const node ) const
		{
		}

		virtual void Visit( const LiteralNode * const node ) const
		{
		}

		virtual void Visit( const ArrayLiteralNode * const node ) const
		{
			for ( auto i : node->list )
			{
				i->Accept( *this );
			}
		}

		virtual void Visit( const HashLiteralNode * const node ) const
		{
			for ( auto i : node->list )
			{
				i->Accept( *this );
			}
		}

		virtual void Visit( const HashNode * const node ) const
		{
			node->key->Accept( *this );
			node->value->Accept( *this );
		}

		virtual void Visit( const FunctionLiteralNode * const node ) const
		{
			for ( auto i : node->list )
			{
				i->Accept( *this );
			}
			node->block->Accept( *this );
		}

		virtual void Visit( const ValueParameterNode * const node ) const
		{
			node->name->Accept( *this );
			if ( node->defalutArgument )
			{
				node->defalutArgument->Accept( *this );
			}
		}

		virtual void Visit( const OutputParameterNode * const node ) const
		{
			node->name->Accept( *this );
		}

		virtual void Visit( const ParenthesizedExpressionNode * const node ) const
		{
			node->expression->Accept( *this );
		}

		virtual void Visit( const IndexReferenceNode * const node ) const
		{
			node->expression->Accept( *this );
			node->index->Accept( *this );
		}

		virtual void Visit( const FunctionCallNode * const node ) const
		{
			node->expression->Accept( *this );
			for ( auto i : node->list )
			{
				i->Accept( *this );
			}
		}

		virtual void Visit( const MemberReferenceNode * const node ) const
		{
			node->expression->Accept( *this );
			node->member->Accept( *this );
		}

		virtual void Visit( const UnaryExpressionNode * const node ) const
		{
			node->expression->Accept( *this );
		}

		virtual void Visit( const MultiplicativeExpressionNode * const node ) const
		{
			node->left->Accept( *this );
			node->right->Accept( *this );
		}

		virtual void Visit( const AdditiveExpressionNode * const node ) const
		{
			node->left->Accept( *this );
			node->right->Accept( *this );
		}

		virtual void Visit( const ShiftExpressionNode * const node ) const
		{
			node->left->Accept( *this );
			node->right->Accept( *this );
		}

		virtual void Visit( const AndExpressionNode * const node ) const
		{
			node->left->Accept( *this );
			node->right->Accept( *this );
		}

		virtual void Visit( const OrExpressionNode * const node ) const
		{
			node->left->Accept( *this );
			node->right->Accept( *this );
		}

		virtual void Visit( const RelationalExpressionNode * const node ) const
		{
			node->left->Accept( *this );
			node->right->Accept( *this );
		}

		virtual void Visit( const EqualityExpressionNode * const node ) const
		{
			node->left->Accept( *this );
			node->right->Accept( *this );
		}

		virtual void Visit( const LogicalAndExpressionNode * const node ) const
		{
			node->left->Accept( *this );
			node->right->Accept( *this );
		}

		virtual void Visit( const LogicalOrExpressionNode * const node ) const
		{
			node->left->Accept( *this );
			node->right->Accept( *this );
		}

		virtual void Visit( const AssignmentExpressionNode * const node ) const
		{
			node->lhs->Accept( *this );
			node->lhs->Accept( *this );
		}

		virtual void Visit( const PublicNode * const node ) const
		{
			node->name->Accept( *this );
			if ( node->initializer )
			{
				node->initializer->Accept( *this );
			}
		}

		virtual void Visit( const PrivateNode * const node ) const
		{
			node->name->Accept( *this );
			if ( node->initializer )
			{
				node->initializer->Accept( *this );
			}
		}

		virtual void Visit( const ClassNode * const node ) const
		{
			node->name->Accept( *this );
			if ( node->base )
			{
				node->base->Accept( *this );
			}
			node->block->Accept( *this );
		}

		virtual void Visit( const PackageNode * const node ) const
		{
			node->name->Accept( *this );
			node->block->Accept( *this );
		}

		virtual void Visit( const ImportNode * const node ) const
		{
			node->package->Accept( *this );
		}

		virtual void Visit( const IfNode * const node ) const
		{
			for ( auto i : node->list )
			{
				i->Accept( *this );
			}
			if ( node->block )
			{
				node->block->Accept( *this );
			}
		}

		virtual void Visit( const ElseIfNode * const node ) const
		{
			node->condition->Accept( *this );
			node->block->Accept( *this );
		}

		virtual void Visit( const CaseNode * const node ) const
		{
			node->value->Accept( *this );
			for ( auto i : node->list )
			{
				i->Accept( *this );
			}
			if ( node->block )
			{
				node->block->Accept( *this );
			}
		}

		virtual void Visit( const WhenNode * const node ) const
		{
			node->condition->Accept( *this );
			node->block->Accept( *this );
		}

		virtual void Visit( const WhileNode * const node ) const
		{
			node->condition->Accept( *this );
			node->block->Accept( *this );
		}

		virtual void Visit( const ForNode * const node ) const
		{
			node->initializer->Accept( *this );
			node->condition->Accept( *this );
			node->iterator->Accept( *this );
			node->block->Accept( *this );
		}

		virtual void Visit( const ForEachNode * const node ) const
		{
			node->variable->Accept( *this );
			node->collection->Accept( *this );
			node->block->Accept( *this );
		}

		virtual void Visit( const RedoNode * const node ) const
		{
		}

		virtual void Visit( const BreakNode * const node ) const
		{
		}

		virtual void Visit( const ReturnNode * const node ) const
		{
			if ( node->value )
			{
				node->value->Accept( *this );
			}
		}
	};
}

#endif
