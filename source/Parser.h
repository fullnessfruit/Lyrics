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
		BlockNode *Parse( const char * const fileName )
		{
			forward_list<Token> token;
			Tokenizer().Tokenize( fileName, token );

			mCurrentToken = token.cbegin();
			mLastToken = token.cend();

			return Block();
		}
	
	private:
		forward_list<Token>::const_iterator mCurrentToken;
		forward_list<Token>::const_iterator mLastToken;

		BlockNode *Block()
		{
			BlockNode *node = new BlockNode( mCurrentToken->location );

			node->last = node->list.cbefore_begin();
			if ( mCurrentToken->type != Token::Type::END && mCurrentToken->type != Token::Type::ELSE && mCurrentToken->type != Token::Type::ELSEIF && mCurrentToken->type != Token::Type::WHEN && mCurrentToken != mLastToken )
			{
				node->last = node->list.insert_after( node->last, Statement() );
			}

			return node;
		}

		StatementNode *Statement()
		{
			switch ( mCurrentToken->type )
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

			case Token::Type::CLASS:
				return Class();

			case Token::Type::WHILE:
				return While();

			case Token::Type::IMPORT:
				return Import();

			case Token::Type::PUBLIC:
				return Public();

			case Token::Type::PACKAGE:
				return Package();

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
			switch ( mCurrentToken->type )
			{
			case Token::Type::IDENTIFIER:
				return new IdentifierNode( mCurrentToken->location, ( mCurrentToken++ )->value.identifier );

			case Token::Type::INTEGER_LITERAL:
				{
					LiteralNode *node = new LiteralNode( mCurrentToken->location, mCurrentToken->value.integer );

					mCurrentToken++;

					return node;
				}

			case Token::Type::STRING_LITERAL:
				{
					LiteralNode *node = new LiteralNode( mCurrentToken->location, mCurrentToken->value.string );

					mCurrentToken++;

					return node;
				}

			case Token::Type::BOOLEAN_LITERAL:
				{
					LiteralNode *node = new LiteralNode( mCurrentToken->location, mCurrentToken->value.boolean );

					mCurrentToken++;

					return node;
				}

			case Token::Type::NIL_LITERAL:
				{
					LiteralNode *node = new LiteralNode( mCurrentToken->location );

					mCurrentToken++;

					return node;
				}

			case Token::Type::REAL_LITERAL:
				{
					LiteralNode *node = new LiteralNode( mCurrentToken->location, mCurrentToken->value.real );

					mCurrentToken++;

					return node;
				}

			case static_cast<Token::Type>( u'(' ):
				{
					ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode( mCurrentToken->location, Expression() );

					mCurrentToken++;

					if ( mCurrentToken->type == static_cast<Token::Type>( u')' ) )
					{
						mCurrentToken++;

						return node;
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_RIGHT_PARENTHESIS, mCurrentToken->location );
						delete node;

						return nullptr;
					}
				}

			case static_cast<Token::Type>( u'[' ):
				{
					ArrayLiteralNode *node = new ArrayLiteralNode( mCurrentToken->location );

					mCurrentToken++;

					if ( mCurrentToken->type != static_cast<Token::Type>( u']' ) )
					{
						node->last = node->list.cbefore_begin();
						for (;;)
						{
							node->last = node->list.insert_after( node->last, Expression() );

							if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
							{
								mCurrentToken++;
							}
							else if ( mCurrentToken->type == static_cast<Token::Type>( u']' ) )
							{
								break;
							}
							else
							{
								BuildLog::Error( ErrorCode::INCOMPLETE_ARRAY_LITERAL, mCurrentToken->location );
								delete node;

								return nullptr;
							}
						}
					}
					mCurrentToken++;

					return node;
				}

			case static_cast<Token::Type>( u'{' ):
				{
					HashLiteralNode *node = new HashLiteralNode( mCurrentToken->location );

					mCurrentToken++;

					if ( mCurrentToken->type != static_cast<Token::Type>( u'}' ) )
					{
						ExpressionNode *expression;

						node->last = node->list.cbefore_begin();
						for (;;)
						{
							if ( mCurrentToken->type != static_cast<Token::Type>( u'[' ) )
							{
								BuildLog::Error( ErrorCode::EXPECTED_HASH, mCurrentToken->location );
								delete node;

								return nullptr;
							}

							forward_list<Token>::const_iterator tToken = mCurrentToken;

							mCurrentToken++;

							expression = Expression();

							if ( mCurrentToken->type != static_cast<Token::Type>( u']' ) )
							{
								BuildLog::Error( ErrorCode::EXPECTED_HASH, mCurrentToken->location );
								delete expression;
								delete node;

								return nullptr;
							}
							mCurrentToken++;

							if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
							{
								BuildLog::Error( ErrorCode::EXPECTED_HASH, mCurrentToken->location );
								delete expression;
								delete node;

								return nullptr;
							}
							mCurrentToken++;

							node->last = node->list.insert_after( node->last, new HashNode( tToken->location, expression, Expression() ) );

							if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
							{
								mCurrentToken++;
							}
							else if ( mCurrentToken->type == static_cast<Token::Type>( u'}' ) )
							{
								break;
							}
							else
							{
								BuildLog::Error( ErrorCode::INCOMPLETE_HASH_LITERAL, mCurrentToken->location );
								delete node;

								return nullptr;
							}
						}
					}
					mCurrentToken++;

					return node;
				}

			case Token::Type::DEF:
				{
					mCurrentToken++;

					return FunctionLiteral();
				}
			
			default:
				BuildLog::Error( ErrorCode::EXPECTED_PRIMARY_EXPRESSION, mCurrentToken->location );

				return nullptr;
			}
		}

		FunctionLiteralNode *FunctionLiteral()
		{
			if ( mCurrentToken->type != static_cast<Token::Type>( u'(' ) )
			{
				FunctionLiteralNode *node = new FunctionLiteralNode( mCurrentToken->location );

				mCurrentToken++;

				if ( mCurrentToken->type != static_cast<Token::Type>( u')' ) )
				{
					ParameterNode *parameter;
					bool isValueParameter;
					IdentifierNode *name;

					node->last = node->list.cbefore_begin();
					for (;;)
					{
						forward_list<Token>::const_iterator tToken = mCurrentToken;

						if ( mCurrentToken->type != Token::Type::OUT )
						{
							isValueParameter = true;
						}
						else
						{
							mCurrentToken++;

							isValueParameter = false;
						}

						if ( mCurrentToken->type == Token::Type::IDENTIFIER )
						{
							name = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
							mCurrentToken++;
						}
						else
						{
							BuildLog::Error( ErrorCode::EXPECTED_PARAMETER_NAME, mCurrentToken->location );
							delete node;

							return nullptr;
						}

						if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
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
							mCurrentToken++;

							if ( isValueParameter )
							{
								parameter = new ValueParameterNode( tToken->location, name, Expression() );
							}
							else
							{
								BuildLog::Error( ErrorCode::OUTPUT_PARAMETER_DEFAULT_ARGUMENT, mCurrentToken->location );
								delete name;
								delete node;

								return nullptr;
							}
						}

						node->last = node->list.insert_after( node->last, parameter );

						if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
						{
							mCurrentToken++;
						}
						else if ( mCurrentToken->type == static_cast<Token::Type>( u')' ) )
						{
							break;
						}
						else
						{
							BuildLog::Error( ErrorCode::INCOMPLETE_FUNCTION, mCurrentToken->location );
							delete node;

							return nullptr;
						}
					}
				}
				mCurrentToken++;

				node->block = Block();

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_PARAMETER, mCurrentToken->location );

				return nullptr;
			}
		}

		ExpressionNode *PostfixExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = PrimaryExpression();

			if ( expression->GetType() != Node::Type::IDENTIFIER && expression->GetType() != Node::Type::MEMBER_REFERENCE && expression->GetType() != Node::Type::INDEX_REFERENCE )
			{
				return expression;
			}

			for (;;)
			{
				if ( mCurrentToken->type == static_cast<Token::Type>( u'[' ) )
				{
					mCurrentToken++;

					IndexReferenceNode *node = new IndexReferenceNode( tToken->location, expression, Expression() );

					if ( mCurrentToken->type == static_cast<Token::Type>( u']' ) )
					{
						mCurrentToken++;

						expression = static_cast<ExpressionNode *>( node );
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_INDEX, mCurrentToken->location );
						delete node;
						delete expression;

						return nullptr;
					}
				}
				else if ( mCurrentToken->type == static_cast<Token::Type>( u'(' ) )
				{
					mCurrentToken++;

					FunctionCallNode *node = new FunctionCallNode( tToken->location, expression );

					if ( mCurrentToken->type != static_cast<Token::Type>( u')' ) )
					{
						node->last = node->list.cbefore_begin();
						for (;;)
						{
							node->last = node->list.insert_after( node->last, Expression() );

							if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
							{
								mCurrentToken++;
							}
							else if ( mCurrentToken->type == static_cast<Token::Type>( u')' ) )
							{
								break;
							}
							else
							{
								BuildLog::Error( ErrorCode::EXPECTED_FUNCTION_CALL, mCurrentToken->location );
								delete node;
								delete expression;

								return nullptr;
							}
						}
					}
					mCurrentToken++;

					expression = static_cast<ExpressionNode *>( node );
				}
				else if ( mCurrentToken->type == static_cast<Token::Type>( u'.' ) )
				{
					mCurrentToken++;

					if ( mCurrentToken->type == Token::Type::IDENTIFIER )
					{
						MemberReferenceNode *node = new MemberReferenceNode( tToken->location, expression, new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier ) );

						mCurrentToken++;

						expression = static_cast<ExpressionNode *>( node );
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_MEMBER, mCurrentToken->location );
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
			if ( mCurrentToken->type != static_cast<Token::Type>( u'+' ) && mCurrentToken->type != static_cast<Token::Type>( u'-' ) && mCurrentToken->type != static_cast<Token::Type>( u'~' ) && mCurrentToken->type != static_cast<Token::Type>( u'!' ) )
			{
				return PostfixExpression();
			}
			else
			{
				UnaryExpressionNode *node = new UnaryExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->expression = UnaryExpression();

				return node;
			}
		}

		ExpressionNode *MultiplicativeExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = UnaryExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'*' ) && mCurrentToken->type != static_cast<Token::Type>( u'/' ) && mCurrentToken->type != static_cast<Token::Type>( u'%' ) )
			{
				return expression;
			}
			else
			{

				MultiplicativeExpressionNode *node = new MultiplicativeExpressionNode( tToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = MultiplicativeExpression();

				return node;
			}
		}

		ExpressionNode *AdditiveExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = MultiplicativeExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'+' ) && mCurrentToken->type != static_cast<Token::Type>( u'-' ) )
			{
				return expression;
			}
			else
			{
				AdditiveExpressionNode *node = new AdditiveExpressionNode( tToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = AdditiveExpression();

				return node;
			}
		}

		ExpressionNode *ShiftExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = AdditiveExpression();

			if ( mCurrentToken->type != Token::Type::SHIFT_LEFT && mCurrentToken->type != Token::Type::SHIFT_RIGHT )
			{
				return expression;
			}
			else
			{
				ShiftExpressionNode *node = new ShiftExpressionNode( tToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = ShiftExpression();

				return node;
			}
		}

		ExpressionNode *AndExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = ShiftExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'&' ) )
			{
				return expression;
			}
			else
			{
				AndExpressionNode *node = new AndExpressionNode( tToken->location );

				mCurrentToken++;

				node->left = expression;
				node->right = AndExpression();

				return node;
			}
		}

		ExpressionNode *OrExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = AndExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'|' ) && mCurrentToken->type != static_cast<Token::Type>( u'^' ) )
			{
				return expression;
			}
			else
			{
				OrExpressionNode *node = new OrExpressionNode( tToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = OrExpression();

				return node;
			}
		}

		ExpressionNode *RelationalExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = OrExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'<' ) && mCurrentToken->type != static_cast<Token::Type>( u'>' ) && mCurrentToken->type != Token::Type::LESS_THAN_OR_EQUAL && mCurrentToken->type != Token::Type::GREATER_THAN_OR_EQUAL )
			{
				return expression;
			}
			else
			{
				RelationalExpressionNode *node = new RelationalExpressionNode( tToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = RelationalExpression();

				return node;
			}
		}

		ExpressionNode *EqualityExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = RelationalExpression();

			if ( mCurrentToken->type != Token::Type::EQUAL && mCurrentToken->type != Token::Type::NOT_EQUAL )
			{
				return expression;
			}
			else
			{
				EqualityExpressionNode *node = new EqualityExpressionNode( tToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = EqualityExpression();

				return node;
			}
		}

		ExpressionNode *LogicalAndExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = EqualityExpression();

			if ( mCurrentToken->type != Token::Type::AND )
			{
				return expression;
			}
			else
			{
				LogicalAndExpressionNode *node = new LogicalAndExpressionNode( tToken->location );

				mCurrentToken++;

				node->left = expression;
				node->right = LogicalAndExpression();

				return node;
			}
		}

		ExpressionNode *LogicalOrExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;
			ExpressionNode *expression = LogicalAndExpression();

			if ( mCurrentToken->type != Token::Type::OR )
			{
				return expression;
			}
			else
			{
				LogicalOrExpressionNode *node = new LogicalOrExpressionNode( tToken->location );

				mCurrentToken++;

				node->left = expression;
				node->right = LogicalOrExpression();

				return node;
			}
		}

		ExpressionNode *AssignmentExpression()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;

			if ( mCurrentToken->type != Token::Type::DEF )
			{
				ExpressionNode *expression = LogicalOrExpression();

				if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return expression;
				}
				else
				{
					if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::MEMBER_REFERENCE || expression->GetType() == Node::Type::INDEX_REFERENCE )
					{
						AssignmentExpressionNode *node = new AssignmentExpressionNode( tToken->location );

						mCurrentToken++;

						node->lhs = expression;
						node->rhs = AssignmentExpression();

						return node;
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_LHS, mCurrentToken->location );
						delete expression;

						return nullptr;
					}
				}
			}
			else
			{
				mCurrentToken++;

				if ( mCurrentToken->type == Token::Type::IDENTIFIER )
				{
					AssignmentExpressionNode *node = new AssignmentExpressionNode( tToken->location );

					node->lhs = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
					mCurrentToken++;

					node->rhs = FunctionLiteral();
					if ( node->rhs == nullptr )
					{
						delete node;

						return nullptr;
					}
					else
					{
						return node;
					}
				}
				else
				{
					mCurrentToken = tToken;

					return PrimaryExpression();
				}
			}
		}

		ExpressionNode *Expression()
		{
			return AssignmentExpression();
		}

		PublicNode *Public()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;

			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;

				if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return new PublicNode( tToken->location, name );
				}
				else
				{
					mCurrentToken++;

					return new PublicNode( tToken->location, name, Expression() );
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_VARIABLE_NAME, mCurrentToken->location );

				return nullptr;
			}
		}

		PrivateNode *Private()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;

			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;

				if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return new PrivateNode( tToken->location, name );
				}
				else
				{
					mCurrentToken++;

					return new PrivateNode( tToken->location, name, Expression() );
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_VARIABLE_NAME, mCurrentToken->location );

				return nullptr;
			}
		}

		ClassNode *Class()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;

			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				ClassNode *node = new ClassNode( tToken->location );

				node->name = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;

				if ( mCurrentToken->type == static_cast<Token::Type>( u':' ) )
				{
					mCurrentToken++;

					if ( mCurrentToken->type == Token::Type::IDENTIFIER )
					{
						node->base = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
						mCurrentToken++;
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_BASE_CLASS, mCurrentToken->location );
						delete node;

						return nullptr;
					}
				}

				node->block = Block();

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_CLASS_NAME, mCurrentToken->location );

				return nullptr;
			}
		}

		PackageNode *Package()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;

			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				PackageNode *node = new PackageNode( tToken->location );

				node->name = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;

				node->block = Block();

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_PACKAGE_NAME, mCurrentToken->location );

				return nullptr;
			}
		}

		ImportNode *Import()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;

			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				ImportNode *node = new ImportNode( tToken->location, new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier ) );
				mCurrentToken++;

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_PACKAGE, mCurrentToken->location );

				return nullptr;
			}
		}

		IfNode* If()
		{
			IfNode *node = new IfNode( mCurrentToken->location );
			ElseIfNode *tNode = new ElseIfNode( mCurrentToken->location );

			mCurrentToken++;

			tNode->condition = Expression();
			if ( mCurrentToken->type == Token::Type::THEN || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
			{
				mCurrentToken++;
			}
			tNode->block = Block();
			node->last = node->list.cbefore_begin();
			node->last = node->list.insert_after( node->last, tNode );

			if ( mCurrentToken->type == Token::Type::END )
			{
				mCurrentToken++;

				return node;
			}
			else if ( mCurrentToken->type == Token::Type::ELSE )
			{
				mCurrentToken++;

				node->block = Block();

				if ( mCurrentToken->type == Token::Type::END )
				{
					mCurrentToken++;

					return node;
				}
				else
				{
					BuildLog::Error( ErrorCode::EXPECTED_END, mCurrentToken->location );
					delete node;

					return nullptr;
				}
			}
			else if ( mCurrentToken->type == Token::Type::ELSEIF )
			{
				for (;;)
				{
					tNode = new ElseIfNode( mCurrentToken->location );
					mCurrentToken++;

					tNode->condition = Expression();
					tNode->block = Block();
					node->last = node->list.insert_after( node->last, tNode );

					if ( mCurrentToken->type == Token::Type::END )
					{
						mCurrentToken++;

						return node;
					}
					else if ( mCurrentToken->type == Token::Type::ELSE )
					{
						mCurrentToken++;

						node->block = Block();

						if ( mCurrentToken->type == Token::Type::END )
						{
							mCurrentToken++;

							return node;
						}
						else
						{
							BuildLog::Error( ErrorCode::EXPECTED_END, mCurrentToken->location );
							delete node;

							return nullptr;
						}
					}
					else if ( mCurrentToken->type != Token::Type::ELSEIF )
					{
						BuildLog::Error( ErrorCode::EXPECTED_END_ELSE_ELSEIF, mCurrentToken->location );
						delete node;

						return nullptr;
					}
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_END_ELSE_ELSEIF, mCurrentToken->location );
				delete node;

				return nullptr;
			}
		}

		CaseNode* Case()
		{
			CaseNode *node = new CaseNode( mCurrentToken->location );

			mCurrentToken++;

			node->value = Expression();

			if ( mCurrentToken->type == Token::Type::WHEN )
			{
				WhenNode *whenNode = new WhenNode( mCurrentToken->location );

				mCurrentToken++;

				whenNode->condition = Expression();
				if ( mCurrentToken->type == Token::Type::THEN || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
				{
					mCurrentToken++;
				}
				whenNode->block = Block();
				node->last = node->list.cbefore_begin();
				node->last = node->list.insert_after( node->last, whenNode );

				for (;;)
				{
					if ( mCurrentToken->type == Token::Type::WHEN )
					{
						whenNode = new WhenNode( mCurrentToken->location );
						mCurrentToken++;

						whenNode->condition = Expression();
						if ( mCurrentToken->type == Token::Type::THEN || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
						{
							mCurrentToken++;
						}
						whenNode->block = Block();
						node->last = node->list.insert_after( node->last, whenNode );
					}
					else if ( mCurrentToken->type == Token::Type::ELSE )
					{
						mCurrentToken++;

						node->block = Block();

						if ( mCurrentToken->type == Token::Type::END )
						{
							mCurrentToken++;

							return node;
						}
						else
						{
							BuildLog::Error( ErrorCode::EXPECTED_END, mCurrentToken->location );
							delete node;

							return nullptr;
						}
					}
					else if ( mCurrentToken->type == Token::Type::END )
					{
						mCurrentToken++;

						return node;
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_WHEN_ELSE_ELSEIF, mCurrentToken->location );
						delete node;

						return nullptr;
					}
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_WHEN, mCurrentToken->location );
				delete node;

				return nullptr;
			}
		}

		WhileNode* While()
		{
			WhileNode *node = new WhileNode( mCurrentToken->location );

			mCurrentToken++;

			node->condition = Expression();

			if ( mCurrentToken->type == Token::Type::DO || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
			{
				mCurrentToken++;
			}

			node->block = Block();

			if ( mCurrentToken->type == Token::Type::END )
			{
				mCurrentToken++;

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_END, mCurrentToken->location );
				delete node;

				return nullptr;
			}
		}

		IterationNode* For()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;

			mCurrentToken++;

			ExpressionNode *expression = Expression();

			if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
			{
				mCurrentToken++;

				ForNode *node = new ForNode( tToken->location );

				node->initializer = expression;
				node->condition = Expression();

				if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
				{
					mCurrentToken++;

					node->iterator = Expression();

					if ( mCurrentToken->type == Token::Type::DO || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
					{
						mCurrentToken++;
					}

					node->block = Block();

					if ( mCurrentToken->type == Token::Type::END )
					{
						mCurrentToken++;

						return node;
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_END, mCurrentToken->location );
						delete node;

						return nullptr;
					}
				}
				else
				{
					BuildLog::Error( ErrorCode::INCOMPLETE_FOR, mCurrentToken->location );
					delete node;

					return nullptr;
				}
			}
			else if ( mCurrentToken->type == Token::Type::IN )
			{
				mCurrentToken++;

				if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::MEMBER_REFERENCE || expression->GetType() == Node::Type::INDEX_REFERENCE )
				{
					return ForEach( expression, tToken );
				}
				else
				{
					BuildLog::Error( ErrorCode::EXPECTED_LHS, mCurrentToken->location );
					delete expression;

					return nullptr;
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_FOR_FOR_EACH, mCurrentToken->location );
				delete expression;

				return nullptr;
			}
		}

		ForEachNode *ForEach( ExpressionNode *expression, forward_list<Token>::const_iterator &token )
		{
			ForEachNode *node = new ForEachNode( token->location );

			node->variable = expression;

			node->collection = Expression();
			if ( node->collection->GetType() != Node::Type::IDENTIFIER && node->collection->GetType() != Node::Type::MEMBER_REFERENCE && node->collection->GetType() != Node::Type::INDEX_REFERENCE )
			{
				BuildLog::Error( ErrorCode::EXPECTED_LHS, mCurrentToken->location );
				delete node;

				return nullptr;
			}

			if ( mCurrentToken->type == Token::Type::DO || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
			{
				mCurrentToken++;
			}

			node->block = Block();

			if ( mCurrentToken->type == Token::Type::END )
			{
				mCurrentToken++;

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_END, mCurrentToken->location );
				delete node;

				return nullptr;
			}
		}

		RedoNode *Redo()
		{
			return new RedoNode( mCurrentToken++->location );
		}

		BreakNode *Break()
		{
			return new BreakNode( mCurrentToken++->location );
		}

		ReturnNode *Return()
		{
			forward_list<Token>::const_iterator tToken = mCurrentToken;

			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::END || mCurrentToken->type == Token::Type::ELSE || mCurrentToken->type == Token::Type::ELSEIF || mCurrentToken->type == Token::Type::WHEN || mCurrentToken == mLastToken )
			{
				return new ReturnNode( tToken->location );
			}
			else
			{
				return new ReturnNode( tToken->location, Expression() );
			}
		}
	};
};

#endif