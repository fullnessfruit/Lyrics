#ifndef VISITOR
#define VISITOR

namespace lyrics
{
	class BlockNode;
				class IdentifierNode;
				class LiteralNode;
				class ArrayLiteralNode;
				class HashLiteralNode;
					class HashNode;
				class FunctionLiteralNode;
						class ValueParameterNode;
						class OutputParameterNode;
				class ParenthesizedExpressionNode;
				class IndexReferenceNode;
				class FunctionCallNode;
				class MemberReferenceNode;
			class UnaryExpressionNode;
			class MultiplicativeExpressionNode;
			class AdditiveExpressionNode;
			class ShiftExpressionNode;
			class AndExpressionNode;
			class OrExpressionNode;
			class RelationalExpressionNode;
			class EqualityExpressionNode;
			class LogicalAndExpressionNode;
			class LogicalOrExpressionNode;
			class AssignmentExpressionNode;
			class PublicNode;
			class PrivateNode;
		class ClassNode;
		class PackageNode;
		class ImportNode;
			class IfNode;
				class ElseIfNode;
			class CaseNode;
				class WhenNode;
			class WhileNode;
			class ForNode;
			class ForEachNode;
			class RedoNode;
			class BreakNode;
			class ReturnNode;

	class Visitor
	{
	public:
		virtual ~Visitor()
		{
		}

		virtual void Visit( const BlockNode * const node ) const = 0;
		virtual void Visit( const IdentifierNode * const node ) const = 0;
		virtual void Visit( const LiteralNode * const node ) const = 0;
		virtual void Visit( const ArrayLiteralNode * const node ) const = 0;
		virtual void Visit( const HashLiteralNode * const node ) const = 0;
		virtual void Visit( const HashNode * const node ) const = 0;
		virtual void Visit( const FunctionLiteralNode * const node ) const = 0;
		virtual void Visit( const ValueParameterNode * const node ) const = 0;
		virtual void Visit( const OutputParameterNode * const node ) const = 0;
		virtual void Visit( const ParenthesizedExpressionNode * const node ) const = 0;
		virtual void Visit( const IndexReferenceNode * const node ) const = 0;
		virtual void Visit( const FunctionCallNode * const node ) const = 0;
		virtual void Visit( const MemberReferenceNode * const node ) const = 0;
		virtual void Visit( const UnaryExpressionNode * const node ) const = 0;
		virtual void Visit( const MultiplicativeExpressionNode * const node ) const = 0;
		virtual void Visit( const AdditiveExpressionNode * const node ) const = 0;
		virtual void Visit( const ShiftExpressionNode * const node ) const = 0;
		virtual void Visit( const AndExpressionNode * const node ) const = 0;
		virtual void Visit( const OrExpressionNode * const node ) const = 0;
		virtual void Visit( const RelationalExpressionNode * const node ) const = 0;
		virtual void Visit( const EqualityExpressionNode * const node ) const = 0;
		virtual void Visit( const LogicalAndExpressionNode * const node ) const = 0;
		virtual void Visit( const LogicalOrExpressionNode * const node ) const = 0;
		virtual void Visit( const AssignmentExpressionNode * const node ) const = 0;
		virtual void Visit( const PublicNode * const node ) const = 0;
		virtual void Visit( const PrivateNode * const node ) const = 0;
		virtual void Visit( const ClassNode * const node ) const = 0;
		virtual void Visit( const PackageNode * const node ) const = 0;
		virtual void Visit( const ImportNode * const node ) const = 0;
		virtual void Visit( const IfNode * const node ) const = 0;
		virtual void Visit( const ElseIfNode * const node ) const = 0;
		virtual void Visit( const CaseNode * const node ) const = 0;
		virtual void Visit( const WhenNode * const node ) const = 0;
		virtual void Visit( const WhileNode * const node ) const = 0;
		virtual void Visit( const ForNode * const node ) const = 0;
		virtual void Visit( const ForEachNode * const node ) const = 0;
		virtual void Visit( const RedoNode * const node ) const = 0;
		virtual void Visit( const BreakNode * const node ) const = 0;
		virtual void Visit( const ReturnNode * const node ) const = 0;
	};
}

#endif
