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
				class ImportNode;
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
				class ClassNode;
				class PackageNode;
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

		virtual bool Visit( const BlockNode * const node ) = 0;
		virtual bool Visit( const IdentifierNode * const node ) = 0;
		virtual bool Visit( const LiteralNode * const node ) = 0;
		virtual bool Visit( const ArrayLiteralNode * const node ) = 0;
		virtual bool Visit( const HashLiteralNode * const node ) = 0;
		virtual bool Visit( const HashNode * const node ) = 0;
		virtual bool Visit( const FunctionLiteralNode * const node ) = 0;
		virtual bool Visit( const ValueParameterNode * const node ) = 0;
		virtual bool Visit( const OutputParameterNode * const node ) = 0;
		virtual bool Visit( const ParenthesizedExpressionNode * const node ) = 0;
		virtual bool Visit( const IndexReferenceNode * const node ) = 0;
		virtual bool Visit( const FunctionCallNode * const node ) = 0;
		virtual bool Visit( const MemberReferenceNode * const node ) = 0;
		virtual bool Visit( const ImportNode * const node ) = 0;
		virtual bool Visit( const UnaryExpressionNode * const node ) = 0;
		virtual bool Visit( const MultiplicativeExpressionNode * const node ) = 0;
		virtual bool Visit( const AdditiveExpressionNode * const node ) = 0;
		virtual bool Visit( const ShiftExpressionNode * const node ) = 0;
		virtual bool Visit( const AndExpressionNode * const node ) = 0;
		virtual bool Visit( const OrExpressionNode * const node ) = 0;
		virtual bool Visit( const RelationalExpressionNode * const node ) = 0;
		virtual bool Visit( const EqualityExpressionNode * const node ) = 0;
		virtual bool Visit( const LogicalAndExpressionNode * const node ) = 0;
		virtual bool Visit( const LogicalOrExpressionNode * const node ) = 0;
		virtual bool Visit( const AssignmentExpressionNode * const node ) = 0;
		virtual bool Visit( const ClassNode * const node ) = 0;
		virtual bool Visit( const PackageNode * const node ) = 0;
		virtual bool Visit( const IfNode * const node ) = 0;
		virtual bool Visit( const ElseIfNode * const node ) = 0;
		virtual bool Visit( const CaseNode * const node ) = 0;
		virtual bool Visit( const WhenNode * const node ) = 0;
		virtual bool Visit( const WhileNode * const node ) = 0;
		virtual bool Visit( const ForNode * const node ) = 0;
		virtual bool Visit( const ForEachNode * const node ) = 0;
		virtual bool Visit( const RedoNode * const node ) = 0;
		virtual bool Visit( const BreakNode * const node ) = 0;
		virtual bool Visit( const ReturnNode * const node ) = 0;
	};
}

#endif
