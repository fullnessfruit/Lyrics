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
		enum struct Type
		{
			BLOCK,
						IDENTIFIER, LITERAL, ARRAY, HASH, PARENTHESIZED_EXPRESSION,
							PAIR,
					POSTFIX_EXPRESSION,
						INDEX, CALL, MEMBER,
					UNARY_EXPRESSION, MULTIPLICATIVE_EXPRESSION, ADDITIVE_EXPRESSION, SHIFT_EXPRESSION, AND_EXPRESSION, OR_EXPRESSION, RELATIONAL_EXPRESSION, EQUALITY_EXPRESSION, LOGICAL_AND_EXPRESSION, LOGICAL_OR_EXPRESSION, ASSIGNMENT_EXPRESSION,
				PROCEDURE,
					PARAMETER, OUT_PARAMETER,
				CLASS,
				PACKAGE,
				IMPORT,
					IF,
						ELSEIF,
					CASE,
						WHEN,
					WHILE, FOR, FOR_EACH,
					REDO, BREAK, RETURN
		};

		virtual ~Node()
		{
		}

		Location location;

		virtual Node::Type GetType() const = 0;

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

		virtual Node::Type GetType() const
		{
			return Node::Type::BLOCK;
		}
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
		explicit IdentifierNode( u16string *str ) : str( str )
		{
		}

		~IdentifierNode()
		{
			delete str;
		}

		u16string *str;

		virtual Node::Type GetType() const
		{
			return Node::Type::IDENTIFIER;
		}
	};

	struct LiteralNode : public PrimaryExpressionNode
	{
		Literal literal;

		virtual Node::Type GetType() const
		{
			return Node::Type::LITERAL;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::ARRAY;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::PAIR;
		}
	};

	struct HashNode : public PrimaryExpressionNode
	{
		~HashNode()
		{
			for ( auto i : list )
			{
				delete i;
			}
		}

		forward_list<PairNode *> list;
		forward_list<PairNode *>::const_iterator last;

		virtual Node::Type GetType() const
		{
			return Node::Type::HASH;
		}
	};

	struct ParenthesizedExpressionNode : public PrimaryExpressionNode
	{
		ParenthesizedExpressionNode( ExpressionNode *expression ) : expression( expression )
		{
		}

		~ParenthesizedExpressionNode()
		{
			delete expression;
		}

		ExpressionNode *expression;

		virtual Node::Type GetType() const
		{
			return Node::Type::PARENTHESIZED_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::INDEX;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::CALL;
		}
	};

	struct MemberNode : public PostfixNode
	{
		~MemberNode()
		{
			delete identifier;
		}

		IdentifierNode *identifier;

		virtual Node::Type GetType() const
		{
			return Node::Type::MEMBER;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::POSTFIX_EXPRESSION;
		}
	};

	struct UnaryExpressionNode : public ExpressionNode
	{
		~UnaryExpressionNode()
		{
			delete expression;
		}

		Token::Type op;
		ExpressionNode *expression;

		virtual Node::Type GetType() const
		{
			return Node::Type::UNARY_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::MULTIPLICATIVE_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::ADDITIVE_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::SHIFT_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::AND_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::OR_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::RELATIONAL_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::EQUALITY_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::LOGICAL_AND_EXPRESSION;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::LOGICAL_OR_EXPRESSION;
		}
	};

	struct AssignmentExpressionNode : public ExpressionNode
	{
		~AssignmentExpressionNode()
		{
			delete lhs;
			delete rhs;
		}

		ExpressionNode *lhs;
		ExpressionNode *rhs;

		virtual Node::Type GetType() const
		{
			return Node::Type::ASSIGNMENT_EXPRESSION;
		}
	};

	struct ProcedureNode : public StatementNode
	{
		~ProcedureNode()
		{
			delete identifier;

			for ( auto i : parameter )
			{
				delete i;
			}

			for ( auto i : outParameter )
			{
				delete i;
			}

			delete block;
		}

		IdentifierNode *identifier;
		forward_list<ParameterNode *> parameter;
		forward_list<ParameterNode *>::const_iterator lastParameter;
		forward_list<OutParameterNode *> outParameter;
		forward_list<OutParameterNode *>::const_iterator lastOutParameter;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::PROCEDURE;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::PARAMETER;
		}
	};

	struct OutParameterNode : public Node
	{
		~OutParameterNode()
		{
			delete identifier;
		}

		IdentifierNode *identifier;

		virtual Node::Type GetType() const
		{
			return Node::Type::OUT_PARAMETER;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::CLASS;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::PACKAGE;
		}
	};

	struct ImportNode : public StatementNode
	{
		~ImportNode()
		{
			delete identifier;
		}

		IdentifierNode *identifier;

		virtual Node::Type GetType() const
		{
			return Node::Type::IMPORT;
		}
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
			for ( auto i : list )
			{
				delete i;
			}

			delete block;
		}

		forward_list<ElseIfNode *> list;
		forward_list<ElseIfNode *>::const_iterator last;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::IF;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::ELSEIF;
		}
	};

	struct CaseNode : public SelectionNode
	{
		~CaseNode()
		{
			for ( auto i : list )
			{
				delete i;
			}

			delete block;
		}

		forward_list<WhenNode *> list;
		forward_list<WhenNode *>::const_iterator last;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::CASE;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::WHEN;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::WHILE;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::FOR;
		}
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

		virtual Node::Type GetType() const
		{
			return Node::Type::FOR_EACH;
		}
	};

	struct JumpNode : public StatementNode
	{
		virtual ~JumpNode()
		{
		}
	};

	struct RedoNode : public JumpNode
	{
		virtual Node::Type GetType() const
		{
			return Node::Type::REDO;
		}
	};

	struct BreakNode : public JumpNode
	{
		virtual Node::Type GetType() const
		{
			return Node::Type::BREAK;
		}
	};

	struct ReturnNode : public JumpNode
	{
		ReturnNode() : expression( nullptr )
		{
		}

		~ReturnNode()
		{
			delete expression;
		}

		ExpressionNode *expression;

		virtual Node::Type GetType() const
		{
			return Node::Type::RETURN;
		}
	};
};

#endif