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

			case Token::Type::PROC:
				return Procedure();

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
					LiteralNode *node = new LiteralNode( mCurrentToken->location );

					node->literal.type = Literal::Type::INTEGER;
					node->literal.value.integer = mCurrentToken->value.integer;

					mCurrentToken++;

					return node;
				}

			case Token::Type::STRING_LITERAL:
				{
					LiteralNode *node = new LiteralNode( mCurrentToken->location );

					node->literal.type = Literal::Type::STRING;
					node->literal.value.string = mCurrentToken->value.string;

					mCurrentToken++;

					return node;
				}

			case Token::Type::BOOLEAN_LITERAL:
				{
					LiteralNode *node = new LiteralNode( mCurrentToken->location );

					node->literal.type = Literal::Type::BOOLEAN;
					node->literal.value.boolean = mCurrentToken->value.boolean;

					mCurrentToken++;

					return node;
				}

			case Token::Type::NIL_LITERAL:
				{
					LiteralNode *node = new LiteralNode( mCurrentToken->location );

					node->literal.type = Literal::Type::NIL;

					mCurrentToken++;

					return node;
				}

			case Token::Type::REAL_LITERAL:
				{
					LiteralNode *node = new LiteralNode( mCurrentToken->location );

					node->literal.type = Literal::Type::REAL;
					node->literal.value.real = mCurrentToken->value.real;

					mCurrentToken++;

					return node;
				}

			case static_cast<Token::Type>( u'(' ):
				{
					mCurrentToken++;

					ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode( mCurrentToken->location, Expression() );

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
					mCurrentToken++;

					ArrayNode *node = new ArrayNode( mCurrentToken->location );

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
					mCurrentToken++;

					HashNode *node = new HashNode( mCurrentToken->location );

					if ( mCurrentToken->type != static_cast<Token::Type>( u'}' ) )
					{
						node->last = node->list.cbefore_begin();
						for (;;)
						{
							ExpressionNode *left = Expression();
							if ( mCurrentToken->type != static_cast<Token::Type>( u':' ) )
							{
								BuildLog::Error( ErrorCode::EXPECTED_HASH_PAIR, mCurrentToken->location );
								delete left;
								delete node;

								return nullptr;
							}
							mCurrentToken++;
							ExpressionNode *right = Expression();

							PairNode *pair = new PairNode( mCurrentToken->location, left, right );

							node->last = node->list.insert_after( node->last, pair );

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
			
			default:
				BuildLog::Error( ErrorCode::EXPECTED_PRIMARY_EXPRESSION, mCurrentToken->location );
				return nullptr;
			}
		}

		ExpressionNode *PostfixExpression()
		{
			ExpressionNode *expression = PrimaryExpression();

			if ( mCurrentToken->type == static_cast<Token::Type>( u'[' ) )
			{
				mCurrentToken++;

				IndexReferenceNode *node = new IndexReferenceNode( mCurrentToken->location, expression, Expression() );

				if ( mCurrentToken->type == static_cast<Token::Type>( u']' ) )
				{
					mCurrentToken++;

					return node;
				}
				else
				{
					BuildLog::Error( ErrorCode::EXPECTED_INDEX, mCurrentToken->location );
					delete expression;
					delete node;

					return nullptr;
				}
			}
			else if ( mCurrentToken->type == static_cast<Token::Type>( u'(' ) )
			{
				mCurrentToken++;

				ProcedureCallNode *node = new ProcedureCallNode( mCurrentToken->location, expression );

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
							BuildLog::Error( ErrorCode::EXPECTED_PROCEDURE_CALL, mCurrentToken->location );
							delete expression;
							delete node;

							return nullptr;
						}
					}
				}
				mCurrentToken++;

				return node;
			}
			else if ( mCurrentToken->type == static_cast<Token::Type>( u'.' ) )
			{
				mCurrentToken++;

				if ( mCurrentToken->type == Token::Type::IDENTIFIER )
				{
					MemberReferenceNode *node = new MemberReferenceNode( mCurrentToken->location, expression, new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier ) );

					mCurrentToken++;

					return node;
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
			ExpressionNode *expression = UnaryExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'*' ) && mCurrentToken->type != static_cast<Token::Type>( u'/' ) && mCurrentToken->type != static_cast<Token::Type>( u'%' ) )
			{
				return expression;
			}
			else
			{

				MultiplicativeExpressionNode *node = new MultiplicativeExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = MultiplicativeExpression();

				return node;
			}
		}

		ExpressionNode *AdditiveExpression()
		{
			ExpressionNode *expression = MultiplicativeExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'+' ) && mCurrentToken->type != static_cast<Token::Type>( u'-' ) )
			{
				return expression;
			}
			else
			{
				AdditiveExpressionNode *node = new AdditiveExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = AdditiveExpression();

				return node;
			}
		}

		ExpressionNode *ShiftExpression()
		{
			ExpressionNode *expression = AdditiveExpression();

			if ( mCurrentToken->type != Token::Type::SHIFT_LEFT && mCurrentToken->type != Token::Type::SHIFT_RIGHT )
			{
				return expression;
			}
			else
			{
				ShiftExpressionNode *node = new ShiftExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = ShiftExpression();

				return node;
			}
		}

		ExpressionNode *AndExpression()
		{
			ExpressionNode *expression = ShiftExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'&' ) )
			{
				return expression;
			}
			else
			{
				AndExpressionNode *node = new AndExpressionNode( mCurrentToken->location );

				mCurrentToken++;

				node->left = expression;
				node->right = AndExpression();

				return node;
			}
		}

		ExpressionNode *OrExpression()
		{
			ExpressionNode *expression = AndExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'|' ) && mCurrentToken->type != static_cast<Token::Type>( u'^' ) )
			{
				return expression;
			}
			else
			{
				OrExpressionNode *node = new OrExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = OrExpression();

				return node;
			}
		}

		ExpressionNode *RelationalExpression()
		{
			ExpressionNode *expression = OrExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'<' ) && mCurrentToken->type != static_cast<Token::Type>( u'>' ) && mCurrentToken->type != Token::Type::LESS_THAN_OR_EQUAL && mCurrentToken->type != Token::Type::GREATER_THAN_OR_EQUAL )
			{
				return expression;
			}
			else
			{
				RelationalExpressionNode *node = new RelationalExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = RelationalExpression();

				return node;
			}
		}

		ExpressionNode *EqualityExpression()
		{
			ExpressionNode *expression = RelationalExpression();

			if ( mCurrentToken->type != Token::Type::EQUAL && mCurrentToken->type != Token::Type::NOT_EQUAL )
			{
				return expression;
			}
			else
			{
				EqualityExpressionNode *node = new EqualityExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				mCurrentToken++;

				node->left = expression;
				node->right = EqualityExpression();

				return node;
			}
		}

		ExpressionNode *LogicalAndExpression()
		{
			ExpressionNode *expression = EqualityExpression();

			if ( mCurrentToken->type != Token::Type::AND )
			{
				return expression;
			}
			else
			{
				LogicalAndExpressionNode *node = new LogicalAndExpressionNode( mCurrentToken->location );

				mCurrentToken++;

				node->left = expression;
				node->right = LogicalAndExpression();

				return node;
			}
		}

		ExpressionNode *LogicalOrExpression()
		{
			ExpressionNode *expression = LogicalAndExpression();

			if ( mCurrentToken->type != Token::Type::OR )
			{
				return expression;
			}
			else
			{
				LogicalOrExpressionNode *node = new LogicalOrExpressionNode( mCurrentToken->location );

				mCurrentToken++;

				node->left = expression;
				node->right = LogicalOrExpression();

				return node;
			}
		}

		ExpressionNode *AssignmentExpression()
		{
			ExpressionNode *expression = LogicalOrExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
			{
				return expression;
			}
			else
			{
				if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::MEMBER_REFERENCE  || expression->GetType() == Node::Type::INDEX_REFERENCE )
				{
					AssignmentExpressionNode *node = new AssignmentExpressionNode( mCurrentToken->location );

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

		ExpressionNode *Expression()
		{
			return AssignmentExpression();
		}

		PublicNode *Public()
		{
			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;

				if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return new PublicNode( mCurrentToken->location, name );
				}
				else
				{
					mCurrentToken++;

					return new PublicNode( mCurrentToken->location, name, Expression() );
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
			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;

				if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return new PrivateNode( mCurrentToken->location, name );
				}
				else
				{
					mCurrentToken++;

					return new PrivateNode( mCurrentToken->location, name, Expression() );
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_VARIABLE_NAME, mCurrentToken->location );

				return nullptr;
			}
		}

		ProcedureNode *Procedure()
		{
			mCurrentToken++;

			ProcedureNode *node = new ProcedureNode( mCurrentToken->location );

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				node->name = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;
			}

			if ( mCurrentToken->type != static_cast<Token::Type>( u'(' ) )
			{
				mCurrentToken++;

				if ( mCurrentToken->type != static_cast<Token::Type>( u')' ) )
				{
					ParameterNode *parameter;
					bool isValueParameter;
					IdentifierNode *name;

					node->last = node->list.cbefore_begin();
					for (;;)
					{
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
								parameter = new ValueParameterNode( mCurrentToken->location, name );
							}
							else
							{
								parameter = new OutputParameterNode( mCurrentToken->location, name );
							}
						}
						else
						{
							mCurrentToken++;

							if ( isValueParameter )
							{
								parameter = new ValueParameterNode( mCurrentToken->location, name, Expression() );
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
							BuildLog::Error( ErrorCode::INCOMPLETE_PROCEDURE, mCurrentToken->location );
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
				delete node;

				return nullptr;
			}
		}

		ClassNode *Class()
		{
			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				ClassNode *node = new ClassNode( mCurrentToken->location );

				node->name = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;

				if ( mCurrentToken->type == Token::Type::EXTENDS )
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
			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				PackageNode *node = new PackageNode( mCurrentToken->location );

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
			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				ImportNode *node = new ImportNode( mCurrentToken->location, new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier ) );
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
			mCurrentToken++;

			IfNode *node = new IfNode( mCurrentToken->location );
			ElseIfNode *tNode = new ElseIfNode( mCurrentToken->location );

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
					mCurrentToken++;

					tNode = new ElseIfNode( mCurrentToken->location );
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
			mCurrentToken++;

			CaseNode *node = new CaseNode( mCurrentToken->location );

			node->value = Expression();

			if ( mCurrentToken->type == Token::Type::WHEN )
			{
				mCurrentToken++;

				WhenNode *tNode = new WhenNode( mCurrentToken->location );
				tNode->condition = Expression();
				if ( mCurrentToken->type == Token::Type::THEN || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
				{
					mCurrentToken++;
				}
				tNode->block = Block();
				node->last = node->list.cbefore_begin();
				node->last = node->list.insert_after( node->last, tNode );

				for (;;)
				{
					if ( mCurrentToken->type == Token::Type::WHEN )
					{
						mCurrentToken++;

						tNode = new WhenNode( mCurrentToken->location );
						tNode->condition = Expression();
						if ( mCurrentToken->type == Token::Type::THEN || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
						{
							mCurrentToken++;
						}
						tNode->block = Block();
						node->last = node->list.insert_after( node->last, tNode );
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
			mCurrentToken++;

			WhileNode *node = new WhileNode( mCurrentToken->location );

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
			mCurrentToken++;

			ExpressionNode *expression = Expression();

			if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
			{
				mCurrentToken++;

				ForNode *node = new ForNode( mCurrentToken->location );

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
					return ForEach( expression );
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

		ForEachNode *ForEach( ExpressionNode *expression )
		{
			ForEachNode *node = new ForEachNode( mCurrentToken->location );

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
			mCurrentToken++;

			return new RedoNode( mCurrentToken->location );
		}

		BreakNode *Break()
		{
			mCurrentToken++;

			return new BreakNode( mCurrentToken->location );
		}

		ReturnNode *Return()
		{
			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::END || mCurrentToken->type == Token::Type::ELSE || mCurrentToken->type == Token::Type::ELSEIF || mCurrentToken->type == Token::Type::WHEN || mCurrentToken == mLastToken )
			{
				return new ReturnNode( mCurrentToken->location );
			}
			else
			{
				return new ReturnNode( mCurrentToken->location, Expression() );
			}
		}
	};
};

#endif