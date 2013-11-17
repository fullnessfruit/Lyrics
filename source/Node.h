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
						INDEX_REFERENCE, PROCEDURE_CALL, MEMBER_REFERENCE,
					UNARY_EXPRESSION, MULTIPLICATIVE_EXPRESSION, ADDITIVE_EXPRESSION, SHIFT_EXPRESSION, AND_EXPRESSION, OR_EXPRESSION, RELATIONAL_EXPRESSION, EQUALITY_EXPRESSION, LOGICAL_AND_EXPRESSION, LOGICAL_OR_EXPRESSION, ASSIGNMENT_EXPRESSION,
				PROCEDURE,
					VALUE_PARAMETER, OUTPUT_PARAMETER,
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

		explicit Node( const Location &location ) : location( location )
		{
		}

		virtual ~Node()
		{
		}

		const Location location;

		virtual Node::Type GetType() const = 0;

		virtual const Location &GetLocation() const
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
			struct OutParameterNode;
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

	struct StatementNode: public Node
	{
		explicit StatementNode( const Location &location ) : Node( location )
		{
		}

		virtual ~StatementNode()
		{
		}
	};

	struct BlockNode: public Node
	{
		explicit BlockNode( const Location &location ) : Node( location )
		{
		}

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

	struct ExpressionNode: public StatementNode
	{
		explicit ExpressionNode( const Location &location ) : StatementNode( location )
		{
		}

		virtual ~ExpressionNode()
		{
		}
	};

	struct PrimaryExpressionNode: public ExpressionNode
	{
		explicit PrimaryExpressionNode( const Location &location ) : ExpressionNode( location )
		{
		}

		virtual ~PrimaryExpressionNode()
		{
		}
	};

	struct IdentifierNode: public PrimaryExpressionNode
	{
		IdentifierNode( const Location &location, const u16string * const str ) : PrimaryExpressionNode( location ), str( str )
		{
		}

		~IdentifierNode()
		{
			delete str;
		}

		const u16string * const str;

		virtual Node::Type GetType() const
		{
			return Node::Type::IDENTIFIER;
		}
	};

	struct LiteralNode: public PrimaryExpressionNode
	{
		explicit LiteralNode( const Location &location ) : PrimaryExpressionNode( location )
		{
		}

		Literal literal;

		virtual Node::Type GetType() const
		{
			return Node::Type::LITERAL;
		}
	};

	struct ArrayNode: public PrimaryExpressionNode
	{
		explicit ArrayNode( const Location &location ) : PrimaryExpressionNode( location )
		{
		}

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

	struct PairNode: public Node
	{
		PairNode( const Location &location, const ExpressionNode * const left, const ExpressionNode * const right ) : Node( location ), left( left ), right( right )
		{
		}

		~PairNode()
		{
			delete left;
			delete right;
		}

		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual Node::Type GetType() const
		{
			return Node::Type::PAIR;
		}
	};

	struct HashNode: public PrimaryExpressionNode
	{
		explicit HashNode( const Location &location ) : PrimaryExpressionNode( location )
		{
		}

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

	struct ParenthesizedExpressionNode: public PrimaryExpressionNode
	{
		ParenthesizedExpressionNode( const Location &location, const ExpressionNode * const expression ) : PrimaryExpressionNode( location ), expression( expression )
		{
		}

		~ParenthesizedExpressionNode()
		{
			delete expression;
		}

		const ExpressionNode * const expression;

		virtual Node::Type GetType() const
		{
			return Node::Type::PARENTHESIZED_EXPRESSION;
		}
	};

	struct PostfixExpressionNode: public ExpressionNode
	{
		PostfixExpressionNode( const Location &location, const ExpressionNode * const expression ) : ExpressionNode( location ), expression( expression )
		{
		}

		~PostfixExpressionNode()
		{
			delete expression;
		}

		const ExpressionNode * const expression;
	};

	struct IndexReferenceNode: public PostfixExpressionNode
	{
		IndexReferenceNode( const Location &location, const ExpressionNode * const expression, const ExpressionNode * const index ) : PostfixExpressionNode( location, expression ), index( index )
		{
		}

		~IndexReferenceNode()
		{
			delete index;
		}

		const ExpressionNode * const index;

		virtual Node::Type GetType() const
		{
			return Node::Type::INDEX_REFERENCE;
		}
	};

	struct ProcedureCallNode: public PostfixExpressionNode
	{
		ProcedureCallNode( const Location &location, const ExpressionNode * const expression ) : PostfixExpressionNode( location, expression )
		{
		}

		~ProcedureCallNode()
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
			return Node::Type::PROCEDURE_CALL;
		}
	};

	struct MemberReferenceNode: public PostfixExpressionNode
	{
		MemberReferenceNode( const Location &location, const ExpressionNode * const expression, const IdentifierNode * const identifier ) : PostfixExpressionNode( location, expression ), identifier( identifier )
		{
		}

		~MemberReferenceNode()
		{
			delete identifier;
		}

		const IdentifierNode * const identifier;

		virtual Node::Type GetType() const
		{
			return Node::Type::MEMBER_REFERENCE;
		}
	};

	struct UnaryExpressionNode: public ExpressionNode
	{
		explicit UnaryExpressionNode( const Location &location ) : ExpressionNode( location ), expression( nullptr )
		{
		}

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

	struct MultiplicativeExpressionNode: public ExpressionNode
	{
		explicit MultiplicativeExpressionNode( const Location &location ) : ExpressionNode( location ), left( nullptr ), right( nullptr )
		{
		}

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

	struct AdditiveExpressionNode: public ExpressionNode
	{
		explicit AdditiveExpressionNode( const Location &location ) : ExpressionNode( location ), left( nullptr ), right( nullptr )
		{
		}

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

	struct ShiftExpressionNode: public ExpressionNode
	{
		explicit ShiftExpressionNode( const Location &location ) : ExpressionNode( location ), left( nullptr ), right( nullptr )
		{
		}

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

	struct AndExpressionNode: public ExpressionNode
	{
		explicit AndExpressionNode( const Location &location ) : ExpressionNode( location ), left( nullptr ), right( nullptr )
		{
		}

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

	struct OrExpressionNode: public ExpressionNode
	{
		explicit OrExpressionNode( const Location &location ) : ExpressionNode( location ), left( nullptr ), right( nullptr )
		{
		}

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

	struct RelationalExpressionNode: public ExpressionNode
	{
		explicit RelationalExpressionNode( const Location &location ) : ExpressionNode( location ), left( nullptr ), right( nullptr )
		{
		}

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

	struct EqualityExpressionNode: public ExpressionNode
	{
		explicit EqualityExpressionNode( const Location &location ) : ExpressionNode( location ), left( nullptr ), right( nullptr )
		{
		}

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

	struct LogicalAndExpressionNode: public ExpressionNode
	{
		explicit LogicalAndExpressionNode( const Location &location ) : ExpressionNode( location ), left( nullptr ), right( nullptr )
		{
		}

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

	struct LogicalOrExpressionNode: public ExpressionNode
	{
		explicit LogicalOrExpressionNode( const Location &location ) : ExpressionNode( location ), left( nullptr ), right( nullptr )
		{
		}

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

	struct AssignmentExpressionNode: public ExpressionNode
	{
		explicit AssignmentExpressionNode( const Location &location ) : ExpressionNode( location ), lhs( nullptr ), rhs( nullptr )
		{
		}

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

	struct ParameterNode: public Node
	{
		ParameterNode( const Location &location, const IdentifierNode * const identifier ) : Node( location ), identifier( identifier )
		{
		}

		~ParameterNode()
		{
			delete identifier;
		}

		const IdentifierNode * const identifier;
	};

	struct ValueParameterNode: public ParameterNode
	{
		ValueParameterNode( const Location &location, const IdentifierNode * const identifier ) : ParameterNode( location, identifier ), expression( nullptr )
		{
		}

		ValueParameterNode( const Location &location, const IdentifierNode * const identifier, const ExpressionNode * const expression ) : ParameterNode( location, identifier ), expression( expression )
		{
		}

		~ValueParameterNode()
		{
			delete expression;
		}

		const ExpressionNode * const expression;

		virtual Node::Type GetType() const
		{
			return Node::Type::VALUE_PARAMETER;
		}
	};

	struct OutputParameterNode: public ParameterNode
	{
		OutputParameterNode( const Location &location, const IdentifierNode * const identifier ) : ParameterNode( location, identifier )
		{
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::OUTPUT_PARAMETER;
		}
	};

	struct ProcedureNode: public StatementNode
	{
		explicit ProcedureNode( const Location &location ) : StatementNode( location ), identifier( nullptr ), block( nullptr )
		{
		}

		~ProcedureNode()
		{
			delete identifier;

			for ( auto i : list )
			{
				delete i;
			}

			delete block;
		}

		IdentifierNode *identifier;
		forward_list<ParameterNode *> list;
		forward_list<ParameterNode *>::const_iterator last;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::PROCEDURE;
		}
	};

	struct ClassNode: public StatementNode
	{
		explicit ClassNode( const Location &location ) : StatementNode( location ), identifier( nullptr ), base( nullptr ), block( nullptr )
		{
		}

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

	struct PackageNode: public StatementNode
	{
		explicit PackageNode( const Location &location ) : StatementNode( location ), identifier( nullptr ), block( nullptr )
		{
		}

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

	struct ImportNode: public StatementNode
	{
		ImportNode( const Location &location, const IdentifierNode * const identifier ) : StatementNode( location ), identifier( identifier )
		{
		}

		~ImportNode()
		{
			delete identifier;
		}

		const IdentifierNode * const identifier;

		virtual Node::Type GetType() const
		{
			return Node::Type::IMPORT;
		}
	};

	struct SelectionNode: public StatementNode
	{
		explicit SelectionNode( const Location &location ) : StatementNode( location )
		{
		}

		virtual ~SelectionNode()
		{
		}
	};

	struct ElseIfNode: public Node
	{
		explicit ElseIfNode( const Location &location ) : Node( location ), expression( nullptr ), block( nullptr )
		{
		}

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

	struct IfNode: public SelectionNode
	{
		explicit IfNode( const Location &location ) : SelectionNode( location ), block( nullptr )
		{
		}

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

	struct WhenNode: public Node
	{
		explicit WhenNode( const Location &location ) : Node( location ), expression( nullptr ), block( nullptr )
		{
		}

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

	struct CaseNode: public SelectionNode
	{
		explicit CaseNode( const Location &location ) : SelectionNode( location ), block( nullptr )
		{
		}

		~CaseNode()
		{
			for ( auto i : list )
			{
				delete i;
			}

			delete block;
		}

		ExpressionNode *expression;
		forward_list<WhenNode *> list;
		forward_list<WhenNode *>::const_iterator last;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::CASE;
		}
	};

	struct IterationNode: public StatementNode
	{
		explicit IterationNode( const Location &location ) : StatementNode( location )
		{
		}

		virtual ~IterationNode()
		{
		}
	};

	struct WhileNode: public IterationNode
	{
		explicit WhileNode( const Location &location ) : IterationNode( location ), expression( nullptr ), block( nullptr )
		{
		}

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

	struct ForNode: public IterationNode
	{
		explicit ForNode( const Location &location ) : IterationNode( location ), expression1( nullptr ), expression2( nullptr ), expression3( nullptr ), block( nullptr )
		{
		}

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

	struct ForEachNode: public IterationNode
	{
		explicit ForEachNode( const Location &location ) : IterationNode( location ), expression1( nullptr ), expression2( nullptr ), block( nullptr )
		{
		}

		~ForEachNode()
		{
			delete expression1;
			delete expression2;
			delete block;
		}

		ExpressionNode *expression1;
		ExpressionNode *expression2;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::FOR_EACH;
		}
	};

	struct JumpNode: public StatementNode
	{
		explicit JumpNode( const Location &location ) : StatementNode( location )
		{
		}

		virtual ~JumpNode()
		{
		}
	};

	struct RedoNode: public JumpNode
	{
		explicit RedoNode( const Location &location ) : JumpNode( location )
		{
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::REDO;
		}
	};

	struct BreakNode: public JumpNode
	{
		explicit BreakNode( const Location &location ) : JumpNode( location )
		{
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::BREAK;
		}
	};

	struct ReturnNode: public JumpNode
	{
		explicit ReturnNode( const Location &location ) : JumpNode( location ), value( nullptr )
		{
		}

		ReturnNode( const Location &location, const ExpressionNode * const value ) : JumpNode( location ), value( value )
		{
		}

		~ReturnNode()
		{
			delete value;
		}

		const ExpressionNode * const value;

		virtual Node::Type GetType() const
		{
			return Node::Type::RETURN;
		}
	};
};

#endif