#include <forward_list>

#include "Tokenizer.h"
#include "Token.h"
#include "Node.h"
#include "Literal.h"
#include "Location.h"

#include "WarningCode.h"
#include "ErrorCode.h"
#include "BuildLog.h"

#ifndef PARSER
#define PARSER

namespace lyrics
{
	using std::forward_list;

	class Parser
	{
	public:
		bool Parse( const char * const fileName, BlockNode *&root )
		{
			forward_list<Token> token;

			if ( !Tokenizer().Tokenize( fileName, token ) )
			{
				return false;
			}

			mToken = token.cbegin();

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

			while ( mToken->type != Token::Type::END && mToken->type != Token::Type::ELSE && mToken->type != Token::Type::ELSEIF && mToken->type != Token::Type::WHEN && mToken->type != Token::Type::END_OF_FILE )
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

			case Token::Type::PRIVATE:
				return Private();

			case Token::Type::RETURN:
				return Return();

			case Token::Type::BREAK:
				return Break();

			case Token::Type::WHILE:
				return While();

			case Token::Type::PUBLIC:
				return Public();

			case Token::Type::FOREACH:
				return ForEach();

			case Token::Type::CASE:
				return Case();

			case Token::Type::REDO:
				return Redo();
			
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
				return new LiteralNode( mToken->location, mToken++->value.integer );

			case Token::Type::STRING_LITERAL:
				return new LiteralNode( mToken->location, mToken++->value.string );

			case Token::Type::BOOLEAN_LITERAL:
				return new LiteralNode( mToken->location, mToken++->value.boolean );

			case Token::Type::NIL_LITERAL:
				return new LiteralNode( mToken++->location );

			case Token::Type::REAL_LITERAL:
				return new LiteralNode( mToken->location, mToken++->value.real );

			case Token::Type::DEF:
				return FunctionLiteral( mToken++ );

			case static_cast<Token::Type>( u'(' ):
				return ParenthesizedExpression();

			case static_cast<Token::Type>( u'[' ):
				return ArrayLiteral();

			case static_cast<Token::Type>( u'{' ):
				return HashLiteral();
			
			default:
				BuildLog::Error( ErrorCode::EXPECTED_PRIMARY_EXPRESSION, mToken->location );

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
				BuildLog::Error( ErrorCode::INCOMPLETE_ARRAY_LITERAL, mToken->location );
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
							BuildLog::Error( ErrorCode::INCOMPLETE_ARRAY_LITERAL, mToken->location );
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
						BuildLog::Error( ErrorCode::INCOMPLETE_ARRAY_LITERAL, mToken->location );
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
				BuildLog::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
				delete node;

				return nullptr;
			}

			if ( mToken->type != static_cast<Token::Type>( u'}' ) )
			{
				ExpressionNode *expression;

				for (;;)
				{
					if ( mToken->type != static_cast<Token::Type>( u'[' ) )
					{
						BuildLog::Error( ErrorCode::EXPECTED_HASH, mToken->location );
						delete node;

						return nullptr;
					}

					auto tToken = mToken;

					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						BuildLog::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
						delete node;

						return nullptr;
					}

					expression = Expression();

					if ( mToken->type != static_cast<Token::Type>( u']' ) )
					{
						BuildLog::Error( ErrorCode::EXPECTED_HASH, mToken->location );
						delete expression;
						delete node;

						return nullptr;
					}

					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						BuildLog::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
						delete expression;
						delete node;

						return nullptr;
					}

					if ( mToken->type != static_cast<Token::Type>( u'=' ) )
					{
						BuildLog::Error( ErrorCode::EXPECTED_HASH, mToken->location );
						delete expression;
						delete node;

						return nullptr;
					}

					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						BuildLog::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
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
							BuildLog::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
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
						BuildLog::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mToken->location );
						delete node;

						return nullptr;
					}
				}
			}

			mToken++;

			return node;
		}

		FunctionLiteralNode *FunctionLiteral( forward_list<Token>::const_iterator token )
		{
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );

				return nullptr;
			}

			if ( mToken->type == static_cast<Token::Type>( u'(' ) )
			{
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );

					return nullptr;
				}

				FunctionLiteralNode *node = new FunctionLiteralNode( token->location );

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
								BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
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
								BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
								delete name;
								delete node;

								return nullptr;
							}
						}
						else
						{
							BuildLog::Error( ErrorCode::EXPECTED_PARAMETER_NAME, mToken->location );
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
								BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
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
								BuildLog::Error( ErrorCode::OUTPUT_PARAMETER_DEFAULT_ARGUMENT, mToken->location );
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
								BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
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
							BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
							delete node;

							return nullptr;
						}
					}
				}

				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );
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
					BuildLog::Error( ErrorCode::EXPECTED_END, mToken->location );
					delete node;

					return nullptr;
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_PARAMETER, mToken->location );

				return nullptr;
			}
		}

		ParenthesizedExpressionNode *ParenthesizedExpression()
		{
			ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode( mToken->location );

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
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
				BuildLog::Error( ErrorCode::EXPECTED_RIGHT_PARENTHESIS, mToken->location );
				delete node;

				return nullptr;
			}
		}

		ExpressionNode *PostfixExpression()
		{
			if ( mToken->type == Token::Type::IMPORT )
			{
				return Import();
			}

			auto tToken = mToken;
			ExpressionNode *expression = PrimaryExpression();

			if ( expression == nullptr || expression->GetType() != Node::Type::IDENTIFIER )
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
						BuildLog::Error( ErrorCode::EXPECTED_INDEX, mToken->location );
						delete expression;

						return nullptr;
					}

					expression = new IndexReferenceNode( tToken->location, expression, Expression() );

					if ( mToken->type == static_cast<Token::Type>( u']' ) )
					{
						mToken++;
						if ( mToken->type == Token::Type::END_OF_FILE )
						{
							BuildLog::Error( ErrorCode::EXPECTED_INDEX, mToken->location );
							delete expression;

							return nullptr;
						}
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_INDEX, mToken->location );
						delete expression;

						return nullptr;
					}
				}
				else if ( mToken->type == static_cast<Token::Type>( u'(' ) )
				{
					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						BuildLog::Error( ErrorCode::EXPECTED_FUNCTION_CALL, mToken->location );
						delete expression;

						return nullptr;
					}

					FunctionCallNode *node = new FunctionCallNode( tToken->location, expression );

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
									BuildLog::Error( ErrorCode::EXPECTED_FUNCTION_CALL, mToken->location );
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
								BuildLog::Error( ErrorCode::EXPECTED_FUNCTION_CALL, mToken->location );
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
						BuildLog::Error( ErrorCode::EXPECTED_MEMBER, mToken->location );
						delete expression;

						return nullptr;
					}

					if ( mToken->type == Token::Type::IDENTIFIER )
					{
						expression = new MemberReferenceNode( tToken->location, expression, new IdentifierNode( mToken->location, mToken->value.identifier ) );

						mToken++;
						if ( mToken->type == Token::Type::END_OF_FILE )
						{
							BuildLog::Error( ErrorCode::EXPECTED_MEMBER, mToken->location );
							delete expression;

							return nullptr;
						}
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_MEMBER, mToken->location );
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

		ImportNode *Import()
		{
			auto tToken = mToken;

			mToken++;
			if ( mToken->type == Token::Type::IDENTIFIER )
			{
				return new ImportNode( tToken->location, new IdentifierNode( mToken->location, mToken++->value.identifier ) );
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_PACKAGE, mToken->location );

				return nullptr;
			}
		}

		ExpressionNode *UnaryExpression()
		{
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );

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
					BuildLog::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
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
					BuildLog::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
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
					BuildLog::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
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
			case Token::Type::END_OF_FILE:
				BuildLog::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );

				return nullptr;

			case Token::Type::DEF:
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mToken->location );

					return nullptr;
				}

				if ( mToken->type == Token::Type::IDENTIFIER )
				{
					return new AssignmentExpressionNode( tToken->location, new IdentifierNode( mToken->location, mToken++->value.identifier ), FunctionLiteral( tToken ) );
				}
				else
				{
					mToken = tToken;

					return PrimaryExpression();
				}

			case Token::Type::CLASS:
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );

					return nullptr;
				}

				if ( mToken->type == Token::Type::IDENTIFIER )
				{
					return new AssignmentExpressionNode( tToken->location, new IdentifierNode( mToken->location, mToken++->value.identifier ), Class( tToken ) );
				}
				else
				{
					BuildLog::Error( ErrorCode::EXPECTED_CLASS_NAME, mToken->location );

					return nullptr;
				}

			case Token::Type::PACKAGE:
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_PACKAGE_DEFINITION, mToken->location );

					return nullptr;
				}

				if ( mToken->type == Token::Type::IDENTIFIER )
				{
					return new AssignmentExpressionNode( tToken->location, new IdentifierNode( mToken->location, mToken++->value.identifier ), Package( tToken ) );
				}
				else
				{
					BuildLog::Error( ErrorCode::EXPECTED_PACKAGE_NAME, mToken->location );

					return nullptr;
				}

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
							BuildLog::Error( ErrorCode::INCOMPLETE_EXPRESSION, mToken->location );
							delete expression;

							return nullptr;
						}

						return new AssignmentExpressionNode( tToken->location, expression, AssignmentExpression() );
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_LHS, mToken->location );
						delete expression;

						return nullptr;
					}
				}
			}
		}

		ClassNode *Class( forward_list<Token>::const_iterator token )
		{
			ClassNode *node = new ClassNode( token->location );

			if ( mToken->type == static_cast<Token::Type>( u':' ) )
			{
				mToken++;
				if ( mToken->type == Token::Type::IDENTIFIER )
				{
					node->base = new IdentifierNode( mToken->location, mToken->value.identifier );

					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						BuildLog::Error( ErrorCode::INCOMPLETE_CLASS_DEFINITION, mToken->location );
						delete node;

						return nullptr;
					}
				}
				else
				{
					BuildLog::Error( ErrorCode::EXPECTED_BASE_CLASS, mToken->location );
					delete node;

					return nullptr;
				}
			}

			node->block = Block();

			if ( mToken->type == Token::Type::END )
			{
				mToken++;

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_END, mToken->location );
				delete node;

				return nullptr;
			}
		}

		PackageNode *Package( forward_list<Token>::const_iterator token )
		{
			PackageNode *node = new PackageNode( token->location, Block() );

			if ( mToken->type == Token::Type::END )
			{
				mToken++;

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_END, mToken->location );
				delete node;

				return nullptr;
			}
		}

		ExpressionNode *Expression()
		{
			return AssignmentExpression();
		}

		PublicNode *Public()
		{
			auto tToken = mToken;

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_VARIABLE_DEFINITION, mToken->location );

				return nullptr;
			}

			ExpressionNode *expression = Expression();

			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_VARIABLE_DEFINITION, mToken->location );

				return nullptr;
			}

			if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::ASSIGNMENT_EXPRESSION )
			{
				return new PublicNode( tToken->location, expression );
			}
			else
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_VARIABLE_DEFINITION, mToken->location );

				return nullptr;
			}
		}

		PrivateNode *Private()
		{
			auto tToken = mToken;

			mToken++;
			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_VARIABLE_DEFINITION, mToken->location );

				return nullptr;
			}

			ExpressionNode *expression = Expression();

			if ( mToken->type == Token::Type::END_OF_FILE )
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_VARIABLE_DEFINITION, mToken->location );

				return nullptr;
			}

			if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::ASSIGNMENT_EXPRESSION )
			{
				return new PrivateNode( tToken->location, expression );
			}
			else
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_VARIABLE_DEFINITION, mToken->location );

				return nullptr;
			}
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
					BuildLog::Error( ErrorCode::INCOMPLETE_IF_STATEMENT, mToken->location );
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
					BuildLog::Error( ErrorCode::INCOMPLETE_IF_STATEMENT, mToken->location );
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
						BuildLog::Error( ErrorCode::INCOMPLETE_IF_STATEMENT, mToken->location );
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
						BuildLog::Error( ErrorCode::EXPECTED_END, mToken->location );
						delete node;

						return nullptr;
					}
				}
				else if ( mToken->type != Token::Type::ELSEIF )
				{
					BuildLog::Error( ErrorCode::EXPECTED_END_ELSE_ELSEIF, mToken->location );
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
				BuildLog::Error( ErrorCode::INCOMPLETE_CASE_STATEMENT, mToken->location );
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
						BuildLog::Error( ErrorCode::INCOMPLETE_CASE_STATEMENT, mToken->location );
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
						BuildLog::Error( ErrorCode::INCOMPLETE_CASE_STATEMENT, mToken->location );
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
							BuildLog::Error( ErrorCode::INCOMPLETE_CASE_STATEMENT, mToken->location );
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
							BuildLog::Error( ErrorCode::EXPECTED_END, mToken->location );
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
						BuildLog::Error( ErrorCode::EXPECTED_WHEN_ELSE_ELSEIF, mToken->location );
						delete node;

						return nullptr;
					}
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_WHEN, mToken->location );
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
				BuildLog::Error( ErrorCode::INCOMPLETE_WHILE_STATEMENT, mToken->location );
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
				BuildLog::Error( ErrorCode::INCOMPLETE_WHILE_STATEMENT, mToken->location );
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
				BuildLog::Error( ErrorCode::EXPECTED_END, mToken->location );
				delete node;

				return nullptr;
			}
		}

		ForNode *For()
		{
			ForNode *node = new ForNode( mToken->location );

			mToken++;
			switch ( mToken->type )
			{
			case Token::Type::END_OF_FILE:
				BuildLog::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
				delete node;

				return nullptr;

			case Token::Type::PUBLIC:
				node->initializer = Public();
				break;

			case Token::Type::PRIVATE:
				node->initializer = Private();
				break;

			default:
				auto tToken = mToken;
				ExpressionNode *expression = Expression();

				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
					delete expression;
					delete node;

					return nullptr;
				}

				if ( expression->GetType() == Node::Type::ASSIGNMENT_EXPRESSION )
				{
					node->initializer = new PublicNode( tToken->location, expression );
				}
				else
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
					delete expression;
					delete node;

					return nullptr;
				}
				break;
			}

			if ( mToken->type == static_cast<Token::Type>( u',' ) )
			{
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
					delete node;

					return nullptr;
				}

				node->condition = Expression();

				if ( mToken->type == static_cast<Token::Type>( u',' ) )
				{
					mToken++;
					if ( mToken->type == Token::Type::END_OF_FILE )
					{
						BuildLog::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
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
						BuildLog::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
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
						BuildLog::Error( ErrorCode::EXPECTED_END, mToken->location );
						delete node;

						return nullptr;
					}
				}
				else
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
					delete node;

					return nullptr;
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_FOR_STATEMENT, mToken->location );
				delete node;

				return nullptr;
			}
		}

		ForEachNode *ForEach()
		{
			ForEachNode *node = new ForEachNode( mToken->location );

			mToken++;
			switch ( mToken->type )
			{
			case Token::Type::END_OF_FILE:
				BuildLog::Error( ErrorCode::INCOMPLETE_FOREACH_STATEMENT, mToken->location );
				delete node;

				return nullptr;

			case Token::Type::PUBLIC:
				node->variable = Public();
				break;

			case Token::Type::PRIVATE:
				node->variable = Private();
				break;

			default:
				auto tToken = mToken;
				ExpressionNode *expression = Expression();

				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FOREACH_STATEMENT, mToken->location );
					delete expression;
					delete node;

					return nullptr;
				}

				if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::MEMBER_REFERENCE || expression->GetType() == Node::Type::INDEX_REFERENCE )
				{
					node->variable = new PublicNode( tToken->location, expression );
				}
				else
				{
					BuildLog::Error( ErrorCode::EXPECTED_LHS, mToken->location );
					delete expression;
					delete node;

					return nullptr;
				}
				break;
			}

			if ( mToken->type == Token::Type::IN )
			{
				mToken++;
				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FOREACH_STATEMENT, mToken->location );
					delete node;

					return nullptr;
				}

				node->collection = Expression();

				if ( node->collection->GetType() != Node::Type::IDENTIFIER && node->collection->GetType() != Node::Type::MEMBER_REFERENCE && node->collection->GetType() != Node::Type::INDEX_REFERENCE )
				{
					BuildLog::Error( ErrorCode::EXPECTED_LHS, mToken->location );
					delete node;

					return nullptr;
				}

				if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
				{
					mToken++;
				}

				if ( mToken->type == Token::Type::END_OF_FILE )
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FOREACH_STATEMENT, mToken->location );
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
					BuildLog::Error( ErrorCode::EXPECTED_END, mToken->location );
					delete node;

					return nullptr;
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_FOREACH_STATEMENT, mToken->location );
				delete node;

				return nullptr;
			}
		}

		RedoNode *Redo()
		{
			return new RedoNode( mToken++->location );
		}

		BreakNode *Break()
		{
			return new BreakNode( mToken++->location );
		}

		ReturnNode *Return()
		{
			auto tToken = mToken;

			mToken++;

			if ( mToken->type == Token::Type::END || mToken->type == Token::Type::ELSE || mToken->type == Token::Type::ELSEIF || mToken->type == Token::Type::WHEN || mToken->type == Token::Type::END_OF_FILE )
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
