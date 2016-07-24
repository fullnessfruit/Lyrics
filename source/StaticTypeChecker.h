#ifndef STATIC_TYPE_CHECKER
#define STATIC_TYPE_CHECKER

#include "Visitor.h"
#include "Node.h"

namespace lyrics
{
	class StaticTypeChecker : public Visitor
	{
	public:
		bool Check(const BlockNode * const node);
		virtual bool Visit(const BlockNode * const node);
		virtual bool Visit(const IdentifierNode * const node);
		virtual bool Visit(const ArrayLiteralNode * const node);
		virtual bool Visit(const HashLiteralNode * const node);
		virtual bool Visit(const HashNode * const node);
		virtual bool Visit(const FunctionLiteralNode * const node);
		virtual bool Visit(const ValueParameterNode * const node);
		virtual bool Visit(const OutputParameterNode * const node);
		virtual bool Visit(const ParenthesizedExpressionNode * const node);
		virtual bool Visit(const IndexReferenceNode * const node);
		virtual bool Visit(const FunctionCallNode * const node);
		virtual bool Visit(const MemberReferenceNode * const node);
		virtual bool Visit(const UnaryExpressionNode * const node);
		virtual bool Visit(const MultiplicativeExpressionNode * const node);
		virtual bool Visit(const AdditiveExpressionNode * const node);
		virtual bool Visit(const ShiftExpressionNode * const node);
		virtual bool Visit(const AndExpressionNode * const node);
		virtual bool Visit(const OrExpressionNode * const node);
		virtual bool Visit(const RelationalExpressionNode * const node);
		virtual bool Visit(const EqualityExpressionNode * const node);
		virtual bool Visit(const LogicalAndExpressionNode * const node);
		virtual bool Visit(const LogicalOrExpressionNode * const node);
		virtual bool Visit(const AssignmentExpressionNode * const node);
		virtual bool Visit(const ClassNode * const node);
		virtual bool Visit(const BaseClassConstructorCallNode * const node);
		virtual bool Visit(const IncludeNode * const node);
		virtual bool Visit(const PackageNode * const node);
		virtual bool Visit(const ImportNode * const node);
		virtual bool Visit(const IfNode * const node);
		virtual bool Visit(const ElseIfNode * const node);
		virtual bool Visit(const CaseNode * const node);
		virtual bool Visit(const WhenNode * const node);
		virtual bool Visit(const WhileNode * const node);
		virtual bool Visit(const ForNode * const node);
		virtual bool Visit(const ForEachNode * const node);
		virtual bool Visit(const ReturnNode * const node);
	};
}

#endif