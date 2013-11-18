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
						IDENTIFIER, LITERAL, ARRAY_LITERAL, HASH_LITERAL, FUNCTION_LITERAL, PARENTHESIZED_EXPRESSION,
							HASH, VALUE_PARAMETER, OUTPUT_PARAMETER,
						INDEX_REFERENCE, FUNCTION_CALL, MEMBER_REFERENCE,
					UNARY_EXPRESSION, MULTIPLICATIVE_EXPRESSION, ADDITIVE_EXPRESSION, SHIFT_EXPRESSION, AND_EXPRESSION, OR_EXPRESSION, RELATIONAL_EXPRESSION, EQUALITY_EXPRESSION, LOGICAL_AND_EXPRESSION, LOGICAL_OR_EXPRESSION, ASSIGNMENT_EXPRESSION,
					PUBLIC, PRIVATE,
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
				struct ArrayLiteralNode;
				struct HashLiteralNode;
					struct HashNode;
				struct ParenthesizedExpressionNode;
			struct PostfixExpressionNode;
				struct IndexReferenceNode;
				struct FunctionCallNode;
				struct MemberReferenceNode;
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
		struct DeclarationNode;
			struct PublicNode;
			struct PrivateNode;
		struct FunctionLiteralNode;
			struct ParameterNode;
				struct ValueParameterNode;
				struct OutputParameterNode;
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
			literal.type = Literal::Type::NIL;
		}

		LiteralNode( const Location &location, const bool boolean ) : PrimaryExpressionNode( location )
		{
			literal.type = Literal::Type::BOOLEAN;
			literal.value.integer = boolean;
		}

		LiteralNode( const Location &location, const long long integer ) : PrimaryExpressionNode( location )
		{
			literal.type = Literal::Type::INTEGER;
			literal.value.integer = integer;
		}

		LiteralNode( const Location &location, const double real ) : PrimaryExpressionNode( location )
		{
			literal.type = Literal::Type::REAL;
			literal.value.integer = real;
		}

		LiteralNode( const Location &location, u16string * const string ) : PrimaryExpressionNode( location )
		{
			literal.type = Literal::Type::STRING;
			literal.value.string = string;
		}

		Literal literal;

		virtual Node::Type GetType() const
		{
			return Node::Type::LITERAL;
		}
	};

	struct ArrayLiteralNode: public PrimaryExpressionNode
	{
		explicit ArrayLiteralNode( const Location &location ) : PrimaryExpressionNode( location )
		{
		}

		~ArrayLiteralNode()
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
			return Node::Type::ARRAY_LITERAL;
		}
	};

	struct HashNode: public Node
	{
		HashNode( const Location &location, const ExpressionNode * const key, const ExpressionNode * const value ) : Node( location ), key( key ), value( value )
		{
		}

		~HashNode()
		{
			delete key;
			delete value;
		}

		const ExpressionNode * const key;
		const ExpressionNode * const value;

		virtual Node::Type GetType() const
		{
			return Node::Type::HASH;
		}
	};

	struct HashLiteralNode: public PrimaryExpressionNode
	{
		explicit HashLiteralNode( const Location &location ) : PrimaryExpressionNode( location )
		{
		}

		~HashLiteralNode()
		{
			for ( auto i : list )
			{
				delete i;
			}
		}

		forward_list<HashNode *> list;
		forward_list<HashNode *>::const_iterator last;

		virtual Node::Type GetType() const
		{
			return Node::Type::HASH_LITERAL;
		}
	};

	struct ParameterNode: public Node
	{
		ParameterNode( const Location &location, const IdentifierNode * const name ) : Node( location ), name( name )
		{
		}

		virtual ~ParameterNode()
		{
			delete name;
		}

		const IdentifierNode * const name;
	};

	struct ValueParameterNode: public ParameterNode
	{
		ValueParameterNode( const Location &location, const IdentifierNode * const name ) : ParameterNode( location, name ), defalutArgument( nullptr )
		{
		}

		ValueParameterNode( const Location &location, const IdentifierNode * const name, const ExpressionNode * const defalutArgument ) : ParameterNode( location, name ), defalutArgument( defalutArgument )
		{
		}

		~ValueParameterNode()
		{
			delete defalutArgument;
		}

		const ExpressionNode * const defalutArgument;

		virtual Node::Type GetType() const
		{
			return Node::Type::VALUE_PARAMETER;
		}
	};

	struct OutputParameterNode: public ParameterNode
	{
		OutputParameterNode( const Location &location, const IdentifierNode * const name ) : ParameterNode( location, name )
		{
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::OUTPUT_PARAMETER;
		}
	};

	struct FunctionLiteralNode: public PrimaryExpressionNode
	{
		explicit FunctionLiteralNode( const Location &location ) : PrimaryExpressionNode( location ), block( nullptr )
		{
		}

		~FunctionLiteralNode()
		{
			for ( auto i : list )
			{
				delete i;
			}

			delete block;
		}

		forward_list<ParameterNode *> list;
		forward_list<ParameterNode *>::const_iterator last;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::FUNCTION_LITERAL;
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

		virtual ~PostfixExpressionNode()
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

	struct FunctionCallNode: public PostfixExpressionNode
	{
		FunctionCallNode( const Location &location, const ExpressionNode * const expression ) : PostfixExpressionNode( location, expression )
		{
		}

		~FunctionCallNode()
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
			return Node::Type::FUNCTION_CALL;
		}
	};

	struct MemberReferenceNode: public PostfixExpressionNode
	{
		MemberReferenceNode( const Location &location, const ExpressionNode * const expression, const IdentifierNode * const member ) : PostfixExpressionNode( location, expression ), member( member )
		{
		}

		~MemberReferenceNode()
		{
			delete member;
		}

		const IdentifierNode * const member;

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

	struct DeclarationNode: public StatementNode
	{
		DeclarationNode( const Location &location, const IdentifierNode * const name ) : StatementNode( location ), name( name ), initializer( nullptr )
		{
		}

		DeclarationNode( const Location &location, const IdentifierNode * const name, const ExpressionNode * const initializer ) : StatementNode( location ), name( name ), initializer( initializer )
		{
		}

		virtual ~DeclarationNode()
		{
			delete name;
			delete initializer;
		}

		const IdentifierNode * const name;
		const ExpressionNode * const initializer;
	};

	struct PublicNode: public DeclarationNode
	{
		PublicNode( const Location &location, const IdentifierNode * const name ) : DeclarationNode( location, name )
		{
		}

		PublicNode( const Location &location, const IdentifierNode * const name, const ExpressionNode * const initializer ) : DeclarationNode( location, name, initializer )
		{
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::PUBLIC;
		}
	};

	struct PrivateNode: public DeclarationNode
	{
		PrivateNode( const Location &location, const IdentifierNode * const name ) : DeclarationNode( location, name )
		{
		}

		PrivateNode( const Location &location, const IdentifierNode * const name, const ExpressionNode * const initializer ) : DeclarationNode( location, name, initializer )
		{
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::PRIVATE;
		}
	};

	struct ClassNode: public StatementNode
	{
		explicit ClassNode( const Location &location ) : StatementNode( location ), name( nullptr ), base( nullptr ), block( nullptr )
		{
		}

		~ClassNode()
		{
			delete name;
			delete base;
			delete block;
		}

		IdentifierNode *name;
		IdentifierNode *base;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::CLASS;
		}
	};

	struct PackageNode: public StatementNode
	{
		explicit PackageNode( const Location &location ) : StatementNode( location ), name( nullptr ), block( nullptr )
		{
		}

		~PackageNode()
		{
			delete name;
			delete block;
		}
		IdentifierNode *name;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::PACKAGE;
		}
	};

	struct ImportNode: public StatementNode
	{
		ImportNode( const Location &location, const IdentifierNode * const package ) : StatementNode( location ), package( package )
		{
		}

		~ImportNode()
		{
			delete package;
		}

		const IdentifierNode * const package;

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
		explicit ElseIfNode( const Location &location ) : Node( location ), condition( nullptr ), block( nullptr )
		{
		}

		~ElseIfNode()
		{
			delete condition;
			delete block;
		}

		ExpressionNode *condition;
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
		explicit WhenNode( const Location &location ) : Node( location ), condition( nullptr ), block( nullptr )
		{
		}

		~WhenNode()
		{
			delete condition;
			delete block;
		}

		ExpressionNode *condition;
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

		ExpressionNode *value;
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
		explicit WhileNode( const Location &location ) : IterationNode( location ), condition( nullptr ), block( nullptr )
		{
		}

		~WhileNode()
		{
			delete condition;
			delete block;
		}

		ExpressionNode *condition;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::WHILE;
		}
	};

	struct ForNode: public IterationNode
	{
		explicit ForNode( const Location &location ) : IterationNode( location ), initializer( nullptr ), condition( nullptr ), iterator( nullptr ), block( nullptr )
		{
		}

		~ForNode()
		{
			delete initializer;
			delete condition;
			delete iterator;
			delete block;
		}

		ExpressionNode *initializer;
		ExpressionNode *condition;
		ExpressionNode *iterator;
		BlockNode *block;

		virtual Node::Type GetType() const
		{
			return Node::Type::FOR;
		}
	};

	struct ForEachNode: public IterationNode
	{
		explicit ForEachNode( const Location &location ) : IterationNode( location ), variable( nullptr ), collection( nullptr ), block( nullptr )
		{
		}

		~ForEachNode()
		{
			delete variable;
			delete collection;
			delete block;
		}

		ExpressionNode *variable;
		ExpressionNode *collection;
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