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
	public:
		bool Parse( const string &fileName, BlockNode *&root )
		{
			forward_list<Token> tokenList;

			if ( !Tokenizer().Tokenize( fileName, tokenList ) )
			{
				return false;
			}

			mToken = tokenList.cbegin();

			root = Block();

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
		forward_list<Token>::const_iterator mToken;

		BlockNode *Block()
		{
			BlockNode *node = new BlockNode( mToken->location );

			while ( mToken->type != Token::Type::END && mToken->type != Token::Type::ELSE && mToken->type != Token::Type::ELSEIF && mToken->type != Token::Type::PRIVATE && mToken->type != Token::Type::PUBLIC && mToken->type != Token::Type::WHEN && mToken->type != Token::Type::END_OF_FILE )
			{
				node->AddStatement( Statement() );
			}

			return node;
		}

		StatementNode *Statement()
		{
			switch ( mToken->type )
			{
			case Token::Type::IF:
				return If();

			case Token::Type::FOR:
				return For();

			case Token::Type::RETURN:
				return Return();

			case Token::Type::BREAK:
				return Break();

			case Token::Type::WHILE:
				return While();

			case Token::Type::CASE:
				return Case();

			case Token::Type::IMPORT:
				return Import();

			case Token::Type::FOREACH:
				return ForEach();

			case Token::Type::NEXT:
				return Next();
			
			default:
				return Expression();
			}
		}

		ExpressionNode *PrimaryExpression()
		{
			switch ( mToken->type )
			{
			case Token::Type::IDENTIFIER:
				return new IdentifierNode( mToken->location, mToken++->value.identifier );

			case Token::Type::INTEGER_LITERAL:
				return new IntegerLiteralNode( mToken->location, mToken++->value.integer );

			case Token::Type::STRING_LITERAL:
				return new StringLiteralNode( mToken->location, mToken++->value.string );

			case Token::Type::BOOLEAN_LITERAL:
				return new BooleanLiteralNode( mToken->location, mToken++->value.boolean );

			case Token::Type::NULL_LITERAL:
				return new NullLiteralNode( mToken++->location );

			case Token::Type::REAL_LITERAL:
				return new RealLiteralNode( mToken->location, mToken++->value.real );

			case Token::Type::ROUTINE:
				return RoutineLiteral( mToken++ );

			case static_cast<Token::Type>( u'(' ):
				return ParenthesizedExpression();

			case static_cast<Token::Type>( u'[' ):
				return ArrayLiteral();

			case static_cast<Token::Type>( u'{' ):
				return HashLiteral();
			
			case Token::Type::THIS:
				return new ThisNode( mToken->location );

			default:
				ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_PRIMARY_EXPRESSION );

				mToken++;

				return nullptr;
			}
		}

		ArrayLiteralNode *ArrayLiteral()
		{
			ArrayLiteralNode *node = new ArrayLiteralNode( mToken->location );

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL );
				delete node;

				return nullptr;
			}

			if ( mToken->type != static_cast<Token::Type>( u']' ) )
			{
				for (;;)
				{
					node->AddExpression( Expression() );

					if ( mToken->type == static_cast<Token::Type>( u',' ) )
					{
						mToken++;
						if ( mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL );
							delete node;

							return nullptr;
						}
						else if ( mToken->type == static_cast<Token::Type>( u']' ) )
						{
							break;
						}
					}
					else if ( mToken->type == static_cast<Token::Type>( u']' ) )
					{
						break;
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL );
						delete node;

						return nullptr;
					}
				}
			}

			mToken++;

			return node;
		}

		HashLiteralNode *HashLiteral()
		{
			HashLiteralNode *node = new HashLiteralNode( mToken->location );

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
				delete node;

				return nullptr;
			}

			if ( mToken->type != static_cast<Token::Type>( u'}' ) )
			{
				ExpressionNode *expression;

				for (;;)
				{
					auto tToken = mToken;

					expression = Expression();

					if ( mToken->type != static_cast<Token::Type>( u':' ) )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_HASH );
						delete expression;
						delete node;

						return nullptr;
					}

					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
						delete expression;
						delete node;

						return nullptr;
					}

					node->AddHash( new HashNode( tToken->location, expression, Expression() ) );

					if ( mToken->type == static_cast<Token::Type>( u',' ) )
					{
						mToken++;
						if ( mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
							delete node;

							return nullptr;
						}
						else if ( mToken->type == static_cast<Token::Type>( u'}' ) )
						{
							break;
						}
					}
					else if ( mToken->type == static_cast<Token::Type>( u'}' ) )
					{
						break;
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL );
						delete node;

						return nullptr;
					}
				}
			}

			mToken++;

			return node;
		}

		RoutineLiteralNode *RoutineLiteral( forward_list<Token>::const_iterator token )
		{
			if ( mToken->type == static_cast<Token::Type>( u'(' ) )
			{
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );

					return nullptr;
				}

				RoutineLiteralNode *node = new RoutineLiteralNode( token->location );

				if ( mToken->type != static_cast<Token::Type>( u')' ) )
				{
					ParameterNode *parameter;
					bool isValueParameter;
					IdentifierNode *name;

					for (;;)
					{
						auto tToken = mToken;

						if ( mToken->type != Token::Type::OUT )
						{
							isValueParameter = true;
						}
						else
						{
							mToken++;
							if ( mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
								delete node;

								return nullptr;
							}

							isValueParameter = false;
						}

						if ( mToken->type == Token::Type::IDENTIFIER )
						{
							name = new IdentifierNode( mToken->location, mToken->value.identifier );

							mToken++;
							if ( mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
								delete name;
								delete node;

								return nullptr;
							}
						}
						else
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_PARAMETER_NAME );
							delete node;

							return nullptr;
						}

						if ( mToken->type != static_cast<Token::Type>( u'=' ) )
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
							mToken++;
							if ( mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
								delete name;
								delete node;

								return nullptr;
							}

							if ( isValueParameter )
							{
								parameter = new ValueParameterNode( tToken->location, name, Expression() );
							}
							else
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::OUTPUT_PARAMETER_DEFAULT_ARGUMENT );
								delete name;
								delete node;

								return nullptr;
							}
						}

						node->AddParameter( parameter );

						if ( mToken->type == static_cast<Token::Type>( u',' ) )
						{
							mToken++;
							if ( mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
								delete node;

								return nullptr;
							}
						}
						else if ( mToken->type == static_cast<Token::Type>( u')' ) )
						{
							break;
						}
						else
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
							delete node;

							return nullptr;
						}
					}
				}

				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
					delete node;

					return nullptr;
				}

				node->block = Block();

				if ( mToken->type == Token::Type::END )
				{
					mToken++;

					return node;
				}
				else
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_END );
					delete node;

					return nullptr;
				}
			}
			else
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_PARAMETER );

				return nullptr;
			}
		}

		ParenthesizedExpressionNode *ParenthesizedExpression()
		{
			ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode( mToken->location );

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
				delete node;

				return nullptr;
			}

			node->expression = Expression();

			if ( mToken->type == static_cast<Token::Type>( u')' ) )
			{
				mToken++;

				return node;
			}
			else
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_RIGHT_PARENTHESIS );
				delete node;

				return nullptr;
			}
		}

		ExpressionNode *PostfixExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = PrimaryExpression();

			if ( expression == nullptr || ( expression->GetType() != Node::Type::IDENTIFIER && expression->GetType() != Node::Type::THIS ) )
			{
				return expression;
			}

			for (;;)
			{
				if ( mToken->type == static_cast<Token::Type>( u'[' ) )
				{
					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_INDEX );
						delete expression;

						return nullptr;
					}

					expression = new IndexReferenceNode( tToken->location, expression, Expression() );

					if ( mToken->type == static_cast<Token::Type>( u']' ) )
					{
						mToken++;
						if ( mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_INDEX );
							delete expression;

							return nullptr;
						}
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_INDEX );
						delete expression;

						return nullptr;
					}
				}
				else if ( mToken->type == static_cast<Token::Type>( u'(' ) )
				{
					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_ROUTINE_CALL );
						delete expression;

						return nullptr;
					}

					RoutineCallNode *node = new RoutineCallNode( tToken->location, expression );

					if ( mToken->type != static_cast<Token::Type>( u')' ) )
					{
						for (;;)
						{
							node->AddArgument( Expression() );

							if ( mToken->type == static_cast<Token::Type>( u',' ) )
							{
								mToken++;
								if ( mToken->type == Token::Type::END_OF_FILE )
								{
									ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_ROUTINE_CALL );
									delete node;

									return nullptr;
								}
							}
							else if ( mToken->type == static_cast<Token::Type>( u')' ) )
							{
								break;
							}
							else
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_ROUTINE_CALL );
								delete node;

								return nullptr;
							}
						}
					}

					mToken++;

					return node;
				}
				else if ( mToken->type == static_cast<Token::Type>( u'.' ) )
				{
					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_MEMBER );
						delete expression;

						return nullptr;
					}

					if ( mToken->type == Token::Type::IDENTIFIER )
					{
						expression = new MemberReferenceNode( tToken->location, expression, new IdentifierNode( mToken->location, mToken->value.identifier ) );

						mToken++;
						if ( mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_MEMBER );
							delete expression;

							return nullptr;
						}
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_MEMBER );
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

		ExpressionNode *UnaryExpression()
		{
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );

				return nullptr;
			}
			else if ( mToken->type != static_cast<Token::Type>( u'+' ) && mToken->type != static_cast<Token::Type>( u'-' ) && mToken->type != static_cast<Token::Type>( u'~' ) && mToken->type != static_cast<Token::Type>( u'!' ) )
			{
				return PostfixExpression();
			}
			else
			{
				return new UnaryExpressionNode( mToken->location, mToken++->type, UnaryExpression() );
			}
		}

		ExpressionNode *MultiplicativeExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = UnaryExpression();

			while ( mToken->type == static_cast<Token::Type>( u'*' ) || mToken->type == static_cast<Token::Type>( u'/' ) || mToken->type == static_cast<Token::Type>( u'%' ) )
			{
				expression = new MultiplicativeExpressionNode( tToken->location, mToken++->type, expression, UnaryExpression() );
			}

			return expression;
		}

		ExpressionNode *AdditiveExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = MultiplicativeExpression();

			while ( mToken->type == static_cast<Token::Type>( u'+' ) || mToken->type == static_cast<Token::Type>( u'-' ) )
			{
				expression = new AdditiveExpressionNode( tToken->location, mToken++->type, expression, MultiplicativeExpression() );
			}

			return expression;
		}

		ExpressionNode *ShiftExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = AdditiveExpression();

			while ( mToken->type == Token::Type::SHIFT_LEFT || mToken->type == Token::Type::SHIFT_RIGHT )
			{
				expression = new ShiftExpressionNode( tToken->location, mToken++->type, expression, AdditiveExpression() );
			}

			return expression;
		}

		ExpressionNode *AndExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = ShiftExpression();

			while ( mToken->type == static_cast<Token::Type>( u'&' ) )
			{
				mToken++;

				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
					delete expression;

					return nullptr;
				}

				expression = new AndExpressionNode( tToken->location, expression, ShiftExpression() );
			}

			return expression;
		}

		ExpressionNode *OrExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = AndExpression();

			while ( mToken->type == static_cast<Token::Type>( u'|' ) || mToken->type == static_cast<Token::Type>( u'^' ) )
			{
				expression = new OrExpressionNode( tToken->location, mToken++->type, expression, AndExpression() );
			}

			return expression;
		}

		ExpressionNode *RelationalExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = OrExpression();

			while ( mToken->type == static_cast<Token::Type>( u'<' ) || mToken->type == static_cast<Token::Type>( u'>' ) || mToken->type == Token::Type::LESS_THAN_OR_EQUAL || mToken->type == Token::Type::GREATER_THAN_OR_EQUAL )
			{
				expression = new RelationalExpressionNode( tToken->location, mToken++->type, expression, OrExpression() );
			}

			return expression;
		}

		ExpressionNode *EqualityExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = RelationalExpression();

			while ( mToken->type == Token::Type::EQUAL || mToken->type == Token::Type::NOT_EQUAL )
			{
				expression = new EqualityExpressionNode( tToken->location, mToken++->type, expression, RelationalExpression() );
			}

			return expression;
		}

		ExpressionNode *LogicalAndExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = EqualityExpression();

			while ( mToken->type == Token::Type::AND )
			{
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
					delete expression;

					return nullptr;
				}

				expression = new LogicalAndExpressionNode( tToken->location, expression, EqualityExpression() );
			}

			return expression;
		}

		ExpressionNode *LogicalOrExpression()
		{
			auto tToken = mToken;
			ExpressionNode *expression = LogicalAndExpression();

			while ( mToken->type == Token::Type::OR )
			{
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
					delete expression;

					return nullptr;
				}

				expression = new LogicalOrExpressionNode( tToken->location, expression, LogicalAndExpression() );
			}

			return expression;
		}

		ExpressionNode *AssignmentExpression()
		{
			auto tToken = mToken;

			switch ( mToken->type )
			{
			case Token::Type::ROUTINE:
				mToken++;
				if ( mToken->type == Token::Type::IDENTIFIER )
				{
					IdentifierNode *identifier = new IdentifierNode( mToken->location, mToken->value.identifier );

					mToken++;
					if ( mToken->type == static_cast<Token::Type>( u'.' ) )
					{
						mToken++;
						if ( mToken->type == Token::Type::IDENTIFIER )
						{
							IdentifierNode *member = new IdentifierNode( mToken->location, mToken->value.identifier );

							mToken++;
							if ( mToken->type == static_cast<Token::Type>( u'(' ) )
							{
								return new AssignmentExpressionNode( tToken->location, new MemberReferenceNode( identifier->location, identifier, member ), RoutineLiteral( tToken ) );
							}
							else
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
								delete identifier;
								delete member;

								return nullptr;
							}
						}
						else
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
							delete identifier;

							return nullptr;
						}
					}
					else if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
						delete identifier;

						return nullptr;
					}
					else
					{
						return new AssignmentExpressionNode( tToken->location, identifier, RoutineLiteral( tToken ) );
					}
				}
				else if ( mToken->type == Token::Type::THIS )
				{
					ThisNode *thisNode = new ThisNode( mToken->location );

					mToken++;
					if ( mToken->type == static_cast<Token::Type>( u'.' ) )
					{
						mToken++;
						if ( mToken->type == Token::Type::IDENTIFIER )
						{
							IdentifierNode *identifier = new IdentifierNode( mToken->location, mToken->value.identifier );

							mToken++;
							if ( mToken->type == static_cast<Token::Type>( u'(' ) )
							{
								return new AssignmentExpressionNode( tToken->location, new MemberReferenceNode( thisNode->location, thisNode, identifier ), RoutineLiteral( tToken ) );
							}
							else
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
								delete thisNode;
								delete identifier;

								return nullptr;
							}
						}
						else
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
							delete thisNode;

							return nullptr;
						}
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );
						delete thisNode;

						return nullptr;
					}
				}
				else if ( mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_ROUTINE );

					return nullptr;
				}
				else
				{
					mToken = tToken;

					return PrimaryExpression();
				}

			case Token::Type::CLASS:
				return Class();

			case Token::Type::PACKAGE:
				return Package();

			case Token::Type::END_OF_FILE:
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );

				return nullptr;

			default:
				ExpressionNode *expression = LogicalOrExpression();

				if ( expression == nullptr || mToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return expression;
				}
				else
				{
					if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::MEMBER_REFERENCE || expression->GetType() == Node::Type::INDEX_REFERENCE )
					{
						mToken++;

						if ( mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_EXPRESSION );
							delete expression;

							return nullptr;
						}

						return new AssignmentExpressionNode( tToken->location, expression, AssignmentExpression() );
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_LHS );
						delete expression;

						return nullptr;
					}
				}
			}
		}

		AssignmentExpressionNode *Class()
		{
			auto tToken = mToken;

			mToken++;
			if ( mToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( mToken->location, mToken->value.identifier );
				ClassNode *node = new ClassNode( tToken->location );

				mToken++;
				if ( mToken->type == static_cast<Token::Type>( u'(' ) )
				{
					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
						delete node;
						delete name;

						return nullptr;
					}

					if ( mToken->type != static_cast<Token::Type>( u')' ) )
					{
						for (;;)
						{
							node->AddArgument( Expression() );

							if ( mToken->type == static_cast<Token::Type>( u',' ) )
							{
								mToken++;
								if ( mToken->type == Token::Type::END_OF_FILE )
								{
									ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
									delete node;
									delete name;

									return nullptr;
								}
							}
							else if ( mToken->type == static_cast<Token::Type>( u')' ) )
							{
								break;
							}
							else
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
								delete node;
								delete name;

								return nullptr;
							}
						}
					}
				}
				else
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
					delete node;
					delete name;

					return nullptr;
				}

				mToken++;
				if ( mToken->type == static_cast<Token::Type>( u':' ) )
				{
					node->baseClassConstructorCall = new BaseClassConstructorCallNode( mToken->location );

					mToken++;
					if ( mToken->type == Token::Type::IDENTIFIER )
					{
						node->baseClassConstructorCall->baseClass = new IdentifierNode( mToken->location, mToken->value.identifier );

						mToken++;
						if ( mToken->type == static_cast<Token::Type>( u'(' ) )
						{
							mToken++;
							if ( mToken->type == Token::Type::END_OF_FILE )
							{
								ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
								delete node;
								delete name;

								return nullptr;
							}

							if ( mToken->type != static_cast<Token::Type>( u')' ) )
							{
								for (;;)
								{
									node->baseClassConstructorCall->AddArgument( Expression() );

									if ( mToken->type == static_cast<Token::Type>( u',' ) )
									{
										mToken++;
										if ( mToken->type == Token::Type::END_OF_FILE )
										{
											ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
											delete node;
											delete name;

											return nullptr;
										}
									}
									else if ( mToken->type == static_cast<Token::Type>( u')' ) )
									{
										break;
									}
									else
									{
										ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
										delete node;
										delete name;

										return nullptr;
									}
								}
							}
						}
						else
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION );
							delete node;
							delete name;

							return nullptr;
						}
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_BASE_CLASS );
						delete node;
						delete name;

						return nullptr;
					}
				}

				mToken++;
				if ( mToken->type == Token::Type::INCLUDE )
				{
					node->include = Include();
				}

				node->block = Block();

				mToken++;
				if ( mToken->type == Token::Type::END )
				{
					mToken++;

					return new AssignmentExpressionNode( tToken->location, name, node );
				}
				else
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_END );
					delete node;
					delete name;

					return nullptr;
				}
			}
			else
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_CLASS_NAME );

				return nullptr;
			}
		}

		IncludeNode *Include()
		{
			IncludeNode *node = new IncludeNode( mToken->location );

			do
			{
				mToken++;
				if ( mToken->type == Token::Type::IDENTIFIER )
				{
					node->AddPackage( new IdentifierNode( mToken->location, mToken++->value.identifier ) );
				}
				else
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_PACKAGE );
					delete node;

					return nullptr;
				}
			}
			while ( mToken->type == static_cast<Token::Type>( u',' ) );

			return node;
		}

		AssignmentExpressionNode *Package()
		{
			auto tToken = mToken;

			mToken++;
			if ( mToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( mToken->location, mToken->value.identifier );
				PackageNode *node = new PackageNode( tToken->location, Block() );

				mToken++;
				if ( mToken->type == Token::Type::END )
				{
					mToken++;

					return new AssignmentExpressionNode( tToken->location, name, node );
				}
				else
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_END );
					delete node;
					delete name;

					return nullptr;
				}
			}
			else
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_PACKAGE_NAME );

				return nullptr;
			}
		}

		ExpressionNode *Expression()
		{
			return AssignmentExpression();
		}

		ImportNode *Import()
		{
			ImportNode *node = new ImportNode( mToken->location );

			do
			{
				mToken++;
				if ( mToken->type == Token::Type::IDENTIFIER )
				{
					node->AddIdentifier( new IdentifierNode( mToken->location, mToken++->value.identifier ) );
				}
				else
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_IDENTIFIER );
					delete node;

					return nullptr;
				}
			}
			while ( mToken->type == static_cast<Token::Type>( u'.' ) );

			return node;
		}

		IfNode *If()
		{
			IfNode *node = new IfNode( mToken->location );
			ElseIfNode *elseIfNode;

			for (;;)
			{
				elseIfNode = new ElseIfNode( mToken->location );

				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT );
					delete elseIfNode;
					delete node;

					return nullptr;
				}

				elseIfNode->condition = Expression();

				if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
				{
					mToken++;
				}

				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT );
					delete elseIfNode;
					delete node;

					return nullptr;
				}

				elseIfNode->block = Block();
				node->AddElseIf( elseIfNode );

				if ( mToken->type == Token::Type::END )
				{
					mToken++;

					return node;
				}
				else if ( mToken->type == Token::Type::ELSE )
				{
					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT );
						delete node;

						return nullptr;
					}

					node->block = Block();

					if ( mToken->type == Token::Type::END )
					{
						mToken++;

						return node;
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_END );
						delete node;

						return nullptr;
					}
				}
				else if ( mToken->type != Token::Type::ELSEIF )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_END_ELSE_ELSEIF );
					delete node;

					return nullptr;
				}
			}
		}

		CaseNode *Case()
		{
			CaseNode *node = new CaseNode( mToken->location );

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
				delete node;

				return nullptr;
			}

			node->value = Expression();

			if ( mToken->type == Token::Type::WHEN )
			{
				WhenNode *whenNode;

				for (;;)
				{
					whenNode = new WhenNode( mToken->location );

					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
						delete whenNode;
						delete node;

						return nullptr;
					}

					whenNode->condition = Expression();

					if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
					{
						mToken++;
					}

					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
						delete whenNode;
						delete node;

						return nullptr;
					}

					whenNode->block = Block();
					node->AddWhen( whenNode );

					if ( mToken->type == Token::Type::ELSE )
					{
						mToken++;
						if ( mToken->type == Token::Type::END_OF_FILE )
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT );
							delete node;

							return nullptr;
						}

						node->block = Block();

						if ( mToken->type == Token::Type::END )
						{
							mToken++;

							return node;
						}
						else
						{
							ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_END );
							delete node;

							return nullptr;
						}
					}
					else if ( mToken->type == Token::Type::END )
					{
						mToken++;

						return node;
					}
					else if ( mToken->type != Token::Type::WHEN )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_WHEN_ELSE_ELSEIF );
						delete node;

						return nullptr;
					}
				}
			}
			else
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_WHEN );
				delete node;

				return nullptr;
			}
		}

		WhileNode *While()
		{
			WhileNode *node = new WhileNode( mToken->location );

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_WHILE_STATEMENT );
				delete node;

				return nullptr;
			}

			node->condition = Expression();

			if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
			{
				mToken++;
			}

			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_WHILE_STATEMENT );
				delete node;

				return nullptr;
			}

			node->block = Block();

			if ( mToken->type == Token::Type::END )
			{
				mToken++;

				return node;
			}
			else
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_END );
				delete node;

				return nullptr;
			}
		}

		ForNode *For()
		{
			ForNode *node = new ForNode( mToken->location );

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
				delete node;

				return nullptr;
			}

			node->initializer = Expression();

			if ( mToken->type == static_cast<Token::Type>( u',' ) )
			{
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
					delete node;

					return nullptr;
				}

				node->condition = Expression();

				if ( mToken->type == static_cast<Token::Type>( u',' ) )
				{
					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
						delete node;

						return nullptr;
					}

					node->iterator = Expression();

					if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
					{
						mToken++;
					}

					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
						delete node;

						return nullptr;
					}

					node->block = Block();

					if ( mToken->type == Token::Type::END )
					{
						mToken++;

						return node;
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_END );
						delete node;

						return nullptr;
					}
				}
				else
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
					delete node;

					return nullptr;
				}
			}
			else
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT );
				delete node;

				return nullptr;
			}
		}

		ForEachNode *ForEach()
		{
			ForEachNode *node = new ForEachNode( mToken->location );

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
				delete node;

				return nullptr;
			}

			node->variable = Expression();

			if ( node->variable ->GetType() == Node::Type::IDENTIFIER || node->variable ->GetType() == Node::Type::MEMBER_REFERENCE || node->variable ->GetType() == Node::Type::INDEX_REFERENCE )
			{
				if ( mToken->type == Token::Type::IN )
				{
					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
						delete node;

						return nullptr;
					}

					node->collection = Expression();

					if ( node->collection->GetType() != Node::Type::IDENTIFIER && node->collection->GetType() != Node::Type::MEMBER_REFERENCE && node->collection->GetType() != Node::Type::INDEX_REFERENCE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_LHS );
						delete node;

						return nullptr;
					}

					if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
					{
						mToken++;
					}

					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
						delete node;

						return nullptr;
					}

					node->block = Block();

					if ( mToken->type == Token::Type::END )
					{
						mToken++;

						return node;
					}
					else
					{
						ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_END );
						delete node;

						return nullptr;
					}
				}
				else
				{
					ErrorHandler::OnError( mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT );
					delete node;

					return nullptr;
				}
			}
			else
			{
				ErrorHandler::OnError( mToken->location, ErrorCode::EXPECTED_LHS );
				delete node;

				return nullptr;
			}
		}

		BreakNode *Break()
		{
			return new BreakNode( mToken++->location );
		}

		NextNode *Next()
		{
			return new NextNode( mToken++->location );
		}

		ReturnNode *Return()
		{
			auto tToken = mToken;

			mToken++;

			if ( mToken->type == Token::Type::END || mToken->type == Token::Type::ELSE || mToken->type == Token::Type::ELSEIF || mToken->type == Token::Type::PRIVATE || mToken->type == Token::Type::PUBLIC || mToken->type == Token::Type::WHEN || mToken->type == Token::Type::END_OF_FILE )
			{
				return new ReturnNode( tToken->location );
			}
			else
			{
				return new ReturnNode( tToken->location, Expression() );
			}
		}
	};
}

#endif
