#include <string>
#include <forward_list>

#include "Token.h"
#include "Node.h"
#include "Tokenizer.h"

#include "ErrorCode.h"
#include "ErrorHandler.h"

#ifndef PARSER
#define PARSER

namespace lyrics
{
	using std::string;
	using std::forward_list;

	class Parser
	{
	private:
		Parser() = delete;

	public:
		static bool Parse( const string fileName, BlockNode *&root )
		{
			forward_list<Token> tokenList;

			if ( !Tokenizer::Tokenize( fileName, tokenList ) )
			{
				return false;
			}

			Parser::mToken = tokenList.cbegin();

			root = Parser::Block();

			if ( root )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	
	private:
		static forward_list<Token>::const_iterator mToken;

		static BlockNode *Block()
		{
			BlockNode *node = new BlockNode( Parser::mToken->location );

			while ( Parser::mToken->type != Token::Type::END && Parser::mToken->type != Token::Type::ELSE && Parser::mToken->type != Token::Type::ELSEIF && Parser::mToken->type != Token::Type::PRIVATE && Parser::mToken->type != Token::Type::PUBLIC && Parser::mToken->type != Token::Type::PROTECTED && Parser::mToken->type != Token::Type::WHEN && Parser::mToken->type != Token::Type::END_OF_FILE )
			{
				node->AddStatement( Parser::Statement() );
			}

			return node;
		}

		static StatementNode *Statement()
		{
			switch ( Parser::mToken->type )
			{
			case Token::Type::IF:
				return Parser::If();

			case Token::Type::FOR:
				return Parser::For();

			case Token::Type::RETURN:
				return Parser::Return();

			case Token::Type::BREAK:
				return Parser::Break();

			case Token::Type::WHILE:
				return Parser::While();

			case Token::Type::CASE:
				return Parser::Case();

			case Token::Type::IMPORT:
				return Parser::Import();

			case Token::Type::FOREACH:
				return Parser::ForEach();

			case Token::Type::NEXT:
				return Parser::Next();
			
			default:
				return Parser::Expression();
			}
		}

		static ExpressionNode *PrimaryExpression()
		{
			switch ( Parser::mToken->type )
			{
			case Token::Type::IDENTIFIER:
				return new IdentifierNode( Parser::mToken->location, Parser::mToken++->value.identifier );

			case Token::Type::INTEGER_LITERAL:
				return new IntegerLiteralNode( Parser::mToken->location, Parser::mToken++->value.integer );

			case Token::Type::STRING_LITERAL:
				return new StringLiteralNode( Parser::mToken->location, Parser::mToken++->value.string );

			case Token::Type::BOOLEAN_LITERAL:
				return new BooleanLiteralNode( Parser::mToken->location, Parser::mToken++->value.boolean );

			case Token::Type::NULL_LITERAL:
				return new NullLiteralNode( Parser::mToken++->location );

			case Token::Type::REAL_LITERAL:
				return new RealLiteralNode( Parser::mToken->location, Parser::mToken++->value.real );

			case Token::Type::DEF:
				return Parser::FunctionLiteral( Parser::mToken++ );

			case static_cast<Token::Type>( u'(' ):
				return Parser::ParenthesizedExpression();

			case static_cast<Token::Type>( u'[' ):
				return Parser::ArrayLiteral();

			case static_cast<Token::Type>( u'{' ):
				return Parser::HashLiteral();
			
			case Token::Type::THIS:
				return new ThisNode( Parser::mToken->location );

			default:
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_PRIMARY_EXPRESSION );

				Parser::mToken++;

				return nullptr;
			}
		}

		static ArrayLiteralNode *ArrayLiteral()
		{
			ArrayLiteralNode *node = new ArrayLiteralNode( Parser::mToken->location );

			Parser::mToken++;
			if ( Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL );
				delete node;

				return nullptr;
			}

			if ( Parser::mToken->type != static_cast<Token::Type>( u']' ) )
			{
				for (;;)
				{
					node->AddExpression( Parser::Expression() );

					if ( Parser::mToken->type == static_cast<Token::Type>( u',' ) )
					{
						Parser::mToken++;
						if ( Parser::mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL );
							delete node;

							return nullptr;
						}
						else if ( Parser::mToken->type == static_cast<Token::Type>( u']' ) )
						{
							break;
						}
					}
					else if ( Parser::mToken->type == static_cast<Token::Type>( u']' ) )
					{
						break;
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL );
						delete node;

						return nullptr;
					}
				}
			}

			Parser::mToken++;

			return node;
		}

		static HashLiteralNode *HashLiteral()
		{
			HashLiteralNode *node = new HashLiteralNode( Parser::mToken->location );

			Parser::mToken++;
			if ( Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
				delete node;

				return nullptr;
			}

			if ( Parser::mToken->type != static_cast<Token::Type>( u'}' ) )
			{
				ExpressionNode *expression;

				for (;;)
				{
					auto tToken = Parser::mToken;

					expression = Parser::Expression();

					if ( Parser::mToken->type != static_cast<Token::Type>( u':' ) )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_HASH );
						delete expression;
						delete node;

						return nullptr;
					}

					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
						delete expression;
						delete node;

						return nullptr;
					}

					node->AddHash( new HashNode( tToken->location, expression, Parser::Expression() ) );

					if ( Parser::mToken->type == static_cast<Token::Type>( u',' ) )
					{
						Parser::mToken++;
						if ( Parser::mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
							delete node;

							return nullptr;
						}
						else if ( Parser::mToken->type == static_cast<Token::Type>( u'}' ) )
						{
							break;
						}
					}
					else if ( Parser::mToken->type == static_cast<Token::Type>( u'}' ) )
					{
						break;
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
						delete node;

						return nullptr;
					}
				}
			}

			Parser::mToken++;

			return node;
		}

		static FunctionLiteralNode *FunctionLiteral( forward_list<Token>::const_iterator token )
		{
			if ( Parser::mToken->type == static_cast<Token::Type>( u'(' ) )
			{
				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );

					return nullptr;
				}

				FunctionLiteralNode *node = new FunctionLiteralNode( token->location );

				if ( Parser::mToken->type != static_cast<Token::Type>( u')' ) )
				{
					ParameterNode *parameter;
					bool isValueParameter;
					IdentifierNode *name;

					for (;;)
					{
						auto tToken = Parser::mToken;

						if ( Parser::mToken->type != Token::Type::OUT )
						{
							isValueParameter = true;
						}
						else
						{
							Parser::mToken++;
							if ( Parser::mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
								delete node;

								return nullptr;
							}

							isValueParameter = false;
						}

						if ( Parser::mToken->type == Token::Type::IDENTIFIER )
						{
							name = new IdentifierNode( Parser::mToken->location, Parser::mToken->value.identifier );

							Parser::mToken++;
							if ( Parser::mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
								delete name;
								delete node;

								return nullptr;
							}
						}
						else
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_PARAMETER_NAME );
							delete node;

							return nullptr;
						}

						if ( Parser::mToken->type != static_cast<Token::Type>( u'=' ) )
						{
							if ( isValueParameter )
							{
								parameter = new ValueParameterNode( tToken->location, name );
							}
							else
							{
								parameter = new OutputParameterNode( tToken->location, name );
							}
						}
						else
						{
							Parser::mToken++;
							if ( Parser::mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
								delete name;
								delete node;

								return nullptr;
							}

							if ( isValueParameter )
							{
								parameter = new ValueParameterNode( tToken->location, name, Parser::Expression() );
							}
							else
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::OUTPUT_PARAMETER_DEFAULT_ARGUMENT );
								delete name;
								delete node;

								return nullptr;
							}
						}

						node->AddParameter( parameter );

						if ( Parser::mToken->type == static_cast<Token::Type>( u',' ) )
						{
							Parser::mToken++;
							if ( Parser::mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
								delete node;

								return nullptr;
							}
						}
						else if ( Parser::mToken->type == static_cast<Token::Type>( u')' ) )
						{
							break;
						}
						else
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
							delete node;

							return nullptr;
						}
					}
				}

				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
					delete node;

					return nullptr;
				}

				node->block = Parser::Block();

				if ( Parser::mToken->type == Token::Type::END )
				{
					Parser::mToken++;

					return node;
				}
				else
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_END );
					delete node;

					return nullptr;
				}
			}
			else
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_PARAMETER );

				return nullptr;
			}
		}

		static ParenthesizedExpressionNode *ParenthesizedExpression()
		{
			ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode( Parser::mToken->location );

			Parser::mToken++;
			if ( Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
				delete node;

				return nullptr;
			}

			node->expression = Parser::Expression();

			if ( Parser::mToken->type == static_cast<Token::Type>( u')' ) )
			{
				Parser::mToken++;

				return node;
			}
			else
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_RIGHT_PARENTHESIS );
				delete node;

				return nullptr;
			}
		}

		static ExpressionNode *PostfixExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::PrimaryExpression();

			if ( expression == nullptr || ( expression->GetType() != Node::Type::IDENTIFIER && expression->GetType() != Node::Type::THIS ) )
			{
				return expression;
			}

			for (;;)
			{
				if ( Parser::mToken->type == static_cast<Token::Type>( u'[' ) )
				{
					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_INDEX );
						delete expression;

						return nullptr;
					}

					expression = new IndexReferenceNode( tToken->location, expression, Parser::Expression() );

					if ( Parser::mToken->type == static_cast<Token::Type>( u']' ) )
					{
						Parser::mToken++;
						if ( Parser::mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_INDEX );
							delete expression;

							return nullptr;
						}
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_INDEX );
						delete expression;

						return nullptr;
					}
				}
				else if ( Parser::mToken->type == static_cast<Token::Type>( u'(' ) )
				{
					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_FUNCTION_CALL );
						delete expression;

						return nullptr;
					}

					FunctionCallNode *node = new FunctionCallNode( tToken->location, expression );

					if ( Parser::mToken->type != static_cast<Token::Type>( u')' ) )
					{
						for (;;)
						{
							node->AddArgument( Parser::Expression() );

							if ( Parser::mToken->type == static_cast<Token::Type>( u',' ) )
							{
								Parser::mToken++;
								if ( Parser::mToken->type == Token::Type::END_OF_FILE )
								{
									ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_FUNCTION_CALL );
									delete node;

									return nullptr;
								}
							}
							else if ( Parser::mToken->type == static_cast<Token::Type>( u')' ) )
							{
								break;
							}
							else
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_FUNCTION_CALL );
								delete node;

								return nullptr;
							}
						}
					}

					Parser::mToken++;

					return node;
				}
				else if ( Parser::mToken->type == static_cast<Token::Type>( u'.' ) )
				{
					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_MEMBER );
						delete expression;

						return nullptr;
					}

					if ( Parser::mToken->type == Token::Type::IDENTIFIER )
					{
						expression = new MemberReferenceNode( tToken->location, expression, new IdentifierNode( Parser::mToken->location, Parser::mToken->value.identifier ) );

						Parser::mToken++;
						if ( Parser::mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_MEMBER );
							delete expression;

							return nullptr;
						}
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_MEMBER );
						delete expression;

						return nullptr;
					}
				}
				else
				{
					return expression;
				}
			}
		}

		static ExpressionNode *UnaryExpression()
		{
			if ( Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );

				return nullptr;
			}
			else if ( Parser::mToken->type != static_cast<Token::Type>( u'+' ) && Parser::mToken->type != static_cast<Token::Type>( u'-' ) && Parser::mToken->type != static_cast<Token::Type>( u'~' ) && Parser::mToken->type != static_cast<Token::Type>( u'!' ) )
			{
				return Parser::PostfixExpression();
			}
			else
			{
				return new UnaryExpressionNode( Parser::mToken->location, Parser::mToken++->type, Parser::UnaryExpression() );
			}
		}

		static ExpressionNode *MultiplicativeExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::UnaryExpression();

			while ( Parser::mToken->type == static_cast<Token::Type>( u'*' ) || Parser::mToken->type == static_cast<Token::Type>( u'/' ) || Parser::mToken->type == static_cast<Token::Type>( u'%' ) )
			{
				expression = new MultiplicativeExpressionNode( tToken->location, Parser::mToken++->type, expression, Parser::UnaryExpression() );
			}

			return expression;
		}

		static ExpressionNode *AdditiveExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::MultiplicativeExpression();

			while ( Parser::mToken->type == static_cast<Token::Type>( u'+' ) || Parser::mToken->type == static_cast<Token::Type>( u'-' ) )
			{
				expression = new AdditiveExpressionNode( tToken->location, Parser::mToken++->type, expression, Parser::MultiplicativeExpression() );
			}

			return expression;
		}

		static ExpressionNode *ShiftExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::AdditiveExpression();

			while ( Parser::mToken->type == Token::Type::SHIFT_LEFT || Parser::mToken->type == Token::Type::SHIFT_RIGHT )
			{
				expression = new ShiftExpressionNode( tToken->location, Parser::mToken++->type, expression, Parser::AdditiveExpression() );
			}

			return expression;
		}

		static ExpressionNode *AndExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::ShiftExpression();

			while ( Parser::mToken->type == static_cast<Token::Type>( u'&' ) )
			{
				Parser::mToken++;

				if ( Parser::mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
					delete expression;

					return nullptr;
				}

				expression = new AndExpressionNode( tToken->location, expression, Parser::ShiftExpression() );
			}

			return expression;
		}

		static ExpressionNode *OrExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::AndExpression();

			while ( Parser::mToken->type == static_cast<Token::Type>( u'|' ) || Parser::mToken->type == static_cast<Token::Type>( u'^' ) )
			{
				expression = new OrExpressionNode( tToken->location, Parser::mToken++->type, expression, Parser::AndExpression() );
			}

			return expression;
		}

		static ExpressionNode *RelationalExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::OrExpression();

			while ( Parser::mToken->type == static_cast<Token::Type>( u'<' ) || Parser::mToken->type == static_cast<Token::Type>( u'>' ) || Parser::mToken->type == Token::Type::LESS_THAN_OR_EQUAL || Parser::mToken->type == Token::Type::GREATER_THAN_OR_EQUAL )
			{
				expression = new RelationalExpressionNode( tToken->location, Parser::mToken++->type, expression, Parser::OrExpression() );
			}

			return expression;
		}

		static ExpressionNode *EqualityExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::RelationalExpression();

			while ( Parser::mToken->type == Token::Type::EQUAL || Parser::mToken->type == Token::Type::NOT_EQUAL )
			{
				expression = new EqualityExpressionNode( tToken->location, Parser::mToken++->type, expression, Parser::RelationalExpression() );
			}

			return expression;
		}

		static ExpressionNode *LogicalAndExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::EqualityExpression();

			while ( Parser::mToken->type == Token::Type::AND )
			{
				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
					delete expression;

					return nullptr;
				}

				expression = new LogicalAndExpressionNode( tToken->location, expression, Parser::EqualityExpression() );
			}

			return expression;
		}

		static ExpressionNode *LogicalOrExpression()
		{
			auto tToken = Parser::mToken;
			ExpressionNode *expression = Parser::LogicalAndExpression();

			while ( Parser::mToken->type == Token::Type::OR )
			{
				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
					delete expression;

					return nullptr;
				}

				expression = new LogicalOrExpressionNode( tToken->location, expression, Parser::LogicalAndExpression() );
			}

			return expression;
		}

		static ExpressionNode *AssignmentExpression()
		{
			auto tToken = Parser::mToken;

			switch ( Parser::mToken->type )
			{
			case Token::Type::DEF:
				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::IDENTIFIER )
				{
					IdentifierNode *identifier = new IdentifierNode( Parser::mToken->location, Parser::mToken->value.identifier );

					Parser::mToken++;
					if ( Parser::mToken->type == static_cast<Token::Type>( u'.' ) )
					{
						Parser::mToken++;
						if ( Parser::mToken->type == Token::Type::IDENTIFIER )
						{
							IdentifierNode *member = new IdentifierNode( Parser::mToken->location, Parser::mToken->value.identifier );

							Parser::mToken++;
							if ( Parser::mToken->type == static_cast<Token::Type>( u'(' ) )
							{
								return new AssignmentExpressionNode( tToken->location, new MemberReferenceNode( identifier->location, identifier, member ), Parser::FunctionLiteral( tToken ) );
							}
							else
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
								delete identifier;
								delete member;

								return nullptr;
							}
						}
						else
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
							delete identifier;

							return nullptr;
						}
					}
					else if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
						delete identifier;

						return nullptr;
					}
					else
					{
						return new AssignmentExpressionNode( tToken->location, identifier, Parser::FunctionLiteral( tToken ) );
					}
				}
				else if ( Parser::mToken->type == Token::Type::THIS )
				{
					ThisNode *thisNode = new ThisNode( Parser::mToken->location );

					Parser::mToken++;
					if ( Parser::mToken->type == static_cast<Token::Type>( u'.' ) )
					{
						Parser::mToken++;
						if ( Parser::mToken->type == Token::Type::IDENTIFIER )
						{
							IdentifierNode *identifier = new IdentifierNode( Parser::mToken->location, Parser::mToken->value.identifier );

							Parser::mToken++;
							if ( Parser::mToken->type == static_cast<Token::Type>( u'(' ) )
							{
								return new AssignmentExpressionNode( tToken->location, new MemberReferenceNode( thisNode->location, thisNode, identifier ), Parser::FunctionLiteral( tToken ) );
							}
							else
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
								delete thisNode;
								delete identifier;

								return nullptr;
							}
						}
						else
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
							delete thisNode;

							return nullptr;
						}
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );
						delete thisNode;

						return nullptr;
					}
				}
				else if ( Parser::mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FUNCTION );

					return nullptr;
				}
				else
				{
					Parser::mToken = tToken;

					return Parser::PrimaryExpression();
				}

			case Token::Type::CLASS:
				return Parser::Class();

			case Token::Type::PACKAGE:
				return Parser::Package();

			case Token::Type::END_OF_FILE:
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );

				return nullptr;

			default:
				ExpressionNode *expression = Parser::LogicalOrExpression();

				if ( expression == nullptr || Parser::mToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return expression;
				}
				else
				{
					if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::MEMBER_REFERENCE || expression->GetType() == Node::Type::INDEX_REFERENCE )
					{
						Parser::mToken++;

						if ( Parser::mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
							delete expression;

							return nullptr;
						}

						return new AssignmentExpressionNode( tToken->location, expression, Parser::AssignmentExpression() );
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_LHS );
						delete expression;

						return nullptr;
					}
				}
			}
		}

		static AssignmentExpressionNode *Class()
		{
			auto tToken = Parser::mToken;

			Parser::mToken++;
			if ( Parser::mToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( Parser::mToken->location, Parser::mToken->value.identifier );
				ClassNode *node = new ClassNode( tToken->location );

				Parser::mToken++;
				if ( Parser::mToken->type == static_cast<Token::Type>( u'(' ) )
				{
					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
						delete node;
						delete name;

						return nullptr;
					}

					if ( Parser::mToken->type != static_cast<Token::Type>( u')' ) )
					{
						for (;;)
						{
							node->AddArgument( Parser::Expression() );

							if ( Parser::mToken->type == static_cast<Token::Type>( u',' ) )
							{
								Parser::mToken++;
								if ( Parser::mToken->type == Token::Type::END_OF_FILE )
								{
									ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
									delete node;
									delete name;

									return nullptr;
								}
							}
							else if ( Parser::mToken->type == static_cast<Token::Type>( u')' ) )
							{
								break;
							}
							else
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
								delete node;
								delete name;

								return nullptr;
							}
						}
					}
				}
				else
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
					delete node;
					delete name;

					return nullptr;
				}

				Parser::mToken++;
				if ( Parser::mToken->type == static_cast<Token::Type>( u':' ) )
				{
					node->baseClassConstructorCall = new BaseClassConstructorCallNode( Parser::mToken->location );

					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::IDENTIFIER )
					{
						node->baseClassConstructorCall->baseClass = new IdentifierNode( Parser::mToken->location, Parser::mToken->value.identifier );

						Parser::mToken++;
						if ( Parser::mToken->type == static_cast<Token::Type>( u'(' ) )
						{
							Parser::mToken++;
							if ( Parser::mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
								delete node;
								delete name;

								return nullptr;
							}

							if ( Parser::mToken->type != static_cast<Token::Type>( u')' ) )
							{
								for (;;)
								{
									node->baseClassConstructorCall->AddArgument( Parser::Expression() );

									if ( Parser::mToken->type == static_cast<Token::Type>( u',' ) )
									{
										Parser::mToken++;
										if ( Parser::mToken->type == Token::Type::END_OF_FILE )
										{
											ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
											delete node;
											delete name;

											return nullptr;
										}
									}
									else if ( Parser::mToken->type == static_cast<Token::Type>( u')' ) )
									{
										break;
									}
									else
									{
										ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
										delete node;
										delete name;

										return nullptr;
									}
								}
							}
						}
						else
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
							delete node;
							delete name;

							return nullptr;
						}
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_BASE_CLASS );
						delete node;
						delete name;

						return nullptr;
					}
				}

				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::INCLUDE )
				{
					node->include = Parser::Include();
				}

				Token::Type accessSpecifier;
				auto tToken = Parser::mToken;

				node->accessSpecifiedBlockList = new AccessSpecifiedBlockListNode( Parser::mToken->location );

				switch ( Parser::mToken->type )
				{
				case Token::Type::PRIVATE:
				case Token::Type::PUBLIC:
				case Token::Type::PROTECTED:
					accessSpecifier = Parser::mToken->type;
					Parser::mToken++;
					break;

				default:
					accessSpecifier = Token::Type::PUBLIC;
					break;
				}

				for (;;)
				{
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
						delete node;
						delete name;

						return nullptr;
					}

					node->accessSpecifiedBlockList->AddAccessSpecifiedBlock( new AccessSpecifiedBlockNode( tToken->location, accessSpecifier, Parser::Block() ) );

					switch ( Parser::mToken->type )
					{
					case Token::Type::PRIVATE:
					case Token::Type::PUBLIC:
					case Token::Type::PROTECTED:
						accessSpecifier = Parser::mToken->type;
						tToken = Parser::mToken++;
						break;

					case Token::Type::END:
						Parser::mToken++;

						return new AssignmentExpressionNode( tToken->location, name, node );

					default:
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_END );
						delete node;
						delete name;

						return nullptr;
					}
				}
			}
			else
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_CLASS_NAME );

				return nullptr;
			}
		}

		static IncludeNode *Include()
		{
			IncludeNode *node = new IncludeNode( Parser::mToken->location );

			do
			{
				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::IDENTIFIER )
				{
					node->AddPackage( new IdentifierNode( Parser::mToken->location, Parser::mToken++->value.identifier ) );
				}
				else
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_PACKAGE );
					delete node;

					return nullptr;
				}
			}
			while ( Parser::mToken->type == static_cast<Token::Type>( u',' ) );

			return node;
		}

		static AssignmentExpressionNode *Package()
		{
			auto tToken = Parser::mToken;

			Parser::mToken++;
			if ( Parser::mToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( Parser::mToken->location, Parser::mToken->value.identifier );
				PackageNode *node = new PackageNode( tToken->location, Parser::Block() );

				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::END )
				{
					Parser::mToken++;

					return new AssignmentExpressionNode( tToken->location, name, node );
				}
				else
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_END );
					delete node;
					delete name;

					return nullptr;
				}
			}
			else
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_PACKAGE_NAME );

				return nullptr;
			}
		}

		static ExpressionNode *Expression()
		{
			return Parser::AssignmentExpression();
		}

		static ImportNode *Import()
		{
			ImportNode *node = new ImportNode( Parser::mToken->location );

			do
			{
				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::IDENTIFIER )
				{
					node->AddIdentifier( new IdentifierNode( Parser::mToken->location, Parser::mToken++->value.identifier ) );
				}
				else
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_IDENTIFIER );
					delete node;

					return nullptr;
				}
			}
			while ( Parser::mToken->type == static_cast<Token::Type>( u'.' ) );

			return node;
		}

		static IfNode *If()
		{
			IfNode *node = new IfNode( Parser::mToken->location );
			ElseIfNode *elseIfNode;

			for (;;)
			{
				elseIfNode = new ElseIfNode( Parser::mToken->location );

				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT );
					delete elseIfNode;
					delete node;

					return nullptr;
				}

				elseIfNode->condition = Parser::Expression();

				if ( Parser::mToken->type == Token::Type::THEN || Parser::mToken->type == static_cast<Token::Type>( u':' ) )
				{
					Parser::mToken++;
				}

				if ( Parser::mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT );
					delete elseIfNode;
					delete node;

					return nullptr;
				}

				elseIfNode->block = Parser::Block();
				node->AddElseIf( elseIfNode );

				if ( Parser::mToken->type == Token::Type::END )
				{
					Parser::mToken++;

					return node;
				}
				else if ( Parser::mToken->type == Token::Type::ELSE )
				{
					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT );
						delete node;

						return nullptr;
					}

					node->block = Parser::Block();

					if ( Parser::mToken->type == Token::Type::END )
					{
						Parser::mToken++;

						return node;
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_END );
						delete node;

						return nullptr;
					}
				}
				else if ( Parser::mToken->type != Token::Type::ELSEIF )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_END_ELSE_ELSEIF );
					delete node;

					return nullptr;
				}
			}
		}

		static CaseNode *Case()
		{
			CaseNode *node = new CaseNode( Parser::mToken->location );

			Parser::mToken++;
			if ( Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
				delete node;

				return nullptr;
			}

			node->value = Parser::Expression();

			if ( Parser::mToken->type == Token::Type::WHEN )
			{
				WhenNode *whenNode;

				for (;;)
				{
					whenNode = new WhenNode( Parser::mToken->location );

					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
						delete whenNode;
						delete node;

						return nullptr;
					}

					whenNode->condition = Parser::Expression();

					if ( Parser::mToken->type == Token::Type::THEN || Parser::mToken->type == static_cast<Token::Type>( u':' ) )
					{
						Parser::mToken++;
					}

					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
						delete whenNode;
						delete node;

						return nullptr;
					}

					whenNode->block = Parser::Block();
					node->AddWhen( whenNode );

					if ( Parser::mToken->type == Token::Type::ELSE )
					{
						Parser::mToken++;
						if ( Parser::mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
							delete node;

							return nullptr;
						}

						node->block = Parser::Block();

						if ( Parser::mToken->type == Token::Type::END )
						{
							Parser::mToken++;

							return node;
						}
						else
						{
							ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_END );
							delete node;

							return nullptr;
						}
					}
					else if ( Parser::mToken->type == Token::Type::END )
					{
						Parser::mToken++;

						return node;
					}
					else if ( Parser::mToken->type != Token::Type::WHEN )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_WHEN_ELSE_ELSEIF );
						delete node;

						return nullptr;
					}
				}
			}
			else
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_WHEN );
				delete node;

				return nullptr;
			}
		}

		static WhileNode *While()
		{
			WhileNode *node = new WhileNode( Parser::mToken->location );

			Parser::mToken++;
			if ( Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_WHILE_STATEMENT );
				delete node;

				return nullptr;
			}

			node->condition = Parser::Expression();

			if ( Parser::mToken->type == Token::Type::DO || Parser::mToken->type == static_cast<Token::Type>( u':' ) )
			{
				Parser::mToken++;
			}

			if ( Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_WHILE_STATEMENT );
				delete node;

				return nullptr;
			}

			node->block = Parser::Block();

			if ( Parser::mToken->type == Token::Type::END )
			{
				Parser::mToken++;

				return node;
			}
			else
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_END );
				delete node;

				return nullptr;
			}
		}

		static ForNode *For()
		{
			ForNode *node = new ForNode( Parser::mToken->location );

			Parser::mToken++;
			if ( Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
				delete node;

				return nullptr;
			}

			node->initializer = Parser::Expression();

			if ( Parser::mToken->type == static_cast<Token::Type>( u',' ) )
			{
				Parser::mToken++;
				if ( Parser::mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
					delete node;

					return nullptr;
				}

				node->condition = Parser::Expression();

				if ( Parser::mToken->type == static_cast<Token::Type>( u',' ) )
				{
					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
						delete node;

						return nullptr;
					}

					node->iterator = Parser::Expression();

					if ( Parser::mToken->type == Token::Type::DO || Parser::mToken->type == static_cast<Token::Type>( u':' ) )
					{
						Parser::mToken++;
					}

					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
						delete node;

						return nullptr;
					}

					node->block = Parser::Block();

					if ( Parser::mToken->type == Token::Type::END )
					{
						Parser::mToken++;

						return node;
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_END );
						delete node;

						return nullptr;
					}
				}
				else
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
					delete node;

					return nullptr;
				}
			}
			else
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
				delete node;

				return nullptr;
			}
		}

		static ForEachNode *ForEach()
		{
			ForEachNode *node = new ForEachNode( Parser::mToken->location );

			Parser::mToken++;
			if ( Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
				delete node;

				return nullptr;
			}

			node->variable = Parser::Expression();

			if ( node->variable ->GetType() == Node::Type::IDENTIFIER || node->variable ->GetType() == Node::Type::MEMBER_REFERENCE || node->variable ->GetType() == Node::Type::INDEX_REFERENCE )
			{
				if ( Parser::mToken->type == Token::Type::IN )
				{
					Parser::mToken++;
					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
						delete node;

						return nullptr;
					}

					node->collection = Parser::Expression();

					if ( node->collection->GetType() != Node::Type::IDENTIFIER && node->collection->GetType() != Node::Type::MEMBER_REFERENCE && node->collection->GetType() != Node::Type::INDEX_REFERENCE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_LHS );
						delete node;

						return nullptr;
					}

					if ( Parser::mToken->type == Token::Type::DO || Parser::mToken->type == static_cast<Token::Type>( u':' ) )
					{
						Parser::mToken++;
					}

					if ( Parser::mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
						delete node;

						return nullptr;
					}

					node->block = Parser::Block();

					if ( Parser::mToken->type == Token::Type::END )
					{
						Parser::mToken++;

						return node;
					}
					else
					{
						ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_END );
						delete node;

						return nullptr;
					}
				}
				else
				{
					ErrorHandler::Error( Parser::mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
					delete node;

					return nullptr;
				}
			}
			else
			{
				ErrorHandler::Error( Parser::mToken->location, ErrorCode::EXPECTED_LHS );
				delete node;

				return nullptr;
			}
		}

		static BreakNode *Break()
		{
			return new BreakNode( Parser::mToken++->location );
		}

		static NextNode *Next()
		{
			return new NextNode( Parser::mToken++->location );
		}

		static ReturnNode *Return()
		{
			auto tToken = Parser::mToken;

			Parser::mToken++;

			if ( Parser::mToken->type == Token::Type::END || Parser::mToken->type == Token::Type::ELSE || Parser::mToken->type == Token::Type::ELSEIF || Parser::mToken->type == Token::Type::PRIVATE || Parser::mToken->type == Token::Type::PUBLIC || Parser::mToken->type == Token::Type::PROTECTED || Parser::mToken->type == Token::Type::WHEN || Parser::mToken->type == Token::Type::END_OF_FILE )
			{
				return new ReturnNode( tToken->location );
			}
			else
			{
				return new ReturnNode( tToken->location, Parser::Expression() );
			}
		}
	};
}

#endif
