#ifndef PARSER
#define PARSER

#include <string>
#include <forward_list>

#include "Token.h"
#include "Node.h"

namespace lyrics
{
	using std::string;
	using std::forward_list;

	class Parser
	{
	public:
		BlockNode *Parse(const string &fileName);
	
	private:
		forward_list<Token>::const_iterator mToken;

		BlockNode *Block();
		StatementNode *Statement();
		ExpressionNode *PrimaryExpression();
		ArrayLiteralNode *ArrayLiteral();
		HashLiteralNode *HashLiteral();
		FunctionLiteralNode *FunctionLiteral(forward_list<Token>::const_iterator token);
		ParenthesizedExpressionNode *ParenthesizedExpression();
		ExpressionNode *PostfixExpression();
		ExpressionNode *UnaryExpression();
		ExpressionNode *MultiplicativeExpression();
		ExpressionNode *AdditiveExpression();
		ExpressionNode *ShiftExpression();
		ExpressionNode *AndExpression();
		ExpressionNode *OrExpression();
		ExpressionNode *RelationalExpression();
		ExpressionNode *EqualityExpression();
		ExpressionNode *LogicalAndExpression();
		ExpressionNode *LogicalOrExpression();
		ExpressionNode *AssignmentExpression();
		AssignmentExpressionNode *Class();
		IncludeNode *Include();
		AssignmentExpressionNode *Package();
		ExpressionNode *Expression();
		ImportNode *Import();
		IfNode *If();
		CaseNode *Case();
		WhileNode *While();
		ForNode *For();
		ForEachNode *ForEach();
		BreakNode *Break();
		NextNode *Next();
		ReturnNode *Return();
	};
}

#endif