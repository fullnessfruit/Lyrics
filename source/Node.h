#include <string>
#include <forward_list>

#include "Token.h"
#include "Location.h"
#include "Literal.h"

#include "Visitor.h"
#include "Element.h"

#ifndef NODE
#define NODE

namespace lyrics
{
	using std::u16string;
	using std::forward_list;

	class Node : public Element
	{
	public:
		enum struct Type
		{
			BLOCK,
						IDENTIFIER, SELF, NULL_LITERAL, BOOLEAN_LITERAL, INTEGER_LITERAL, REAL_LITERAL, STRING_LITERAL, ARRAY_LITERAL, HASH_LITERAL, FUNCTION_LITERAL, PARENTHESIZED_EXPRESSION,
							HASH, VALUE_PARAMETER, OUTPUT_PARAMETER,
						INDEX_REFERENCE, FUNCTION_CALL, MEMBER_REFERENCE,
					UNARY_EXPRESSION, MULTIPLICATIVE_EXPRESSION, ADDITIVE_EXPRESSION, SHIFT_EXPRESSION, AND_EXPRESSION, OR_EXPRESSION, RELATIONAL_EXPRESSION, EQUALITY_EXPRESSION, LOGICAL_AND_EXPRESSION, LOGICAL_OR_EXPRESSION, ASSIGNMENT_EXPRESSION,
				CLASS,
					BASE_CLASS_CONSTRUCTOR_CALL, INCLUDE, ACCESS_SPECIFIED_BLOCK_LIST, ACCESS_SPECIFIED_BLOCK,
				PACKAGE,
				IMPORT,
					IF,
						ELSEIF,
					CASE,
						WHEN,
					WHILE, FOR, FOREACH,
					BREAK, NEXT, RETURN
		};

		explicit Node( const Location &location ) : location( location )
		{
		}

		virtual ~Node()
		{
		}

		const Location location;

		virtual Node::Type GetType() const = 0;
	};

	class BlockNode;
	class StatementNode;
		class ExpressionNode;
			class PrimaryExpressionNode;
				class IdentifierNode;
				class SelfNode;
				class LiteralNode;
				class ArrayLiteralNode;
				class HashLiteralNode;
					class HashNode;
				class FunctionLiteralNode;
					class ParameterNode;
						class ValueParameterNode;
						class OutputParameterNode;
				class ParenthesizedExpressionNode;
			class PostfixExpressionNode;
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
				class ClassNode;
					class BaseClassConstructorCallNode;
					class IncludeNode;
					class AccessSpecifiedBlockListNode;
					class AccessSpecifiedBlockNode;
				class PackageNode;
		class ImportNode;
		class SelectionNode;
			class IfNode;
				class ElseIfNode;
			class CaseNode;
				class WhenNode;
		class IterationNode;
			class WhileNode;
			class ForNode;
			class ForEachNode;
		class JumpNode;
			class BreakNode;
			class NextNode;
			class ReturnNode;

	class StatementNode: public Node
	{
	public:
		explicit StatementNode( const Location &location ) : Node( location )
		{
		}

		virtual ~StatementNode()
		{
		}
	};

	class BlockNode: public Node
	{
	public:
		explicit BlockNode( const Location &location ) : Node( location ), last( list.cbefore_begin() )
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::BLOCK;
		}

		void AddStatement( StatementNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class ExpressionNode: public StatementNode
	{
	public:
		explicit ExpressionNode( const Location &location ) : StatementNode( location )
		{
		}

		virtual ~ExpressionNode()
		{
		}
	};

	class PrimaryExpressionNode: public ExpressionNode
	{
	public:
		explicit PrimaryExpressionNode( const Location &location ) : ExpressionNode( location )
		{
		}

		virtual ~PrimaryExpressionNode()
		{
		}
	};

	class IdentifierNode: public PrimaryExpressionNode
	{
	public:
		IdentifierNode( const Location &location, const u16string * const identifier ) : PrimaryExpressionNode( location ), identifier( identifier )
		{
		}

		~IdentifierNode()
		{
			delete identifier;
		}

		const u16string * const identifier;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::IDENTIFIER;
		}
	};

	class SelfNode: public PrimaryExpressionNode
	{
	public:
		explicit SelfNode( const Location &location ) : PrimaryExpressionNode( location )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::SELF;
		}
	};

	class NullLiteralNode: public PrimaryExpressionNode
	{
	public:
		explicit NullLiteralNode( const Location &location ) : PrimaryExpressionNode( location )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::NULL_LITERAL;
		}
	};

	class BooleanLiteralNode: public PrimaryExpressionNode
	{
	public:
		BooleanLiteralNode( const Location &location, const bool boolean ) : PrimaryExpressionNode( location ), boolean( boolean )
		{
		}

		const bool boolean;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::BOOLEAN_LITERAL;
		}
	};

	class IntegerLiteralNode: public PrimaryExpressionNode
	{
	public:
		IntegerLiteralNode( const Location &location, const long long integer ) : PrimaryExpressionNode( location ), integer( integer )
		{
		}

		const int integer;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::INTEGER_LITERAL;
		}
	};

	class RealLiteralNode: public PrimaryExpressionNode
	{
	public:
		RealLiteralNode( const Location &location, const double real ) : PrimaryExpressionNode( location ), real( real )
		{
		}

		const double real;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::REAL_LITERAL;
		}
	};

	class StringLiteralNode: public PrimaryExpressionNode
	{
	public:
		StringLiteralNode( const Location &location, u16string * const string ) : PrimaryExpressionNode( location ), string( string )
		{
		}

		u16string * const string;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::STRING_LITERAL;
		}
	};

	class ArrayLiteralNode: public PrimaryExpressionNode
	{
	public:
		explicit ArrayLiteralNode( const Location &location ) : PrimaryExpressionNode( location ), last( list.cbefore_begin() )
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::ARRAY_LITERAL;
		}

		void AddExpression( ExpressionNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class HashNode: public Node
	{
	public:
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::HASH;
		}
	};

	class HashLiteralNode: public PrimaryExpressionNode
	{
	public:
		explicit HashLiteralNode( const Location &location ) : PrimaryExpressionNode( location ), last( list.cbefore_begin() )
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::HASH_LITERAL;
		}

		void AddHash( HashNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class ParameterNode: public Node
	{
	public:
		ParameterNode( const Location &location, const IdentifierNode * const name ) : Node( location ), name( name )
		{
		}

		virtual ~ParameterNode()
		{
			delete name;
		}

		const IdentifierNode * const name;
	};

	class ValueParameterNode: public ParameterNode
	{
	public:
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::VALUE_PARAMETER;
		}
	};

	class OutputParameterNode: public ParameterNode
	{
	public:
		OutputParameterNode( const Location &location, const IdentifierNode * const name ) : ParameterNode( location, name )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::OUTPUT_PARAMETER;
		}
	};

	class FunctionLiteralNode: public PrimaryExpressionNode
	{
	public:
		explicit FunctionLiteralNode( const Location &location ) : PrimaryExpressionNode( location ), last( list.cbefore_begin() ), block( nullptr )
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::FUNCTION_LITERAL;
		}

		void AddParameter( ParameterNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class ParenthesizedExpressionNode: public PrimaryExpressionNode
	{
	public:
		ParenthesizedExpressionNode( const Location &location ) : PrimaryExpressionNode( location ), expression( nullptr )
		{
		}

		~ParenthesizedExpressionNode()
		{
			delete expression;
		}

		ExpressionNode *expression;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::PARENTHESIZED_EXPRESSION;
		}
	};

	class PostfixExpressionNode: public ExpressionNode
	{
	public:
		PostfixExpressionNode( const Location &location, const ExpressionNode * const expression ) : ExpressionNode( location ), expression( expression )
		{
		}

		virtual ~PostfixExpressionNode()
		{
			delete expression;
		}

		const ExpressionNode * const expression;
	};

	class IndexReferenceNode: public PostfixExpressionNode
	{
	public:
		IndexReferenceNode( const Location &location, const ExpressionNode * const expression, const ExpressionNode * const index ) : PostfixExpressionNode( location, expression ), index( index )
		{
		}

		~IndexReferenceNode()
		{
			delete index;
		}

		const ExpressionNode * const index;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::INDEX_REFERENCE;
		}
	};

	class FunctionCallNode: public PostfixExpressionNode
	{
	public:
		FunctionCallNode( const Location &location, const ExpressionNode * const expression ) : PostfixExpressionNode( location, expression ), last( list.cbefore_begin() )
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::FUNCTION_CALL;
		}

		void AddArgument( ExpressionNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class MemberReferenceNode: public PostfixExpressionNode
	{
	public:
		MemberReferenceNode( const Location &location, const ExpressionNode * const expression, const IdentifierNode * const member ) : PostfixExpressionNode( location, expression ), member( member )
		{
		}

		~MemberReferenceNode()
		{
			delete member;
		}

		const IdentifierNode * const member;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::MEMBER_REFERENCE;
		}
	};

	class UnaryExpressionNode: public ExpressionNode
	{
	public:
		UnaryExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const expression ) : ExpressionNode( location ), op( op ), expression( expression )
		{
		}

		~UnaryExpressionNode()
		{
			delete expression;
		}

		const Token::Type op;
		const ExpressionNode * const expression;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::UNARY_EXPRESSION;
		}
	};

	class MultiplicativeExpressionNode: public ExpressionNode
	{
	public:
		MultiplicativeExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location ), op( op ), left( left ), right( right )
		{
		}

		~MultiplicativeExpressionNode()
		{
			delete left;
			delete right;
		}

		const Token::Type op;
		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::MULTIPLICATIVE_EXPRESSION;
		}
	};

	class AdditiveExpressionNode: public ExpressionNode
	{
	public:
		AdditiveExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location ), op( op ), left( left ), right( right )
		{
		}

		~AdditiveExpressionNode()
		{
			delete left;
			delete right;
		}

		const Token::Type op;
		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::ADDITIVE_EXPRESSION;
		}
	};

	class ShiftExpressionNode: public ExpressionNode
	{
	public:
		ShiftExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location ), op( op ), left( left ), right( right )
		{
		}

		~ShiftExpressionNode()
		{
			delete left;
			delete right;
		}

		const Token::Type op;
		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::SHIFT_EXPRESSION;
		}
	};

	class AndExpressionNode: public ExpressionNode
	{
	public:
		AndExpressionNode( const Location &location, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location ), left( left ), right( right )
		{
		}

		~AndExpressionNode()
		{
			delete left;
			delete right;
		}

		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::AND_EXPRESSION;
		}
	};

	class OrExpressionNode: public ExpressionNode
	{
	public:
		OrExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location ), op( op ), left( left ), right( right )
		{
		}

		~OrExpressionNode()
		{
			delete left;
			delete right;
		}

		const Token::Type op;
		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::OR_EXPRESSION;
		}
	};

	class RelationalExpressionNode: public ExpressionNode
	{
	public:
		RelationalExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location ), op( op ), left( left ), right( right )
		{
		}

		~RelationalExpressionNode()
		{
			delete left;
			delete right;
		}

		const Token::Type op;
		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::RELATIONAL_EXPRESSION;
		}
	};

	class EqualityExpressionNode: public ExpressionNode
	{
	public:
		EqualityExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location ), op( op ), left( left ), right( right )
		{
		}

		~EqualityExpressionNode()
		{
			delete left;
			delete right;
		}

		const Token::Type op;
		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::EQUALITY_EXPRESSION;
		}
	};

	class LogicalAndExpressionNode: public ExpressionNode
	{
	public:
		LogicalAndExpressionNode( const Location &location, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location ), left( left ), right( right )
		{
		}

		~LogicalAndExpressionNode()
		{
			delete left;
			delete right;
		}

		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::LOGICAL_AND_EXPRESSION;
		}
	};

	class LogicalOrExpressionNode: public ExpressionNode
	{
	public:
		LogicalOrExpressionNode( const Location &location, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location ), left( left ), right( right )
		{
		}

		~LogicalOrExpressionNode()
		{
			delete left;
			delete right;
		}

		const ExpressionNode * const left;
		const ExpressionNode * const right;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::LOGICAL_OR_EXPRESSION;
		}
	};

	class AssignmentExpressionNode: public ExpressionNode
	{
	public:
		AssignmentExpressionNode( const Location &location, const ExpressionNode * const lhs, const ExpressionNode * const rhs ) : ExpressionNode( location ), lhs( lhs ), rhs( rhs )
		{
		}

		~AssignmentExpressionNode()
		{
			delete lhs;
			delete rhs;
		}

		const ExpressionNode * const lhs;
		const ExpressionNode * const rhs;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::ASSIGNMENT_EXPRESSION;
		}
	};

	class BaseClassConstructorCallNode: public Node
	{
	public:
		explicit BaseClassConstructorCallNode( const Location &location ) : Node( location ), baseClass( nullptr ), last( list.cbefore_begin() )
		{
		}

		~BaseClassConstructorCallNode()
		{
			delete baseClass;
			for ( auto i : list )
			{
				delete i;
			}
		}

		IdentifierNode *baseClass;
		forward_list<ExpressionNode *> list;
		forward_list<ExpressionNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::BASE_CLASS_CONSTRUCTOR_CALL;
		}

		void AddArgument( ExpressionNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class IncludeNode: public Node
	{
	public:
		explicit IncludeNode( const Location &location ) : Node( location ), last( list.cbefore_begin() )
		{
		}

		~IncludeNode()
		{
			for ( auto i : list )
			{
				delete i;
			}
		}

		forward_list<IdentifierNode *> list;
		forward_list<IdentifierNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::INCLUDE;
		}

		void AddPackage( IdentifierNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class AccessSpecifiedBlockNode: public Node
	{
	public:
		AccessSpecifiedBlockNode( const Location &location, const Token::Type accessSpecifier, const BlockNode * const block ) : Node( location ), accessSpecifier( accessSpecifier ), block( block )
		{
		}

		~AccessSpecifiedBlockNode()
		{
			delete block;
		}

		const Token::Type accessSpecifier;
		const BlockNode * const block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::ACCESS_SPECIFIED_BLOCK;
		}
	};

	class AccessSpecifiedBlockListNode: public Node
	{
	public:
		explicit AccessSpecifiedBlockListNode( const Location &location ) : Node( location ), last( list.cbefore_begin() )
		{
		}

		~AccessSpecifiedBlockListNode()
		{
			for ( auto i : list )
			{
				delete i;
			}
		}

		forward_list<AccessSpecifiedBlockNode *> list;
		forward_list<AccessSpecifiedBlockNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::ACCESS_SPECIFIED_BLOCK_LIST;
		}

		void AddAccessSpecifiedBlock( AccessSpecifiedBlockNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class ClassNode: public PrimaryExpressionNode
	{
	public:
		explicit ClassNode( const Location &location ) : PrimaryExpressionNode( location ), last( list.cbefore_begin() ), baseClassConstructorCall( nullptr ), include( nullptr ), accessSpecifiedBlockList( nullptr )
		{
		}

		~ClassNode()
		{
			for ( auto i : list )
			{
				delete i;
			}
			delete baseClassConstructorCall;
			delete include;
			delete accessSpecifiedBlockList;
		}

		forward_list<ExpressionNode *> list;
		forward_list<ExpressionNode *>::const_iterator last;
		BaseClassConstructorCallNode *baseClassConstructorCall;
		IncludeNode *include;
		AccessSpecifiedBlockListNode *accessSpecifiedBlockList;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::CLASS;
		}

		void AddArgument( ExpressionNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class PackageNode: public PrimaryExpressionNode
	{
	public:
		PackageNode( const Location &location, const BlockNode * const block ) : PrimaryExpressionNode( location ), block( block )
		{
		}

		~PackageNode()
		{
			delete block;
		}

		const BlockNode * const block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::PACKAGE;
		}
	};

	class ImportNode: public StatementNode
	{
	public:
		explicit ImportNode( const Location &location ) : StatementNode( location ), last( list.cbefore_begin() )
		{
		}

		~ImportNode()
		{
			for ( auto i : list )
			{
				delete i;
			}
		}

		forward_list<IdentifierNode *> list;
		forward_list<IdentifierNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::IMPORT;
		}

		void AddIdentifier( IdentifierNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class SelectionNode: public StatementNode
	{
	public:
		explicit SelectionNode( const Location &location ) : StatementNode( location )
		{
		}

		virtual ~SelectionNode()
		{
		}
	};

	class ElseIfNode: public Node
	{
	public:
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::ELSEIF;
		}
	};

	class IfNode: public SelectionNode
	{
	public:
		explicit IfNode( const Location &location ) : SelectionNode( location ), last( list.cbefore_begin() ), block( nullptr )
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::IF;
		}

		void AddElseIf( ElseIfNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class WhenNode: public Node
	{
	public:
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::WHEN;
		}
	};

	class CaseNode: public SelectionNode
	{
	public:
		explicit CaseNode( const Location &location ) : SelectionNode( location ), value( nullptr ), last( list.cbefore_begin() ), block( nullptr )
		{
		}

		~CaseNode()
		{
			delete value;
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::CASE;
		}

		void AddWhen( WhenNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class IterationNode: public StatementNode
	{
	public:
		explicit IterationNode( const Location &location ) : StatementNode( location )
		{
		}

		virtual ~IterationNode()
		{
		}
	};

	class WhileNode: public IterationNode
	{
	public:
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::WHILE;
		}
	};

	class ForNode: public IterationNode
	{
	public:
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::FOR;
		}
	};

	class ForEachNode: public IterationNode
	{
	public:
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::FOREACH;
		}
	};

	class JumpNode: public StatementNode
	{
	public:
		explicit JumpNode( const Location &location ) : StatementNode( location )
		{
		}

		virtual ~JumpNode()
		{
		}
	};

	class BreakNode: public JumpNode
	{
	public:
		explicit BreakNode( const Location &location ) : JumpNode( location )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::BREAK;
		}
	};

	class NextNode: public JumpNode
	{
	public:
		explicit NextNode( const Location &location ) : JumpNode( location )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::NEXT;
		}
	};

	class ReturnNode: public JumpNode
	{
	public:
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

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		virtual Node::Type GetType() const
		{
			return Node::Type::RETURN;
		}
	};
}

#endif
