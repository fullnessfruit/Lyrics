#include <string>
#include <forward_list>
#include <unordered_map>

#include "Tokenizer.h"
#include "Token.h"
#include "Literal.h"

#include "ErrorCode.h"
#include "FatalErrorCode.h"
#include "Logger.h"

#ifndef INTERPRETER
#define INTERPRETER

namespace lyrics
{
	using std::string;
	using std::forward_list;
	using std::unordered_map;

	class Interpreter
	{
	public:
		static bool Interpret( const Option &option )
		{
			if ( option.GetSourceFileName().empty() )
			{
				ErrorHandler::FatalError( FatalErrorCode::NO_INPUT_FILES );
				return false;
			}

			forward_list<Token> tokenList;

			if ( !Tokenizer::Tokenize( option.GetSourceFileName(), tokenList ) )
			{
				return false;
			}

			mToken = tokenList.cbegin();

			Literal returnCode = Interpreter::Block();
			switch (returnCode.type)
			{
			case Literal::Type::INTEGER:
				Interpreter::mReturnCode = returnCode.value.integer;
				break;

			case Literal::Type::NULL_LITERAL:
				Interpreter::mReturnCode = 0;
				break;

			case Literal::Type::BOOLEAN:
				Interpreter::mReturnCode = returnCode.value.boolean;
				break;

			case Literal::Type::REAL:
				Interpreter::mReturnCode = returnCode.value.real;
				break;

			default:
				Interpreter::mReturnCode = ( bool )returnCode.value.integer;
			}

			return true;
		}

		static int GetReturnCode()
		{
			return Interpreter::mReturnCode;
		}

	private:
		static forward_list<Token>::const_iterator mToken;
		static unordered_map<u16string, Literal> mSymbolTable;
		static int mReturnCode;

		static Literal Block()
		{
			Literal returnCode;

			while ( mToken->type != Token::Type::END && mToken->type != Token::Type::ELSE && mToken->type != Token::Type::ELSEIF && mToken->type != Token::Type::PRIVATE && mToken->type != Token::Type::PUBLIC && mToken->type != Token::Type::PROTECTED && mToken->type != Token::Type::WHEN && mToken->type != Token::Type::END_OF_FILE )
			{
				returnCode = Interpreter::Statement();

				if ( ( bool )ErrorHandler::GetLastErrorCode() )
				{
					return Literal( 1ll );
				}
			}

			return returnCode;
		}

		static Literal Statement()
		{
			switch ( mToken->type )
			{
//			case Token::Type::IF:
//				Interpreter::If();
//				break;
//
//			case Token::Type::FOR:
//				Interpreter::For();
//				break;
//
//			case Token::Type::RETURN:
//				return Interpreter::Return();

			case Token::Type::BREAK:
				Interpreter::Break();
				break;

//			case Token::Type::WHILE:
//				Interpreter::While();
//				break;
//
//			case Token::Type::CASE:
//				Interpreter::Case();
//				break;
//
//			case Token::Type::IMPORT:
//				Interpreter::Import();
//				break;
//
//			case Token::Type::FOREACH:
//				Interpreter::ForEach();
//				break;

			case Token::Type::NEXT:
				Interpreter::Next();
				break;

//			default:
//				Interpreter::Expression();
//				break;
			}

			return Literal( 0ll );
		}

		static Literal PrimaryExpression()
		{
			switch ( mToken->type )
			{
			case Token::Type::IDENTIFIER:
				return Literal( Literal::Type::REFERENCE, mToken++->value.identifier );

			case Token::Type::INTEGER_LITERAL:
				return Literal( mToken++->value.integer );

			case Token::Type::STRING_LITERAL:
				return Literal( Literal::Type::STRING, mToken++->value.string );

			case Token::Type::BOOLEAN_LITERAL:
				return Literal( mToken++->value.boolean );

			case Token::Type::NULL_LITERAL:
				return Literal();

			case Token::Type::REAL_LITERAL:
				return Literal( mToken++->value.real );

//			case Token::Type::DEF:
//				return Interpreter::FunctionLiteral( mToken++ );
//
//			case static_cast<Token::Type>( u'(' ):
//				return Interpreter::ParenthesizedExpression();
//
//			case static_cast<Token::Type>( u'[' ):
//				return Interpreter::ArrayLiteral();
//
//			case static_cast<Token::Type>( u'{' ):
//				return Interpreter::HashLiteral();
//
//			case Token::Type::THIS:
//				return new ThisNode( mToken->location );
//
//			default:
//				ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_PRIMARY_EXPRESSION );
//
//				mToken++;
//
//				return nullptr;
			}
		}

//		static Literal ArrayLiteral()
//		{
//			ArrayLiteralNode *node = new ArrayLiteralNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL );
//				delete node;
//
//				return nullptr;
//			}
//
//			if ( mToken->type != static_cast<Token::Type>( u']' ) )
//			{
//				for (;;)
//				{
//					node->AddExpression( Interpreter::Expression() );
//
//					if ( mToken->type == static_cast<Token::Type>( u',' ) )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL );
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
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL );
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
//		static Literal HashLiteral()
//		{
//			HashLiteralNode *node = new HashLiteralNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
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
//					expression = Interpreter::Expression();
//
//					if ( mToken->type != static_cast<Token::Type>( u':' ) )
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_HASH );
//						delete expression;
//						delete node;
//
//						return nullptr;
//					}
//
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
//						delete expression;
//						delete node;
//
//						return nullptr;
//					}
//
//					node->AddHash( new HashNode( tToken->location, expression, Interpreter::Expression() ) );
//
//					if ( mToken->type == static_cast<Token::Type>( u',' ) )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
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
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
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
//		static Literal FunctionLiteral( forward_list<Token>::const_iterator token )
//		{
//			if ( mToken->type == static_cast<Token::Type>( u'(' ) )
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
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
//								ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
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
//								ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//								delete name;
//								delete node;
//
//								return nullptr;
//							}
//						}
//						else
//						{
//							ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_PARAMETER_NAME );
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
//								ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//								delete name;
//								delete node;
//
//								return nullptr;
//							}
//
//							if ( isValueParameter )
//							{
//								parameter = new ValueParameterNode( tToken->location, name, Interpreter::Expression() );
//							}
//							else
//							{
//								ErrorHandler::Error( mToken->location, ErrorCode::OUTPUT_PARAMETER_DEFAULT_ARGUMENT );
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
//								ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
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
//							ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
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
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//					delete node;
//
//					return nullptr;
//				}
//
//				node->block = Interpreter::Block();
//
//				if ( mToken->type == Token::Type::END )
//				{
//					mToken++;
//
//					return node;
//				}
//				else
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_END );
//					delete node;
//
//					return nullptr;
//				}
//			}
//			else
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_PARAMETER );
//
//				return nullptr;
//			}
//		}
//
//		static Literal ParenthesizedExpression()
//		{
//			ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->expression = Interpreter::Expression();
//
//			if ( mToken->type == static_cast<Token::Type>( u')' ) )
//			{
//				mToken++;
//
//				return node;
//			}
//			else
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_RIGHT_PARENTHESIS );
//				delete node;
//
//				return nullptr;
//			}
//		}
//
//		static Literal PostfixExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::PrimaryExpression();
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
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_INDEX );
//						delete expression;
//
//						return nullptr;
//					}
//
//					expression = new IndexReferenceNode( tToken->location, expression, Interpreter::Expression() );
//
//					if ( mToken->type == static_cast<Token::Type>( u']' ) )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_INDEX );
//							delete expression;
//
//							return nullptr;
//						}
//					}
//					else
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_INDEX );
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
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_FUNCTION_CALL );
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
//							node->AddArgument( Interpreter::Expression() );
//
//							if ( mToken->type == static_cast<Token::Type>( u',' ) )
//							{
//								mToken++;
//								if ( mToken->type == Token::Type::END_OF_FILE )
//								{
//									ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_FUNCTION_CALL );
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
//								ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_FUNCTION_CALL );
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
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_MEMBER );
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
//							ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_MEMBER );
//							delete expression;
//
//							return nullptr;
//						}
//					}
//					else
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_MEMBER );
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
//		static Literal UnaryExpression()
//		{
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
//
//				return nullptr;
//			}
//			else if ( mToken->type != static_cast<Token::Type>( u'+' ) && mToken->type != static_cast<Token::Type>( u'-' ) && mToken->type != static_cast<Token::Type>( u'~' ) && mToken->type != static_cast<Token::Type>( u'!' ) )
//			{
//				return Interpreter::PostfixExpression();
//			}
//			else
//			{
//				return new UnaryExpressionNode( mToken->location, mToken++->type, Interpreter::UnaryExpression() );
//			}
//		}
//
//		static Literal MultiplicativeExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::UnaryExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'*' ) || mToken->type == static_cast<Token::Type>( u'/' ) || mToken->type == static_cast<Token::Type>( u'%' ) )
//			{
//				expression = new MultiplicativeExpressionNode( tToken->location, mToken++->type, expression, Interpreter::UnaryExpression() );
//			}
//
//			return expression;
//		}
//
//		static Literal AdditiveExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::MultiplicativeExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'+' ) || mToken->type == static_cast<Token::Type>( u'-' ) )
//			{
//				expression = new AdditiveExpressionNode( tToken->location, mToken++->type, expression, Interpreter::MultiplicativeExpression() );
//			}
//
//			return expression;
//		}
//
//		static Literal ShiftExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::AdditiveExpression();
//
//			while ( mToken->type == Token::Type::SHIFT_LEFT || mToken->type == Token::Type::SHIFT_RIGHT )
//			{
//				expression = new ShiftExpressionNode( tToken->location, mToken++->type, expression, Interpreter::AdditiveExpression() );
//			}
//
//			return expression;
//		}
//
//		static Literal AndExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::ShiftExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'&' ) )
//			{
//				mToken++;
//
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
//					delete expression;
//
//					return nullptr;
//				}
//
//				expression = new AndExpressionNode( tToken->location, expression, Interpreter::ShiftExpression() );
//			}
//
//			return expression;
//		}
//
//		static Literal OrExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::AndExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'|' ) || mToken->type == static_cast<Token::Type>( u'^' ) )
//			{
//				expression = new OrExpressionNode( tToken->location, mToken++->type, expression, Interpreter::AndExpression() );
//			}
//
//			return expression;
//		}
//
//		static Literal RelationalExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::OrExpression();
//
//			while ( mToken->type == static_cast<Token::Type>( u'<' ) || mToken->type == static_cast<Token::Type>( u'>' ) || mToken->type == Token::Type::LESS_THAN_OR_EQUAL || mToken->type == Token::Type::GREATER_THAN_OR_EQUAL )
//			{
//				expression = new RelationalExpressionNode( tToken->location, mToken++->type, expression, Interpreter::OrExpression() );
//			}
//
//			return expression;
//		}
//
//		static Literal EqualityExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::RelationalExpression();
//
//			while ( mToken->type == Token::Type::EQUAL || mToken->type == Token::Type::NOT_EQUAL )
//			{
//				expression = new EqualityExpressionNode( tToken->location, mToken++->type, expression, Interpreter::RelationalExpression() );
//			}
//
//			return expression;
//		}
//
//		static Literal LogicalAndExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::EqualityExpression();
//
//			while ( mToken->type == Token::Type::AND )
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
//					delete expression;
//
//					return nullptr;
//				}
//
//				expression = new LogicalAndExpressionNode( tToken->location, expression, Interpreter::EqualityExpression() );
//			}
//
//			return expression;
//		}
//
//		static Literal LogicalOrExpression()
//		{
//			auto tToken = mToken;
//			ExpressionNode *expression = Interpreter::LogicalAndExpression();
//
//			while ( mToken->type == Token::Type::OR )
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
//					delete expression;
//
//					return nullptr;
//				}
//
//				expression = new LogicalOrExpressionNode( tToken->location, expression, Interpreter::LogicalAndExpression() );
//			}
//
//			return expression;
//		}
//
//		static Literal AssignmentExpression()
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
//								return new AssignmentExpressionNode( tToken->location, new MemberReferenceNode( identifier->location, identifier, member ), Interpreter::FunctionLiteral( tToken ) );
//							}
//							else
//							{
//								ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//								delete identifier;
//								delete member;
//
//								return nullptr;
//							}
//						}
//						else
//						{
//							ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//							delete identifier;
//
//							return nullptr;
//						}
//					}
//					else if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//						delete identifier;
//
//						return nullptr;
//					}
//					else
//					{
//						return new AssignmentExpressionNode( tToken->location, identifier, Interpreter::FunctionLiteral( tToken ) );
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
//								return new AssignmentExpressionNode( tToken->location, new MemberReferenceNode( thisNode->location, thisNode, identifier ), Interpreter::FunctionLiteral( tToken ) );
//							}
//							else
//							{
//								ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//								delete thisNode;
//								delete identifier;
//
//								return nullptr;
//							}
//						}
//						else
//						{
//							ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//							delete thisNode;
//
//							return nullptr;
//						}
//					}
//					else
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//						delete thisNode;
//
//						return nullptr;
//					}
//				}
//				else if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
//
//					return nullptr;
//				}
//				else
//				{
//					mToken = tToken;
//
//					return Interpreter::PrimaryExpression();
//				}
//
//			case Token::Type::CLASS:
//				return Interpreter::Class();
//
//			case Token::Type::PACKAGE:
//				return Interpreter::Package();
//
//			case Token::Type::END_OF_FILE:
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
//
//				return nullptr;
//
//			default:
//				ExpressionNode *expression = Interpreter::LogicalOrExpression();
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
//							ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
//							delete expression;
//
//							return nullptr;
//						}
//
//						return new AssignmentExpressionNode( tToken->location, expression, Interpreter::AssignmentExpression() );
//					}
//					else
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_LHS );
//						delete expression;
//
//						return nullptr;
//					}
//				}
//			}
//		}
//
//		static Literal Class()
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
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
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
//							node->AddArgument( Interpreter::Expression() );
//
//							if ( mToken->type == static_cast<Token::Type>( u',' ) )
//							{
//								mToken++;
//								if ( mToken->type == Token::Type::END_OF_FILE )
//								{
//									ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
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
//								ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
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
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
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
//								ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
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
//									node->baseClassConstructorCall->AddArgument( Interpreter::Expression() );
//
//									if ( mToken->type == static_cast<Token::Type>( u',' ) )
//									{
//										mToken++;
//										if ( mToken->type == Token::Type::END_OF_FILE )
//										{
//											ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
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
//										ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
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
//							ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
//							delete node;
//							delete name;
//
//							return nullptr;
//						}
//					}
//					else
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_BASE_CLASS );
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
//					node->include = Interpreter::Include();
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
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
//						delete node;
//						delete name;
//
//						return nullptr;
//					}
//
//					node->accessSpecifiedBlockList->AddAccessSpecifiedBlock( new AccessSpecifiedBlockNode( tToken->location, accessSpecifier, Interpreter::Block() ) );
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
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_END );
//						delete node;
//						delete name;
//
//						return nullptr;
//					}
//				}
//			}
//			else
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_CLASS_NAME );
//
//				return nullptr;
//			}
//		}
//
//		static Literal Include()
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
//					ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_PACKAGE );
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
//		static Literal Package()
//		{
//			auto tToken = mToken;
//
//			mToken++;
//			if ( mToken->type == Token::Type::IDENTIFIER )
//			{
//				IdentifierNode *name = new IdentifierNode( mToken->location, mToken->value.identifier );
//				PackageNode *node = new PackageNode( tToken->location, Interpreter::Block() );
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
//					ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_END );
//					delete node;
//					delete name;
//
//					return nullptr;
//				}
//			}
//			else
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_PACKAGE_NAME );
//
//				return nullptr;
//			}
//		}
//
//		static Literal Expression()
//		{
//			return Interpreter::AssignmentExpression();
//		}
//
//		static Literal Import()
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
//					ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_IDENTIFIER );
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
//		static Literal If()
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
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT );
//					delete elseIfNode;
//					delete node;
//
//					return nullptr;
//				}
//
//				elseIfNode->condition = Interpreter::Expression();
//
//				if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
//				{
//					mToken++;
//				}
//
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT );
//					delete elseIfNode;
//					delete node;
//
//					return nullptr;
//				}
//
//				elseIfNode->block = Interpreter::Block();
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
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->block = Interpreter::Block();
//
//					if ( mToken->type == Token::Type::END )
//					{
//						mToken++;
//
//						return node;
//					}
//					else
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_END );
//						delete node;
//
//						return nullptr;
//					}
//				}
//				else if ( mToken->type != Token::Type::ELSEIF )
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_END_ELSE_ELSEIF );
//					delete node;
//
//					return nullptr;
//				}
//			}
//		}
//
//		static Literal Case()
//		{
//			CaseNode *node = new CaseNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->value = Interpreter::Expression();
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
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
//						delete whenNode;
//						delete node;
//
//						return nullptr;
//					}
//
//					whenNode->condition = Interpreter::Expression();
//
//					if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
//					{
//						mToken++;
//					}
//
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
//						delete whenNode;
//						delete node;
//
//						return nullptr;
//					}
//
//					whenNode->block = Interpreter::Block();
//					node->AddWhen( whenNode );
//
//					if ( mToken->type == Token::Type::ELSE )
//					{
//						mToken++;
//						if ( mToken->type == Token::Type::END_OF_FILE )
//						{
//							ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
//							delete node;
//
//							return nullptr;
//						}
//
//						node->block = Interpreter::Block();
//
//						if ( mToken->type == Token::Type::END )
//						{
//							mToken++;
//
//							return node;
//						}
//						else
//						{
//							ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_END );
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
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_WHEN_ELSE_ELSEIF );
//						delete node;
//
//						return nullptr;
//					}
//				}
//			}
//			else
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_WHEN );
//				delete node;
//
//				return nullptr;
//			}
//		}
//
//		static Literal While()
//		{
//			WhileNode *node = new WhileNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_WHILE_STATEMENT );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->condition = Interpreter::Expression();
//
//			if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
//			{
//				mToken++;
//			}
//
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_WHILE_STATEMENT );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->block = Interpreter::Block();
//
//			if ( mToken->type == Token::Type::END )
//			{
//				mToken++;
//
//				return node;
//			}
//			else
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_END );
//				delete node;
//
//				return nullptr;
//			}
//		}
//
//		static Literal For()
//		{
//			ForNode *node = new ForNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->initializer = Interpreter::Expression();
//
//			if ( mToken->type == static_cast<Token::Type>( u',' ) )
//			{
//				mToken++;
//				if ( mToken->type == Token::Type::END_OF_FILE )
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
//					delete node;
//
//					return nullptr;
//				}
//
//				node->condition = Interpreter::Expression();
//
//				if ( mToken->type == static_cast<Token::Type>( u',' ) )
//				{
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->iterator = Interpreter::Expression();
//
//					if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
//					{
//						mToken++;
//					}
//
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->block = Interpreter::Block();
//
//					if ( mToken->type == Token::Type::END )
//					{
//						mToken++;
//
//						return node;
//					}
//					else
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_END );
//						delete node;
//
//						return nullptr;
//					}
//				}
//				else
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
//					delete node;
//
//					return nullptr;
//				}
//			}
//			else
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
//				delete node;
//
//				return nullptr;
//			}
//		}
//
//		static Literal ForEach()
//		{
//			ForEachNode *node = new ForEachNode( mToken->location );
//
//			mToken++;
//			if ( mToken->type == Token::Type::END_OF_FILE )
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
//				delete node;
//
//				return nullptr;
//			}
//
//			node->variable = Interpreter::Expression();
//
//			if ( node->variable ->GetType() == Node::Type::IDENTIFIER || node->variable ->GetType() == Node::Type::MEMBER_REFERENCE || node->variable ->GetType() == Node::Type::INDEX_REFERENCE )
//			{
//				if ( mToken->type == Token::Type::IN )
//				{
//					mToken++;
//					if ( mToken->type == Token::Type::END_OF_FILE )
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->collection = Interpreter::Expression();
//
//					if ( node->collection->GetType() != Node::Type::IDENTIFIER && node->collection->GetType() != Node::Type::MEMBER_REFERENCE && node->collection->GetType() != Node::Type::INDEX_REFERENCE )
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_LHS );
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
//						ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
//						delete node;
//
//						return nullptr;
//					}
//
//					node->block = Interpreter::Block();
//
//					if ( mToken->type == Token::Type::END )
//					{
//						mToken++;
//
//						return node;
//					}
//					else
//					{
//						ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_END );
//						delete node;
//
//						return nullptr;
//					}
//				}
//				else
//				{
//					ErrorHandler::Error( mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
//					delete node;
//
//					return nullptr;
//				}
//			}
//			else
//			{
//				ErrorHandler::Error( mToken->location, ErrorCode::EXPECTED_LHS );
//				delete node;
//
//				return nullptr;
//			}
//		}

		static Literal Break()
		{
			return Literal( 0ll );
		}

		static Literal Next()
		{
			return Literal( 0ll );
		}

//		static Literal Return()
//		{
//			auto tToken = mToken;
//
//			mToken++;
//
//			if ( mToken->type == Token::Type::END || mToken->type == Token::Type::ELSE || mToken->type == Token::Type::ELSEIF || mToken->type == Token::Type::PRIVATE || mToken->type == Token::Type::PUBLIC || mToken->type == Token::Type::PROTECTED || mToken->type == Token::Type::WHEN || mToken->type == Token::Type::END_OF_FILE )
//			{
//				return Literal( 0ll );
//			}
//			else
//			{
//				return Interpreter::Expression();
//			}
//		}
	};
}

#endif
