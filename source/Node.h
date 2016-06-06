#include <string>
#include <forward_list>

#include "Token.h"
#include "Location.h"
#include "Literal.h"

#include "Visitor.h"
#include "Element.h"

#include "Utility.h"

#ifndef NODE
#define NODE

namespace lyrics
{
	using std::u32string;
	using std::forward_list;

	class Node : public Element
	{
	public:
		enum struct Type
		{
			BLOCK,
						IDENTIFIER, THIS, NULL_LITERAL, BOOLEAN_LITERAL, INTEGER_LITERAL, REAL_LITERAL, STRING_LITERAL, ARRAY_LITERAL, HASH_LITERAL, ROUTINE_LITERAL, PARENTHESIZED_EXPRESSION,
							HASH, VALUE_PARAMETER, OUTPUT_PARAMETER,
						INDEX_REFERENCE, ROUTINE_CALL, MEMBER_REFERENCE,
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

		explicit Node( const Location &location, const Type type ) : location( location ), type( type )
		{
		}

		virtual ~Node()
		{
		}

		const Location location;
		const Type type;
	};

	class BlockNode;
	class StatementNode;
		class ExpressionNode;
			class PrimaryExpressionNode;
				class IdentifierNode;
				class ThisNode;
				class LiteralNode;
				class ArrayLiteralNode;
				class HashLiteralNode;
					class HashNode;
				class RoutineLiteralNode;
					class ParameterNode;
						class ValueParameterNode;
						class OutputParameterNode;
				class ParenthesizedExpressionNode;
			class PostfixExpressionNode;
				class IndexReferenceNode;
				class RoutineCallNode;
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
		StatementNode( const Location &location, const Type type ) : Node( location, type )
		{
		}

		virtual ~StatementNode()
		{
		}
	};

	class BlockNode: public Node
	{
	public:
		explicit BlockNode( const Location &location ) : Node( location, Type::BLOCK ), last( list.cbefore_begin() )
		{
		}

		~BlockNode()
		{
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
		}

		forward_list<StatementNode *> list;
		forward_list<StatementNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddStatement( StatementNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class ExpressionNode: public StatementNode
	{
	public:
		ExpressionNode( const Location &location, const Type type ) : StatementNode( location, type )
		{
		}

		virtual ~ExpressionNode()
		{
		}
	};

	class PrimaryExpressionNode: public ExpressionNode
	{
	public:
		PrimaryExpressionNode( const Location &location, const Type type ) : ExpressionNode( location, type )
		{
		}

		virtual ~PrimaryExpressionNode()
		{
		}
	};

	class IdentifierNode: public PrimaryExpressionNode
	{
	public:
		IdentifierNode( const Location &location, const u32string * const identifier ) : PrimaryExpressionNode( location, Type::IDENTIFIER ), identifier( identifier )
		{
		}

		~IdentifierNode()
		{
			delete identifier;
		}

		const u32string * const identifier;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}
	};

	class ThisNode: public PrimaryExpressionNode
	{
	public:
		explicit ThisNode( const Location &location ) : PrimaryExpressionNode( location, Type::THIS )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}
	};

	class NullLiteralNode: public PrimaryExpressionNode
	{
	public:
		explicit NullLiteralNode( const Location &location ) : PrimaryExpressionNode( location, Type::NULL_LITERAL )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}
	};

	class BooleanLiteralNode: public PrimaryExpressionNode
	{
	public:
		BooleanLiteralNode( const Location &location, const bool boolean ) : PrimaryExpressionNode( location, Type::BOOLEAN_LITERAL ), boolean( boolean )
		{
		}

		const bool boolean;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}
	};

	class IntegerLiteralNode: public PrimaryExpressionNode
	{
	public:
		IntegerLiteralNode( const Location &location, const long long integer ) : PrimaryExpressionNode( location, Type::INTEGER_LITERAL ), integer( integer )
		{
		}

		const int integer;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}
	};

	class RealLiteralNode: public PrimaryExpressionNode
	{
	public:
		RealLiteralNode( const Location &location, const double real ) : PrimaryExpressionNode( location, Type::REAL_LITERAL ), real( real )
		{
		}

		const double real;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}
	};

	class StringLiteralNode: public PrimaryExpressionNode
	{
	public:
		StringLiteralNode( const Location &location, u32string * const string ) : PrimaryExpressionNode( location, Type::STRING_LITERAL ), string( string )
		{
		}

		u32string * const string;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}
	};

	class ArrayLiteralNode: public PrimaryExpressionNode
	{
	public:
		explicit ArrayLiteralNode( const Location &location ) : PrimaryExpressionNode( location, Type::ARRAY_LITERAL ), last( list.cbefore_begin() )
		{
		}

		~ArrayLiteralNode()
		{
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
		}

		forward_list<ExpressionNode *> list;
		forward_list<ExpressionNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddExpression( ExpressionNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class HashNode: public Node
	{
	public:
		HashNode( const Location &location, const ExpressionNode * const key, const ExpressionNode * const value ) : Node( location, Type::HASH ), key( key ), value( value )
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
	};

	class HashLiteralNode: public PrimaryExpressionNode
	{
	public:
		explicit HashLiteralNode( const Location &location ) : PrimaryExpressionNode( location, Type::HASH_LITERAL ), last( list.cbefore_begin() )
		{
		}

		~HashLiteralNode()
		{
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
		}

		forward_list<HashNode *> list;
		forward_list<HashNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddHash( HashNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class ParameterNode: public Node
	{
	public:
		ParameterNode( const Location &location, const Type type, const IdentifierNode * const name ) : Node( location, type ), name( name )
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
		ValueParameterNode( const Location &location, const IdentifierNode * const name ) : ParameterNode( location, Type::VALUE_PARAMETER, name ), defalutArgument( nullptr )
		{
		}

		ValueParameterNode( const Location &location, const IdentifierNode * const name, const ExpressionNode * const defalutArgument ) : ParameterNode( location, Type::VALUE_PARAMETER, name ), defalutArgument( defalutArgument )
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
	};

	class OutputParameterNode: public ParameterNode
	{
	public:
		OutputParameterNode( const Location &location, const IdentifierNode * const name ) : ParameterNode( location, Type::OUTPUT_PARAMETER, name )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}
	};

	class RoutineLiteralNode: public PrimaryExpressionNode
	{
	public:
		explicit RoutineLiteralNode( const Location &location ) : PrimaryExpressionNode( location, Type::ROUTINE_LITERAL ), last( list.cbefore_begin() ), block( nullptr )
		{
		}

		~RoutineLiteralNode()
		{
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
			Utility::SafeDelete( block );
		}

		forward_list<ParameterNode *> list;
		forward_list<ParameterNode *>::const_iterator last;
		BlockNode *block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddParameter( ParameterNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class ParenthesizedExpressionNode: public PrimaryExpressionNode
	{
	public:
		ParenthesizedExpressionNode( const Location &location ) : PrimaryExpressionNode( location, Type::PARENTHESIZED_EXPRESSION ), expression( nullptr )
		{
		}

		~ParenthesizedExpressionNode()
		{
			Utility::SafeDelete( expression );
		}

		ExpressionNode *expression;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}
	};

	class PostfixExpressionNode: public ExpressionNode
	{
	public:
		PostfixExpressionNode( const Location &location, const Type type, const ExpressionNode * const expression ) : ExpressionNode( location, type ), expression( expression )
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
		IndexReferenceNode( const Location &location, const ExpressionNode * const expression, const ExpressionNode * const index ) : PostfixExpressionNode( location, Type::INDEX_REFERENCE, expression ), index( index )
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
	};

	class RoutineCallNode: public PostfixExpressionNode
	{
	public:
		RoutineCallNode( const Location &location, const ExpressionNode * const expression ) : PostfixExpressionNode( location, Type::ROUTINE_CALL, expression ), last( list.cbefore_begin() )
		{
		}

		~RoutineCallNode()
		{
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
		}

		forward_list<ExpressionNode *> list;
		forward_list<ExpressionNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddArgument( ExpressionNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class MemberReferenceNode: public PostfixExpressionNode
	{
	public:
		MemberReferenceNode( const Location &location, const ExpressionNode * const expression, const IdentifierNode * const member ) : PostfixExpressionNode( location, Type::MEMBER_REFERENCE, expression ), member( member )
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
	};

	class UnaryExpressionNode: public ExpressionNode
	{
	public:
		UnaryExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const expression ) : ExpressionNode( location, Type::UNARY_EXPRESSION ), op( op ), expression( expression )
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
	};

	class MultiplicativeExpressionNode: public ExpressionNode
	{
	public:
		MultiplicativeExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location, Type::MULTIPLICATIVE_EXPRESSION ), op( op ), left( left ), right( right )
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
	};

	class AdditiveExpressionNode: public ExpressionNode
	{
	public:
		AdditiveExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location, Type::ADDITIVE_EXPRESSION ), op( op ), left( left ), right( right )
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
	};

	class ShiftExpressionNode: public ExpressionNode
	{
	public:
		ShiftExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location, Type::SHIFT_EXPRESSION ), op( op ), left( left ), right( right )
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
	};

	class AndExpressionNode: public ExpressionNode
	{
	public:
		AndExpressionNode( const Location &location, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location, Type::AND_EXPRESSION ), left( left ), right( right )
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
	};

	class OrExpressionNode: public ExpressionNode
	{
	public:
		OrExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location, Type::OR_EXPRESSION ), op( op ), left( left ), right( right )
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
	};

	class RelationalExpressionNode: public ExpressionNode
	{
	public:
		RelationalExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location, Type::RELATIONAL_EXPRESSION ), op( op ), left( left ), right( right )
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
	};

	class EqualityExpressionNode: public ExpressionNode
	{
	public:
		EqualityExpressionNode( const Location &location, const Token::Type op, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location, Type::EQUALITY_EXPRESSION ), op( op ), left( left ), right( right )
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
	};

	class LogicalAndExpressionNode: public ExpressionNode
	{
	public:
		LogicalAndExpressionNode( const Location &location, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location, Type::LOGICAL_AND_EXPRESSION ), left( left ), right( right )
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
	};

	class LogicalOrExpressionNode: public ExpressionNode
	{
	public:
		LogicalOrExpressionNode( const Location &location, const ExpressionNode * const left, const ExpressionNode * const right ) : ExpressionNode( location, Type::LOGICAL_OR_EXPRESSION ), left( left ), right( right )
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
	};

	class AssignmentExpressionNode: public ExpressionNode
	{
	public:
		AssignmentExpressionNode( const Location &location, const ExpressionNode * const lhs, const ExpressionNode * const rhs ) : ExpressionNode( location, Type::ASSIGNMENT_EXPRESSION ), lhs( lhs ), rhs( rhs )
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
	};

	class BaseClassConstructorCallNode: public Node
	{
	public:
		explicit BaseClassConstructorCallNode( const Location &location ) : Node( location, Type::BASE_CLASS_CONSTRUCTOR_CALL ), baseClass( nullptr ), last( list.cbefore_begin() )
		{
		}

		~BaseClassConstructorCallNode()
		{
			Utility::SafeDelete( baseClass );
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
		}

		IdentifierNode *baseClass;
		forward_list<ExpressionNode *> list;
		forward_list<ExpressionNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddArgument( ExpressionNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class IncludeNode: public Node
	{
	public:
		explicit IncludeNode( const Location &location ) : Node( location, Type::INCLUDE ), last( list.cbefore_begin() )
		{
		}

		~IncludeNode()
		{
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
		}

		forward_list<IdentifierNode *> list;
		forward_list<IdentifierNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddPackage( IdentifierNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class ClassNode: public PrimaryExpressionNode
	{
	public:
		explicit ClassNode( const Location &location ) : PrimaryExpressionNode( location, Type::CLASS ), last( list.cbefore_begin() ), baseClassConstructorCall( nullptr ), include( nullptr ), block( nullptr )
		{
		}

		~ClassNode()
		{
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
			Utility::SafeDelete( baseClassConstructorCall );
			Utility::SafeDelete( include );
			Utility::SafeDelete( block );
		}

		forward_list<ExpressionNode *> list;
		forward_list<ExpressionNode *>::const_iterator last;
		BaseClassConstructorCallNode *baseClassConstructorCall;
		IncludeNode *include;
		BlockNode *block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddArgument( ExpressionNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class PackageNode: public PrimaryExpressionNode
	{
	public:
		PackageNode( const Location &location, const BlockNode * const block ) : PrimaryExpressionNode( location, Type::PACKAGE ), block( block )
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
	};

	class ImportNode: public StatementNode
	{
	public:
		explicit ImportNode( const Location &location ) : StatementNode( location, Type::IMPORT ), last( list.cbefore_begin() )
		{
		}

		~ImportNode()
		{
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
		}

		forward_list<IdentifierNode *> list;
		forward_list<IdentifierNode *>::const_iterator last;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddIdentifier( IdentifierNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class SelectionNode: public StatementNode
	{
	public:
		SelectionNode( const Location &location, const Type type ) : StatementNode( location, type )
		{
		}

		virtual ~SelectionNode()
		{
		}
	};

	class ElseIfNode: public Node
	{
	public:
		explicit ElseIfNode( const Location &location ) : Node( location, Type::ELSEIF ), condition( nullptr ), block( nullptr )
		{
		}

		~ElseIfNode()
		{
			Utility::SafeDelete( condition );
			Utility::SafeDelete( block );
		}

		ExpressionNode *condition;
		BlockNode *block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}
	};

	class IfNode: public SelectionNode
	{
	public:
		explicit IfNode( const Location &location ) : SelectionNode( location, Type::IF ), last( list.cbefore_begin() ), block( nullptr )
		{
		}

		~IfNode()
		{
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
			Utility::SafeDelete( block );
		}

		forward_list<ElseIfNode *> list;
		forward_list<ElseIfNode *>::const_iterator last;
		BlockNode *block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddElseIf( ElseIfNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class WhenNode: public Node
	{
	public:
		explicit WhenNode( const Location &location ) : Node( location, Type::WHEN ), condition( nullptr ), block( nullptr )
		{
		}

		~WhenNode()
		{
			Utility::SafeDelete( condition );
			Utility::SafeDelete( block );
		}

		ExpressionNode *condition;
		BlockNode *block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}
	};

	class CaseNode: public SelectionNode
	{
	public:
		explicit CaseNode( const Location &location ) : SelectionNode( location, Type::CASE ), value( nullptr ), last( list.cbefore_begin() ), block( nullptr )
		{
		}

		~CaseNode()
		{
			Utility::SafeDelete( value );
			for ( auto i : list )
			{
				Utility::SafeDelete( i );
			}
			Utility::SafeDelete( block );
		}

		ExpressionNode *value;
		forward_list<WhenNode *> list;
		forward_list<WhenNode *>::const_iterator last;
		BlockNode *block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}

		void AddWhen( WhenNode * const node )
		{
			last = list.insert_after( last, node );
		}
	};

	class IterationNode: public StatementNode
	{
	public:
		IterationNode( const Location &location, const Type type ) : StatementNode( location, type )
		{
		}

		virtual ~IterationNode()
		{
		}
	};

	class WhileNode: public IterationNode
	{
	public:
		explicit WhileNode( const Location &location ) : IterationNode( location, Type::WHILE ), condition( nullptr ), block( nullptr )
		{
		}

		~WhileNode()
		{
			Utility::SafeDelete( condition );
			Utility::SafeDelete( block );
		}

		ExpressionNode *condition;
		BlockNode *block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}
	};

	class ForNode: public IterationNode
	{
	public:
		explicit ForNode( const Location &location ) : IterationNode( location, Type::FOR ), initializer( nullptr ), condition( nullptr ), iterator( nullptr ), block( nullptr )
		{
		}

		~ForNode()
		{
			Utility::SafeDelete( initializer );
			Utility::SafeDelete( condition );
			Utility::SafeDelete( iterator );
			Utility::SafeDelete( block );
		}

		ExpressionNode *initializer;
		ExpressionNode *condition;
		ExpressionNode *iterator;
		BlockNode *block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}
	};

	class ForEachNode: public IterationNode
	{
	public:
		explicit ForEachNode( const Location &location ) : IterationNode( location, Type::FOREACH ), variable( nullptr ), collection( nullptr ), block( nullptr )
		{
		}

		~ForEachNode()
		{
			Utility::SafeDelete( variable );
			Utility::SafeDelete( collection );
			Utility::SafeDelete( block );
		}

		ExpressionNode *variable;
		ExpressionNode *collection;
		BlockNode *block;

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit( this );
		}
	};

	class JumpNode: public StatementNode
	{
	public:
		JumpNode( const Location &location, const Type type ) : StatementNode( location, type )
		{
		}

		virtual ~JumpNode()
		{
		}
	};

	class BreakNode: public JumpNode
	{
	public:
		explicit BreakNode( const Location &location ) : JumpNode( location, Type::BREAK )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}
	};

	class NextNode: public JumpNode
	{
	public:
		explicit NextNode( const Location &location ) : JumpNode( location, Type::NEXT )
		{
		}

		virtual bool Accept( Visitor &visitor ) const
		{
			return visitor.Visit();
		}
	};

	class ReturnNode: public JumpNode
	{
	public:
		explicit ReturnNode( const Location &location ) : JumpNode( location, Type::RETURN ), value( nullptr )
		{
		}

		ReturnNode( const Location &location, const ExpressionNode * const value ) : JumpNode( location, Type::RETURN ), value( value )
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
	};
}

#endif
