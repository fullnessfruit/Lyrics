#include <string>
#include <forward_list>
#include <unordered_map>

#include "Tokenizer.h"
#include "Token.h"
#include "Literal.h"

#include "ErrorCode.h"
#include "Logger.h"

#ifndef INTERPRETER
#define INTERPRETER

namespace lyrics
{
	using std::string;
	using std::forward_list;

	class Interpreter
	{
	public:
		bool Interpret( const string fileName )
		{
			forward_list<Token> token;

			if ( !Tokenizer().Tokenize( fileName, token ) )
			{
				return false;
			}

			mToken = token.cbegin();

			Literal literal = Block();

			return true;
		}

	private:
		forward_list<Token>::const_iterator mToken;
		unordered_map<u16string, Literal> mSymbolTable;

		Literal Block()
		{
//			while ( mToken->type != Token::Type::END && mToken->type != Token::Type::ELSE && mToken->type != Token::Type::ELSEIF && mToken->type != Token::Type::PRIVATE && mToken->type != Token::Type::PUBLIC && mToken->type != Token::Type::PROTECTED && mToken->type != Token::Type::WHEN && mToken->type != Token::Type::END_OF_FILE )
//			{
//				Statement();
//			}
		}

//		Literal Statement()
//		{
//			switch ( mToken->type )
//			{
//			case Token::Type::IF:
//				return If();
//
//			case Token::Type::FOR:
//				return For();
//
//			case Token::Type::RETURN:
//				return Return();
//
//			case Token::Type::BREAK:
//				return Break();
//
//			case Token::Type::WHILE:
//				return While();
//
//			case Token::Type::CASE:
//				return Case();
//
//			case Token::Type::IMPORT:
//				return Import();
//
//			case Token::Type::FOREACH:
//				return ForEach();
//
//			case Token::Type::NEXT:
//				return Next();
//
//			default:
//				return Expression();
//			}
//		}
//
//		Literal PrimaryExpression()
//		{
//			switch ( mToken->type )
//			{
//			case Token::Type::IDENTIFIER:
//				return new IdentifierNode( mToken->location, mToken++->value.identifier );
//
//			case Token::Type::INTEGER_LITERAL:
//				return new IntegerLiteralNode( mToken->location, mToken++->value.integer );
//
//			case Token::Type::STRING_LITERAL:
//				return new StringLiteralNode( mToken->location, mToken++->value.string );
//
//			case Token::Type::BOOLEAN_LITERAL:
//				return new BooleanLiteralNode( mToken->location, mToken++->value.boolean );
//
//			case Token::Type::NULL_LITERAL:
//				return new NullLiteralNode( mToken++->location );
//
//			case Token::Type::REAL_LITERAL:
//				return new RealLiteralNode( mToken->location, mToken++->value.real );
//
//			case Token::Type::DEF:
//				return FunctionLiteral( mToken++ );
//
//			case static_cast<Token::Type>( u'(' ):
//				return ParenthesizedExpression();
//
//			case static_cast<Token::Type>( u'[' ):
//				return ArrayLiteral();
//
//			case static_cast<Token::Type>( u'{' ):
//				return HashLiteral();
//
//			case Token::Type::THIS:
//				return new ThisNode( mToken->location );
//
//			default:
//				Logger::Error( ErrorCode::EXPECTED_PRIMARY_EXPRESSION, mToken->location );
//
//				mToken++;
//
//				return nullptr;
//			}
//		}
//
//		Literal ArrayLiteral()
//		{
//			ArrayLiteralNode *node = new ArrayLiteralNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_ARRAY_LITERAL, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//
//			if ( mToken->type != static_cast<Token::Type>( u']' ) )
//			{
//				for (;;)
//				{
//					node->AddExpression( Expression() );
//
//					if ( mToken->type == static_cast<Token::Type>( u',' ) )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							Logger::Error( ErrorCode::INCOMPLETE_ARRAY_LITERAL, mToken->location );
//							delete node;
//
//							return nullptr;
//						}
//						else if ( mToken->type == static_cast<Token::Type>( u']' ) )
//						{
//							break;
//						}
//					}
//					else if ( mToken->type == static_cast<Token::Type>( u']' ) )
//					{
//						break;
//					}
//					else
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_ARRAY_LITERAL, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//				}
//			}
//
//			mToken++;
//
//			return node;
//		}
//
//		Literal HashLiteral()
//		{
//			HashLiteralNode *node = new HashLiteralNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//
//			if ( mToken->type != static_cast<Token::Type>( u'}' ) )
//			{
//				ExpressionNode *expression;
//
//				for (;;)
//				{
//					auto tToken = mToken;
//
//					expression = Expression();
//
//					if ( mToken->type != static_cast<Token::Type>( u':' ) )
//					{
//						Logger::Error( ErrorCode::EXPECTED_HASH, mToken->location );
//						delete expression;
//						delete node;
//
//						return nullptr;
//					}
//
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
//						delete expression;
//						delete node;
//
//						return nullptr;
//					}
//
//					node->AddHash( new HashNode( tToken->location, expression, Expression() ) );
//
//					if ( mToken->type == static_cast<Token::Type>( u',' ) )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							Logger::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
//							delete node;
//
//							return nullptr;
//						}
//						else if ( mToken->type == static_cast<Token::Type>( u'}' ) )
//						{
//							break;
//						}
//					}
//					else if ( mToken->type == static_cast<Token::Type>( u'}' ) )
//					{
//						break;
//					}
//					else
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//				}
//			}
//
//			mToken++;
//
//			return node;
//		}
//
//		Literal FunctionLiteral( forward_list<Token>::const_iterator token )
//		{
//			if ( mToken->type == static_cast<Token::Type>( u'(' ) )
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//
//					return nullptr;
//				}
//
//				FunctionLiteralNode *node = new FunctionLiteralNode( token->location );
//
//				if ( mToken->type != static_cast<Token::Type>( u')' ) )
//				{
//					ParameterNode *parameter;
//					bool isValueParameter;
//					IdentifierNode *name;
//
//					for (;;)
//					{
//						auto tToken = mToken;
//
//						if ( mToken->type != Token::Type::OUT )
//						{
//							isValueParameter = true;
//						}
//						else
//						{
//							mToken++;
//							if ( mToken->type == Token::Type::END_OF_FILE )
//							{
//								Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//								delete node;
//
//								return nullptr;
//							}
//
//							isValueParameter = false;
//						}
//
//						if ( mToken->type == Token::Type::IDENTIFIER )
//						{
//							name = new IdentifierNode( mToken->location, mToken->value.identifier );
//
//							mToken++;
//							if ( mToken->type == Token::Type::END_OF_FILE )
//							{
//								Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//								delete name;
//								delete node;
//
//								return nullptr;
//							}
//						}
//						else
//						{
//							Logger::Error( ErrorCode::EXPECTED_PARAMETER_NAME, mToken->location );
//							delete node;
//
//							return nullptr;
//						}
//
//						if ( mToken->type != static_cast<Token::Type>( u'=' ) )
//						{
//							if ( isValueParameter )
//							{
//								parameter = new ValueParameterNode( tToken->location, name );
//							}
//							else
//							{
//								parameter = new OutputParameterNode( tToken->location, name );
//							}
//						}
//						else
//						{
//							mToken++;
//							if ( mToken->type == Token::Type::END_OF_FILE )
//							{
//								Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//								delete name;
//								delete node;
//
//								return nullptr;
//							}
//
//							if ( isValueParameter )
//							{
//								parameter = new ValueParameterNode( tToken->location, name, Expression() );
//							}
//							else
//							{
//								Logger::Error( ErrorCode::OUTPUT_PARAMETER_DEFAULT_ARGUMENT, mToken->location );
//								delete name;
//								delete node;
//
//								return nullptr;
//							}
//						}
//
//						node->AddParameter( parameter );
//
//						if ( mToken->type == static_cast<Token::Type>( u',' ) )
//						{
//							mToken++;
//							if ( mToken->type == Token::Type::END_OF_FILE )
//							{
//								Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//								delete node;
//
//								return nullptr;
//							}
//						}
//						else if ( mToken->type == static_cast<Token::Type>( u')' ) )
//						{
//							break;
//						}
//						else
//						{
//							Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//							delete node;
//
//							return nullptr;
//						}
//					}
//				}
//
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//					delete node;
//
//					return nullptr;
//				}
//
//				node->block = Block();
//
//				if ( mToken->type == Token::Type::END )
//				{
//					mToken++;
//
//					return node;
//				}
//				else
//				{
//					Logger::Error( ErrorCode::EXPECTED_END, mToken->location );
//					delete node;
//
//					return nullptr;
//				}
//			}
//			else
//			{
//				Logger::Error( ErrorCode::EXPECTED_PARAMETER, mToken->location );
//
//				return nullptr;
//			}
//		}
//
//		Literal ParenthesizedExpression()
//		{
//			ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->expression = Expression();
//
//			if ( mToken->type == static_cast<Token::Type>( u')' ) )
//			{
//				mToken++;
//
//				return node;
//			}
//			else
//			{
//				Logger::Error( ErrorCode::EXPECTED_RIGHT_PARENTHESIS, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//		}
//
//		Literal PostfixExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = PrimaryExpression();
//
//			if ( expression == nullptr || ( expression->GetType() != Node::Type::IDENTIFIER && expression->GetType() != Node::Type::THIS ) )
//			{
//				return expression;
//			}
//
//			for (;;)
//			{
//				if ( mToken->type == static_cast<Token::Type>( u'[' ) )
//				{
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::EXPECTED_INDEX, mToken->location );
//						delete expression;
//
//						return nullptr;
//					}
//
//					expression = new IndexReferenceNode( tToken->location, expression, Expression() );
//
//					if ( mToken->type == static_cast<Token::Type>( u']' ) )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							Logger::Error( ErrorCode::EXPECTED_INDEX, mToken->location );
//							delete expression;
//
//							return nullptr;
//						}
//					}
//					else
//					{
//						Logger::Error( ErrorCode::EXPECTED_INDEX, mToken->location );
//						delete expression;
//
//						return nullptr;
//					}
//				}
//				else if ( mToken->type == static_cast<Token::Type>( u'(' ) )
//				{
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::EXPECTED_FUNCTION_CALL, mToken->location );
//						delete expression;
//
//						return nullptr;
//					}
//
//					FunctionCallNode *node = new FunctionCallNode( tToken->location, expression );
//
//					if ( mToken->type != static_cast<Token::Type>( u')' ) )
//					{
//						for (;;)
//						{
//							node->AddArgument( Expression() );
//
//							if ( mToken->type == static_cast<Token::Type>( u',' ) )
//							{
//								mToken++;
//								if ( mToken->type == Token::Type::END_OF_FILE )
//								{
//									Logger::Error( ErrorCode::EXPECTED_FUNCTION_CALL, mToken->location );
//									delete node;
//
//									return nullptr;
//								}
//							}
//							else if ( mToken->type == static_cast<Token::Type>( u')' ) )
//							{
//								break;
//							}
//							else
//							{
//								Logger::Error( ErrorCode::EXPECTED_FUNCTION_CALL, mToken->location );
//								delete node;
//
//								return nullptr;
//							}
//						}
//					}
//
//					mToken++;
//
//					return node;
//				}
//				else if ( mToken->type == static_cast<Token::Type>( u'.' ) )
//				{
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::EXPECTED_MEMBER, mToken->location );
//						delete expression;
//
//						return nullptr;
//					}
//
//					if ( mToken->type == Token::Type::IDENTIFIER )
//					{
//						expression = new MemberReferenceNode( tToken->location, expression, new IdentifierNode( mToken->location, mToken->value.identifier ) );
//
//						mToken++;
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							Logger::Error( ErrorCode::EXPECTED_MEMBER, mToken->location );
//							delete expression;
//
//							return nullptr;
//						}
//					}
//					else
//					{
//						Logger::Error( ErrorCode::EXPECTED_MEMBER, mToken->location );
//						delete expression;
//
//						return nullptr;
//					}
//				}
//				else
//				{
//					return expression;
//				}
//			}
//		}
//
//		Literal UnaryExpression()
//		{
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
//
//				return nullptr;
//			}
//			else if ( mToken->type != static_cast<Token::Type>( u'+' ) && mToken->type != static_cast<Token::Type>( u'-' ) && mToken->type != static_cast<Token::Type>( u'~' ) && mToken->type != static_cast<Token::Type>( u'!' ) )
//			{
//				return PostfixExpression();
//			}
//			else
//			{
//				return new UnaryExpressionNode( mToken->location, mToken++->type, UnaryExpression() );
//			}
//		}
//
//		Literal MultiplicativeExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = UnaryExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'*' ) || mToken->type == static_cast<Token::Type>( u'/' ) || mToken->type == static_cast<Token::Type>( u'%' ) )
//			{
//				expression = new MultiplicativeExpressionNode( tToken->location, mToken++->type, expression, UnaryExpression() );
//			}
//
//			return expression;
//		}
//
//		Literal AdditiveExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = MultiplicativeExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'+' ) || mToken->type == static_cast<Token::Type>( u'-' ) )
//			{
//				expression = new AdditiveExpressionNode( tToken->location, mToken++->type, expression, MultiplicativeExpression() );
//			}
//
//			return expression;
//		}
//
//		Literal ShiftExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = AdditiveExpression();
//
//			while ( mToken->type == Token::Type::SHIFT_LEFT || mToken->type == Token::Type::SHIFT_RIGHT )
//			{
//				expression = new ShiftExpressionNode( tToken->location, mToken++->type, expression, AdditiveExpression() );
//			}
//
//			return expression;
//		}
//
//		Literal AndExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = ShiftExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'&' ) )
//			{
//				mToken++;
//
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
//					delete expression;
//
//					return nullptr;
//				}
//
//				expression = new AndExpressionNode( tToken->location, expression, ShiftExpression() );
//			}
//
//			return expression;
//		}
//
//		Literal OrExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = AndExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'|' ) || mToken->type == static_cast<Token::Type>( u'^' ) )
//			{
//				expression = new OrExpressionNode( tToken->location, mToken++->type, expression, AndExpression() );
//			}
//
//			return expression;
//		}
//
//		Literal RelationalExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = OrExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'<' ) || mToken->type == static_cast<Token::Type>( u'>' ) || mToken->type == Token::Type::LESS_THAN_OR_EQUAL || mToken->type == Token::Type::GREATER_THAN_OR_EQUAL )
//			{
//				expression = new RelationalExpressionNode( tToken->location, mToken++->type, expression, OrExpression() );
//			}
//
//			return expression;
//		}
//
//		Literal EqualityExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = RelationalExpression();
//
//			while ( mToken->type == Token::Type::EQUAL || mToken->type == Token::Type::NOT_EQUAL )
//			{
//				expression = new EqualityExpressionNode( tToken->location, mToken++->type, expression, RelationalExpression() );
//			}
//
//			return expression;
//		}
//
//		Literal LogicalAndExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = EqualityExpression();
//
//			while ( mToken->type == Token::Type::AND )
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
//					delete expression;
//
//					return nullptr;
//				}
//
//				expression = new LogicalAndExpressionNode( tToken->location, expression, EqualityExpression() );
//			}
//
//			return expression;
//		}
//
//		Literal LogicalOrExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = LogicalAndExpression();
//
//			while ( mToken->type == Token::Type::OR )
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
//					delete expression;
//
//					return nullptr;
//				}
//
//				expression = new LogicalOrExpressionNode( tToken->location, expression, LogicalAndExpression() );
//			}
//
//			return expression;
//		}
//
//		Literal AssignmentExpression()
//		{
//			auto tToken = mToken;
//
//			switch ( mToken->type )
//			{
//			case Token::Type::DEF:
//				mToken++;
//				if ( mToken->type == Token::Type::IDENTIFIER )
//				{
//					IdentifierNode *identifier = new IdentifierNode( mToken->location, mToken->value.identifier );
//
//					mToken++;
//					if ( mToken->type == static_cast<Token::Type>( u'.' ) )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::IDENTIFIER )
//						{
//							IdentifierNode *member = new IdentifierNode( mToken->location, mToken->value.identifier );
//
//							mToken++;
//							if ( mToken->type == static_cast<Token::Type>( u'(' ) )
//							{
//								return new AssignmentExpressionNode( tToken->location, new MemberReferenceNode( identifier->location, identifier, member ), FunctionLiteral( tToken ) );
//							}
//							else
//							{
//								Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//								delete identifier;
//								delete member;
//
//								return nullptr;
//							}
//						}
//						else
//						{
//							Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//							delete identifier;
//
//							return nullptr;
//						}
//					}
//					else if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//						delete identifier;
//
//						return nullptr;
//					}
//					else
//					{
//						return new AssignmentExpressionNode( tToken->location, identifier, FunctionLiteral( tToken ) );
//					}
//				}
//				else if ( mToken->type == Token::Type::THIS )
//				{
//					ThisNode *thisNode = new ThisNode( mToken->location );
//
//					mToken++;
//					if ( mToken->type == static_cast<Token::Type>( u'.' ) )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::IDENTIFIER )
//						{
//							IdentifierNode *identifier = new IdentifierNode( mToken->location, mToken->value.identifier );
//
//							mToken++;
//							if ( mToken->type == static_cast<Token::Type>( u'(' ) )
//							{
//								return new AssignmentExpressionNode( tToken->location, new MemberReferenceNode( thisNode->location, thisNode, identifier ), FunctionLiteral( tToken ) );
//							}
//							else
//							{
//								Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//								delete thisNode;
//								delete identifier;
//
//								return nullptr;
//							}
//						}
//						else
//						{
//							Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//							delete thisNode;
//
//							return nullptr;
//						}
//					}
//					else
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//						delete thisNode;
//
//						return nullptr;
//					}
//				}
//				else if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
//
//					return nullptr;
//				}
//				else
//				{
//					mToken = tToken;
//
//					return PrimaryExpression();
//				}
//
//			case Token::Type::CLASS:
//				return Class();
//
//			case Token::Type::PACKAGE:
//				return Package();
//
//			case Token::Type::END_OF_FILE:
//				Logger::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
//
//				return nullptr;
//
//			default:
//				ExpressionNode *expression = LogicalOrExpression();
//
//				if ( expression == nullptr || mToken->type != static_cast<Token::Type>( u'=' ) )
//				{
//					return expression;
//				}
//				else
//				{
//					if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::MEMBER_REFERENCE || expression->GetType() == Node::Type::INDEX_REFERENCE )
//					{
//						mToken++;
//
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							Logger::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
//							delete expression;
//
//							return nullptr;
//						}
//
//						return new AssignmentExpressionNode( tToken->location, expression, AssignmentExpression() );
//					}
//					else
//					{
//						Logger::Error( ErrorCode::EXPECTED_LHS, mToken->location );
//						delete expression;
//
//						return nullptr;
//					}
//				}
//			}
//		}
//
//		Literal Class()
//		{
//			auto tToken = mToken;
//
//			mToken++;
//			if ( mToken->type == Token::Type::IDENTIFIER )
//			{
//				IdentifierNode *name = new IdentifierNode( mToken->location, mToken->value.identifier );
//				ClassNode *node = new ClassNode( tToken->location );
//
//				mToken++;
//				if ( mToken->type == static_cast<Token::Type>( u'(' ) )
//				{
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
//						delete node;
//						delete name;
//
//						return nullptr;
//					}
//
//					if ( mToken->type != static_cast<Token::Type>( u')' ) )
//					{
//						for (;;)
//						{
//							node->AddArgument( Expression() );
//
//							if ( mToken->type == static_cast<Token::Type>( u',' ) )
//							{
//								mToken++;
//								if ( mToken->type == Token::Type::END_OF_FILE )
//								{
//									Logger::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
//									delete node;
//									delete name;
//
//									return nullptr;
//								}
//							}
//							else if ( mToken->type == static_cast<Token::Type>( u')' ) )
//							{
//								break;
//							}
//							else
//							{
//								Logger::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
//								delete node;
//								delete name;
//
//								return nullptr;
//							}
//						}
//					}
//				}
//				else
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
//					delete node;
//					delete name;
//
//					return nullptr;
//				}
//
//				mToken++;
//				if ( mToken->type == static_cast<Token::Type>( u':' ) )
//				{
//					node->baseClassConstructorCall = new BaseClassConstructorCallNode( mToken->location );
//
//					mToken++;
//					if ( mToken->type == Token::Type::IDENTIFIER )
//					{
//						node->baseClassConstructorCall->baseClass = new IdentifierNode( mToken->location, mToken->value.identifier );
//
//						mToken++;
//						if ( mToken->type == static_cast<Token::Type>( u'(' ) )
//						{
//							mToken++;
//							if ( mToken->type == Token::Type::END_OF_FILE )
//							{
//								Logger::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
//								delete node;
//								delete name;
//
//								return nullptr;
//							}
//
//							if ( mToken->type != static_cast<Token::Type>( u')' ) )
//							{
//								for (;;)
//								{
//									node->baseClassConstructorCall->AddArgument( Expression() );
//
//									if ( mToken->type == static_cast<Token::Type>( u',' ) )
//									{
//										mToken++;
//										if ( mToken->type == Token::Type::END_OF_FILE )
//										{
//											Logger::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
//											delete node;
//											delete name;
//
//											return nullptr;
//										}
//									}
//									else if ( mToken->type == static_cast<Token::Type>( u')' ) )
//									{
//										break;
//									}
//									else
//									{
//										Logger::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
//										delete node;
//										delete name;
//
//										return nullptr;
//									}
//								}
//							}
//						}
//						else
//						{
//							Logger::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
//							delete node;
//							delete name;
//
//							return nullptr;
//						}
//					}
//					else
//					{
//						Logger::Error( ErrorCode::EXPECTED_BASE_CLASS, mToken->location );
//						delete node;
//						delete name;
//
//						return nullptr;
//					}
//				}
//
//				mToken++;
//				if ( mToken->type == Token::Type::INCLUDE )
//				{
//					node->include = Include();
//				}
//
//				Token::Type accessSpecifier;
//				auto tToken = mToken;
//
//				node->accessSpecifiedBlockList = new AccessSpecifiedBlockListNode( mToken->location );
//
//				switch ( mToken->type )
//				{
//				case Token::Type::PRIVATE:
//				case Token::Type::PUBLIC:
//				case Token::Type::PROTECTED:
//					accessSpecifier = mToken->type;
//					mToken++;
//					break;
//
//				default:
//					accessSpecifier = Token::Type::PUBLIC;
//					break;
//				}
//
//				for (;;)
//				{
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
//						delete node;
//						delete name;
//
//						return nullptr;
//					}
//
//					node->accessSpecifiedBlockList->AddAccessSpecifiedBlock( new AccessSpecifiedBlockNode( tToken->location, accessSpecifier, Block() ) );
//
//					switch ( mToken->type )
//					{
//					case Token::Type::PRIVATE:
//					case Token::Type::PUBLIC:
//					case Token::Type::PROTECTED:
//						accessSpecifier = mToken->type;
//						tToken = mToken++;
//						break;
//
//					case Token::Type::END:
//						mToken++;
//
//						return new AssignmentExpressionNode( tToken->location, name, node );
//
//					default:
//						Logger::Error( ErrorCode::EXPECTED_END, mToken->location );
//						delete node;
//						delete name;
//
//						return nullptr;
//					}
//				}
//			}
//			else
//			{
//				Logger::Error( ErrorCode::EXPECTED_CLASS_NAME, mToken->location );
//
//				return nullptr;
//			}
//		}
//
//		Literal Include()
//		{
//			IncludeNode *node = new IncludeNode( mToken->location );
//
//			do
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::IDENTIFIER )
//				{
//					node->AddPackage( new IdentifierNode( mToken->location, mToken++->value.identifier ) );
//				}
//				else
//				{
//					Logger::Error( ErrorCode::EXPECTED_PACKAGE, mToken->location );
//					delete node;
//
//					return nullptr;
//				}
//			}
//			while ( mToken->type == static_cast<Token::Type>( u',' ) );
//
//			return node;
//		}
//
//		Literal Package()
//		{
//			auto tToken = mToken;
//
//			mToken++;
//			if ( mToken->type == Token::Type::IDENTIFIER )
//			{
//				IdentifierNode *name = new IdentifierNode( mToken->location, mToken->value.identifier );
//				PackageNode *node = new PackageNode( tToken->location, Block() );
//
//				mToken++;
//				if ( mToken->type == Token::Type::END )
//				{
//					mToken++;
//
//					return new AssignmentExpressionNode( tToken->location, name, node );
//				}
//				else
//				{
//					Logger::Error( ErrorCode::EXPECTED_END, mToken->location );
//					delete node;
//					delete name;
//
//					return nullptr;
//				}
//			}
//			else
//			{
//				Logger::Error( ErrorCode::EXPECTED_PACKAGE_NAME, mToken->location );
//
//				return nullptr;
//			}
//		}
//
//		Literal Expression()
//		{
//			return AssignmentExpression();
//		}
//
//		Literal Import()
//		{
//			ImportNode *node = new ImportNode( mToken->location );
//
//			do
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::IDENTIFIER )
//				{
//					node->AddIdentifier( new IdentifierNode( mToken->location, mToken++->value.identifier ) );
//				}
//				else
//				{
//					Logger::Error( ErrorCode::EXPECTED_IDENTIFIER, mToken->location );
//					delete node;
//
//					return nullptr;
//				}
//			}
//			while ( mToken->type == static_cast<Token::Type>( u'.' ) );
//
//			return node;
//		}
//
//		Literal If()
//		{
//			IfNode *node = new IfNode( mToken->location );
//			ElseIfNode *elseIfNode;
//
//			for (;;)
//			{
//				elseIfNode = new ElseIfNode( mToken->location );
//
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_IF_STATEMENT, mToken->location );
//					delete elseIfNode;
//					delete node;
//
//					return nullptr;
//				}
//
//				elseIfNode->condition = Expression();
//
//				if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
//				{
//					mToken++;
//				}
//
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_IF_STATEMENT, mToken->location );
//					delete elseIfNode;
//					delete node;
//
//					return nullptr;
//				}
//
//				elseIfNode->block = Block();
//				node->AddElseIf( elseIfNode );
//
//				if ( mToken->type == Token::Type::END )
//				{
//					mToken++;
//
//					return node;
//				}
//				else if ( mToken->type == Token::Type::ELSE )
//				{
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_IF_STATEMENT, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->block = Block();
//
//					if ( mToken->type == Token::Type::END )
//					{
//						mToken++;
//
//						return node;
//					}
//					else
//					{
//						Logger::Error( ErrorCode::EXPECTED_END, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//				}
//				else if ( mToken->type != Token::Type::ELSEIF )
//				{
//					Logger::Error( ErrorCode::EXPECTED_END_ELSE_ELSEIF, mToken->location );
//					delete node;
//
//					return nullptr;
//				}
//			}
//		}
//
//		Literal Case()
//		{
//			CaseNode *node = new CaseNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_CASE_STATEMENT, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->value = Expression();
//
//			if ( mToken->type == Token::Type::WHEN )
//			{
//				WhenNode *whenNode;
//
//				for (;;)
//				{
//					whenNode = new WhenNode( mToken->location );
//
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_CASE_STATEMENT, mToken->location );
//						delete whenNode;
//						delete node;
//
//						return nullptr;
//					}
//
//					whenNode->condition = Expression();
//
//					if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
//					{
//						mToken++;
//					}
//
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_CASE_STATEMENT, mToken->location );
//						delete whenNode;
//						delete node;
//
//						return nullptr;
//					}
//
//					whenNode->block = Block();
//					node->AddWhen( whenNode );
//
//					if ( mToken->type == Token::Type::ELSE )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							Logger::Error( ErrorCode::INCOMPLETE_CASE_STATEMENT, mToken->location );
//							delete node;
//
//							return nullptr;
//						}
//
//						node->block = Block();
//
//						if ( mToken->type == Token::Type::END )
//						{
//							mToken++;
//
//							return node;
//						}
//						else
//						{
//							Logger::Error( ErrorCode::EXPECTED_END, mToken->location );
//							delete node;
//
//							return nullptr;
//						}
//					}
//					else if ( mToken->type == Token::Type::END )
//					{
//						mToken++;
//
//						return node;
//					}
//					else if ( mToken->type != Token::Type::WHEN )
//					{
//						Logger::Error( ErrorCode::EXPECTED_WHEN_ELSE_ELSEIF, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//				}
//			}
//			else
//			{
//				Logger::Error( ErrorCode::EXPECTED_WHEN, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//		}
//
//		Literal While()
//		{
//			WhileNode *node = new WhileNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_WHILE_STATEMENT, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->condition = Expression();
//
//			if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
//			{
//				mToken++;
//			}
//
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_WHILE_STATEMENT, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->block = Block();
//
//			if ( mToken->type == Token::Type::END )
//			{
//				mToken++;
//
//				return node;
//			}
//			else
//			{
//				Logger::Error( ErrorCode::EXPECTED_END, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//		}
//
//		Literal For()
//		{
//			ForNode *node = new ForNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->initializer = Expression();
//
//			if ( mToken->type == static_cast<Token::Type>( u',' ) )
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
//					delete node;
//
//					return nullptr;
//				}
//
//				node->condition = Expression();
//
//				if ( mToken->type == static_cast<Token::Type>( u',' ) )
//				{
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->iterator = Expression();
//
//					if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
//					{
//						mToken++;
//					}
//
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->block = Block();
//
//					if ( mToken->type == Token::Type::END )
//					{
//						mToken++;
//
//						return node;
//					}
//					else
//					{
//						Logger::Error( ErrorCode::EXPECTED_END, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//				}
//				else
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
//					delete node;
//
//					return nullptr;
//				}
//			}
//			else
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//		}
//
//		Literal ForEach()
//		{
//			ForEachNode *node = new ForEachNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				Logger::Error( ErrorCode::INCOMPLETE_FOREACH_STATEMENT, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->variable = Expression();
//
//			if ( node->variable ->GetType() == Node::Type::IDENTIFIER || node->variable ->GetType() == Node::Type::MEMBER_REFERENCE || node->variable ->GetType() == Node::Type::INDEX_REFERENCE )
//			{
//				if ( mToken->type == Token::Type::IN )
//				{
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_FOREACH_STATEMENT, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->collection = Expression();
//
//					if ( node->collection->GetType() != Node::Type::IDENTIFIER && node->collection->GetType() != Node::Type::MEMBER_REFERENCE && node->collection->GetType() != Node::Type::INDEX_REFERENCE )
//					{
//						Logger::Error( ErrorCode::EXPECTED_LHS, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//
//					if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
//					{
//						mToken++;
//					}
//
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						Logger::Error( ErrorCode::INCOMPLETE_FOREACH_STATEMENT, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->block = Block();
//
//					if ( mToken->type == Token::Type::END )
//					{
//						mToken++;
//
//						return node;
//					}
//					else
//					{
//						Logger::Error( ErrorCode::EXPECTED_END, mToken->location );
//						delete node;
//
//						return nullptr;
//					}
//				}
//				else
//				{
//					Logger::Error( ErrorCode::INCOMPLETE_FOREACH_STATEMENT, mToken->location );
//					delete node;
//
//					return nullptr;
//				}
//			}
//			else
//			{
//				Logger::Error( ErrorCode::EXPECTED_LHS, mToken->location );
//				delete node;
//
//				return nullptr;
//			}
//		}
//
//		Literal Break()
//		{
//			return new BreakNode( mToken++->location );
//		}
//
//		Literal Next()
//		{
//			return new NextNode( mToken++->location );
//		}
//
//		Literal Return()
//		{
//			auto tToken = mToken;
//
//			mToken++;
//
//			if ( mToken->type == Token::Type::END || mToken->type == Token::Type::ELSE || mToken->type == Token::Type::ELSEIF || mToken->type == Token::Type::PRIVATE || mToken->type == Token::Type::PUBLIC || mToken->type == Token::Type::PROTECTED || mToken->type == Token::Type::WHEN || mToken->type == Token::Type::END_OF_FILE )
//			{
//				return new ReturnNode( tToken->location );
//			}
//			else
//			{
//				return new ReturnNode( tToken->location, Expression() );
//			}
//		}
	};
}

#endif
