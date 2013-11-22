#include <stack>

#include "Visitor.h"
#include "Node.h"
#include "Scope.h"

#include "ErrorCode.h"
#include "BuildLog.h"

#ifndef LOCAL_RESOLVER
#define LOCAL_RESOLVER

namespace lyrics
{
	using std::stack;

	class LocalResolver : public Visitor
	{
	public:
		bool Resolve( const BlockNode * const node, Scope *&scope )
		{
			scope = new Scope( nullptr );
			mScopeStack.push( scope );

			return node->Accept( *this );
		}

		virtual bool Visit( const BlockNode * const node ) const
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
					canProgress = false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const IdentifierNode * const node ) const
		{
			return true;
		}

		virtual bool Visit( const LiteralNode * const node ) const
		{
			return true;
		}

		virtual bool Visit( const ArrayLiteralNode * const node ) const
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
					canProgress = false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const HashLiteralNode * const node ) const
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
					canProgress = false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const HashNode * const node ) const
		{
			bool canProgress = true;

			if ( node->key )
			{
				canProgress &= node->key->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->value )
			{
				canProgress &= node->value->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const FunctionLiteralNode * const node ) const
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
					canProgress = false;
				}
			}

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

		virtual bool Visit( const ValueParameterNode * const node ) const
		{
			bool canProgress = true;

			if ( node->name )
			{
				canProgress &= node->name->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->defalutArgument )
			{
				canProgress &= node->defalutArgument->Accept( *this );
			}

			return canProgress;
		}

		virtual bool Visit( const OutputParameterNode * const node ) const
		{
			bool canProgress = true;

			if ( node->name )
			{
				canProgress &= node->name->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const ParenthesizedExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const IndexReferenceNode * const node ) const
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->index )
			{
				canProgress &= node->index->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const FunctionCallNode * const node ) const
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress = false;
				}
			}

			return canProgress;
		}

		virtual bool Visit( const MemberReferenceNode * const node ) const
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->member )
			{
				canProgress &= node->member->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const UnaryExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->expression )
			{
				canProgress &= node->expression->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const MultiplicativeExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const AdditiveExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const ShiftExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const AndExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const OrExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const RelationalExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const EqualityExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const LogicalAndExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const LogicalOrExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->left )
			{
				canProgress &= node->left->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->right )
			{
				canProgress &= node->right->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const AssignmentExpressionNode * const node ) const
		{
			bool canProgress = true;

			if ( node->lhs )
			{
				if ( node->lhs->GetType() == Node::Type::IDENTIFIER )
				{
					const Scope *scope = mScopeStack.top();

					while ( !scope->IsExist( static_cast<const IdentifierNode * const>( node->lhs )->str ) )
					{
						scope = scope->GetParent();

						if ( !scope )
						{
							mScopeStack.top()->AddEntity( static_cast<const IdentifierNode * const>( node->lhs )->str );
							break;
						}
					}
				}

				canProgress &= node->lhs->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->rhs )
			{
				canProgress &= node->rhs->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const PublicNode * const node ) const
		{
			bool canProgress = true;

			if ( node->name )
			{
				Scope *scope = mScopeStack.top();

				if ( !scope->IsExist( node->name->str ) )
				{
					scope->AddEntity( node->name->str );
				}
				else
				{
					BuildLog::Error( ErrorCode::DUPLICATED_IDENTIFIER, node->location );
					canProgress = false;
				}

//				canProgress &= node->name->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->initializer )
			{
				canProgress &= node->initializer->Accept( *this );
			}

			return canProgress;
		}

		virtual bool Visit( const PrivateNode * const node ) const
		{
			bool canProgress = true;

			if ( node->name )
			{
				Scope *scope = mScopeStack.top();

				if ( !scope->IsExist( node->name->str ) )
				{
					scope->AddEntity( node->name->str );
				}
				else
				{
					BuildLog::Error( ErrorCode::DUPLICATED_IDENTIFIER, node->location );
					canProgress = false;
				}

//				canProgress &= node->name->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->initializer )
			{
				canProgress &= node->initializer->Accept( *this );
			}

			return canProgress;
		}

		virtual bool Visit( const ClassNode * const node ) const
		{
			bool canProgress = true;

			if ( node->name )
			{
				Scope *scope = mScopeStack.top();

				if ( !scope->IsExist( node->name->str ) )
				{
					scope->AddEntity( node->name->str );
				}
				else
				{
					BuildLog::Error( ErrorCode::DUPLICATED_IDENTIFIER, node->location );
					canProgress = false;
				}

//				canProgress &= node->name->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->base )
			{
				canProgress &= node->base->Accept( *this );
			}

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

		virtual bool Visit( const PackageNode * const node ) const
		{
			bool canProgress = true;

			if ( node->name )
			{
				Scope *scope = mScopeStack.top();

				if ( !scope->IsExist( node->name->str ) )
				{
					scope->AddEntity( node->name->str );
				}
				else
				{
					BuildLog::Error( ErrorCode::DUPLICATED_IDENTIFIER, node->location );
					canProgress = false;
				}

//				canProgress &= node->name->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

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

		virtual bool Visit( const ImportNode * const node ) const
		{
			bool canProgress = true;

			if ( node->package )
			{
				canProgress &= node->package->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			return canProgress;
		}

		virtual bool Visit( const IfNode * const node ) const
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
					canProgress = false;
				}
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}

			return canProgress;
		}

		virtual bool Visit( const ElseIfNode * const node ) const
		{
			bool canProgress = true;

			if ( node->condition )
			{
				canProgress &= node->condition->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

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

		virtual bool Visit( const CaseNode * const node ) const
		{
			bool canProgress = true;

			if ( node->value )
			{
				canProgress &= node->value->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			for ( auto i : node->list )
			{
				if ( i )
				{
					canProgress &= i->Accept( *this );
				}
				else
				{
					canProgress = false;
				}
			}

			if ( node->block )
			{
				canProgress &= node->block->Accept( *this );
			}

			return canProgress;
		}

		virtual bool Visit( const WhenNode * const node ) const
		{
			bool canProgress = true;

			if ( node->condition )
			{
				canProgress &= node->condition->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

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

		virtual bool Visit( const WhileNode * const node ) const
		{
			bool canProgress = true;

			if ( node->condition )
			{
				canProgress &= node->condition->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

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

		virtual bool Visit( const ForNode * const node ) const
		{
			bool canProgress = true;

			if ( node->initializer )
			{
				canProgress &= node->initializer->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->condition )
			{
				canProgress &= node->condition->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

			if ( node->iterator )
			{
				canProgress &= node->iterator->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

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

		virtual bool Visit( const ForEachNode * const node ) const
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

			if ( node->collection )
			{
				canProgress &= node->collection->Accept( *this );
			}
			else
			{
				canProgress = false;
			}

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

		virtual bool Visit( const RedoNode * const node ) const
		{
			return true;
		}

		virtual bool Visit( const BreakNode * const node ) const
		{
			return true;
		}

		virtual bool Visit( const ReturnNode * const node ) const
		{
			bool canProgress = true;

			if ( node->value )
			{
				node->value->Accept( *this );
			}

			return canProgress;
		}

	private:
		stack<Scope *> mScopeStack;
	};
}

#endif
