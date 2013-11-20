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
		}

		virtual void Visit( const IdentifierNode * const node ) const
		{
		}

		virtual void Visit( const LiteralNode * const node ) const
		{
		}

		virtual void Visit( const ArrayLiteralNode * const node ) const
		{
		}

		virtual void Visit( const HashLiteralNode * const node ) const
		{
		}

		virtual void Visit( const HashNode * const node ) const
		{
		}

		virtual void Visit( const FunctionLiteralNode * const node ) const
		{
		}

		virtual void Visit( const ValueParameterNode * const node ) const
		{
		}

		virtual void Visit( const OutputParameterNode * const node ) const
		{
		}

		virtual void Visit( const ParenthesizedExpressionNode * const node ) const
		{
		}

		virtual void Visit( const IndexReferenceNode * const node ) const
		{
		}

		virtual void Visit( const FunctionCallNode * const node ) const
		{
		}

		virtual void Visit( const MemberReferenceNode * const node ) const
		{
		}

		virtual void Visit( const UnaryExpressionNode * const node ) const
		{
		}

		virtual void Visit( const MultiplicativeExpressionNode * const node ) const
		{
		}

		virtual void Visit( const AdditiveExpressionNode * const node ) const
		{
		}

		virtual void Visit( const ShiftExpressionNode * const node ) const
		{
		}

		virtual void Visit( const AndExpressionNode * const node ) const
		{
		}

		virtual void Visit( const OrExpressionNode * const node ) const
		{
		}

		virtual void Visit( const RelationalExpressionNode * const node ) const
		{
		}

		virtual void Visit( const EqualityExpressionNode * const node ) const
		{
		}

		virtual void Visit( const LogicalAndExpressionNode * const node ) const
		{
		}

		virtual void Visit( const LogicalOrExpressionNode * const node ) const
		{
		}

		virtual void Visit( const AssignmentExpressionNode * const node ) const
		{
		}

		virtual void Visit( const PublicNode * const node ) const
		{
		}

		virtual void Visit( const PrivateNode * const node ) const
		{
		}

		virtual void Visit( const ClassNode * const node ) const
		{
		}

		virtual void Visit( const PackageNode * const node ) const
		{
		}

		virtual void Visit( const ImportNode * const node ) const
		{
		}

		virtual void Visit( const IfNode * const node ) const
		{
		}

		virtual void Visit( const ElseIfNode * const node ) const
		{
		}

		virtual void Visit( const CaseNode * const node ) const
		{
		}

		virtual void Visit( const WhenNode * const node ) const
		{
		}

		virtual void Visit( const WhileNode * const node ) const
		{
		}

		virtual void Visit( const ForNode * const node ) const
		{
		}

		virtual void Visit( const ForEachNode * const node ) const
		{
		}

		virtual void Visit( const RedoNode * const node ) const
		{
		}

		virtual void Visit( const BreakNode * const node ) const
		{
		}

		virtual void Visit( const ReturnNode * const node ) const
		{
		}
	};
}

#endif
