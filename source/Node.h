#include <string>
#include <forward_list>

#include "Location.h"
#include "Token.h"
#include "Literal.h"

#ifndef NODE
#define NODE

namespace lyrics
{
	using std::u16string;
	using std::forward_list;

	struct Node
	{
		virtual ~Node()
		{
		}

		Location location;

//		virtual constexpr Token::Type GetType() const = 0;

		virtual Location GetLocation()
		{
			return location;
		}
	};

	struct BlockNode;
	struct StatementNode;
		struct ExpressionNode;
			struct PrimaryExpressionNode;
				struct IdentifierNode;
				struct LiteralNode;
				struct ArrayNode;
				struct HashNode;
					struct PairNode;
				struct ParenthesizedExpressionNode;
			struct PostfixExpressionNode;
				struct PostfixNode;
					struct IndexNode;
					struct CallNode;
					struct MemberNode;
			struct UnaryExpressionNode;
			struct MultiplicativeExpressionNode;
			struct AdditiveExpressionNode;
			struct ShiftExpressionNode;
			struct AndExpressionNode;
			struct OrExpressionNode;
			struct RelationalExpressionNode;
			struct EqualityExpressionNode;
			struct LogicalAndExpressionNode;
			struct LogicalOrExpressionNode;
		struct AssignmentNode;
		struct ProcedureNode;
			struct ParameterNode;
		struct ClassNode;
		struct PackageNode;
		struct ImportNode;
		struct SelectionNode;
			struct IfNode;
				struct ElseIfNode;
			struct CaseNode;
				struct WhenNode;
		struct IterationNode;
			struct WhileNode;
			struct ForNode;
			struct ForEachNode;
		struct JumpNode;
			struct RedoNode;
			struct BreakNode;
			struct ReturnNode;

	struct StatementNode : public Node
	{
		virtual ~StatementNode()
		{
		}
	};

	struct BlockNode : public Node
	{
		~BlockNode()
		{
			for ( auto i : list )
			{
				delete i;
			}
		}

		forward_list<StatementNode *> list;
		forward_list<StatementNode *>::const_iterator last;
	};

	struct ExpressionNode : public StatementNode
	{
		virtual ~ExpressionNode()
		{
		}
	};

	struct PrimaryExpressionNode : public ExpressionNode
	{
		virtual ~PrimaryExpressionNode()
		{
		}
	};

	struct IdentifierNode : public PrimaryExpressionNode
	{
		u16string identifier;
	};

	struct LiteralNode : public PrimaryExpressionNode
	{
		Literal literal;
	};

	struct ArrayNode : public PrimaryExpressionNode
	{
		~ArrayNode()
		{
			for ( auto i : list )
			{
				delete i;
			}
		}

		forward_list<ExpressionNode *> list;
		forward_list<ExpressionNode *>::const_iterator last;
	};

	struct HashNode : public PrimaryExpressionNode
	{
		forward_list<PairNode> list;
		forward_list<PairNode>::const_iterator last;
	};

	struct PairNode : public Node
	{
		~PairNode()
		{
			delete left;
			delete right;
		}

		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct ParenthesizedExpressionNode : public PrimaryExpressionNode
	{
		~ParenthesizedExpressionNode()
		{
			delete expression;
		}

		ExpressionNode *expression;
	};

	struct PostfixNode : public Node
	{
		virtual ~PostfixNode()
		{
		}
	};

	struct IndexNode : public PostfixNode
	{
		~IndexNode()
		{
			delete expression;
		}

		ExpressionNode *expression;
	};

	struct CallNode : public PostfixNode
	{
		~CallNode()
		{
			for ( auto i : list )
			{
				delete i;
			}
		}

		forward_list<ExpressionNode *> list;
		forward_list<ExpressionNode *>::const_iterator last;
	};

	struct MemberNode : public PostfixNode
	{
		~MemberNode()
		{
			delete identifier;
		}

		IdentifierNode *identifier;
	};

	struct PostfixExpressionNode : public ExpressionNode
	{
		~PostfixExpressionNode()
		{
			delete expression;
			delete postfix;
		}

		ExpressionNode *expression;
		PostfixNode *postfix;
	};

	struct UnaryExpressionNode : public ExpressionNode
	{
		~UnaryExpressionNode()
		{
			delete expression;
		}

		Token::Type op;
		ExpressionNode *expression;
	};

	struct MultiplicativeExpressionNode : public ExpressionNode
	{
		~MultiplicativeExpressionNode()
		{
			delete left;
			delete right;
		}

		Token::Type op;
		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct AdditiveExpressionNode : public ExpressionNode
	{
		~AdditiveExpressionNode()
		{
			delete left;
			delete right;
		}

		Token::Type op;
		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct ShiftExpressionNode : public ExpressionNode
	{
		~ShiftExpressionNode()
		{
			delete left;
			delete right;
		}

		Token::Type op;
		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct AndExpressionNode : public ExpressionNode
	{
		~AndExpressionNode()
		{
			delete left;
			delete right;
		}

		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct OrExpressionNode : public ExpressionNode
	{
		~OrExpressionNode()
		{
			delete left;
			delete right;
		}

		Token::Type op;
		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct RelationalExpressionNode : public ExpressionNode
	{
		~RelationalExpressionNode()
		{
			delete left;
			delete right;
		}

		Token::Type op;
		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct EqualityExpressionNode : public ExpressionNode
	{
		~EqualityExpressionNode()
		{
			delete left;
			delete right;
		}

		Token::Type op;
		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct LogicalAndExpressionNode : public ExpressionNode
	{
		~LogicalAndExpressionNode()
		{
			delete left;
			delete right;
		}

		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct LogicalOrExpressionNode : public ExpressionNode
	{
		~LogicalOrExpressionNode()
		{
			delete left;
			delete right;
		}

		ExpressionNode *left;
		ExpressionNode *right;
	};

	struct AssignmentExpressionNode : public ExpressionNode
	{
		~AssignmentExpressionNode()
		{
			delete left;
			delete right;
		}

		PostfixExpressionNode *left;
		ExpressionNode *right;
	};

	struct ProcedureNode : public StatementNode
	{
		~ProcedureNode()
		{
			delete identifier;
			delete block;
		}

		IdentifierNode *identifier;
		forward_list<ParameterNode> list;
		forward_list<ParameterNode>::const_iterator last;
		BlockNode *block;
	};

	struct ParameterNode : public Node
	{
		~ParameterNode()
		{
			delete identifier;
			delete expression;
		}

		IdentifierNode *identifier;
		ExpressionNode *expression;
	};

	struct ClassNode : public StatementNode
	{
		~ClassNode()
		{
			delete identifier;
			delete base;
			delete block;
		}

		IdentifierNode *identifier;
		IdentifierNode *base;
		BlockNode *block;
	};

	struct PackageNode : public StatementNode
	{
		~PackageNode()
		{
			delete identifier;
			delete block;
		}
		IdentifierNode *identifier;
		BlockNode *block;
	};

	struct ImportNode : public StatementNode
	{
		~ImportNode()
		{
			delete identifier;
		}

		IdentifierNode *identifier;
	};

	struct SelectionNode : public StatementNode
	{
		virtual ~SelectionNode()
		{
		}
	};

	struct IfNode : public SelectionNode
	{
		~IfNode()
		{
			delete block;
		}

		forward_list<ElseIfNode> list;
		forward_list<ElseIfNode>::const_iterator last;
		BlockNode *block;
	};

	struct ElseIfNode : public Node
	{
		~ElseIfNode()
		{
			delete expression;
			delete block;
		}

		ExpressionNode *expression;
		BlockNode *block;
	};

	struct CaseNode : public SelectionNode
	{
		~CaseNode()
		{
			delete block;
		}

		forward_list<WhenNode> list;
		forward_list<WhenNode>::const_iterator last;
		BlockNode *block;
	};

	struct WhenNode : public Node
	{
		~WhenNode()
		{
			delete expression;
			delete block;
		}

		ExpressionNode *expression;
		BlockNode *block;
	};

	struct IterationNode : public StatementNode
	{
		virtual ~IterationNode()
		{
		}
	};

	struct WhileNode : public IterationNode
	{
		~WhileNode()
		{
			delete expression;
			delete block;
		}

		ExpressionNode *expression;
		BlockNode *block;
	};

	struct ForNode : public IterationNode
	{
		~ForNode()
		{
			delete expression1;
			delete expression2;
			delete expression3;
			delete block;
		}

		ExpressionNode *expression1;
		ExpressionNode *expression2;
		ExpressionNode *expression3;
		BlockNode *block;
	};

	struct ForEachNode : public IterationNode
	{
		~ForEachNode()
		{
			delete expression1;
			delete expression2;
			delete block;
		}

		PostfixExpressionNode *expression1;
		PostfixExpressionNode *expression2;
		BlockNode *block;
	};

	struct JumpNode : public StatementNode
	{
		virtual ~JumpNode()
		{
		}
	};

	struct RedoNode : public JumpNode
	{
	};

	struct BreakNode : public JumpNode
	{
	};

	struct ReturnNode : public JumpNode
	{
		~ReturnNode()
		{
			delete expression;
		}

		ExpressionNode *expression;
	};
};

#endif