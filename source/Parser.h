#include <iostream>
#include <forward_list>

#include "Tokenizer.h"
#include "Token.h"
#include "Node.h"
#include "Literal.h"
#include "Location.h"
#include "WarningCode.h"
#include "ErrorCode.h"

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
			Tokenizer tokenizer( fileName );
			forward_list<Token> token;

			tokenizer.Tokenize( token );

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

			node->last = node->list.cend();
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
						// TODO: Expected ).
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
						node->last = node->list.cend();
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
								// TODO: error: Expected , or ].
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
						node->last = node->list.cend();
						for (;;)
						{
							ExpressionNode *left = Expression();
							if ( mCurrentToken->type != static_cast<Token::Type>( u':' ) )
							{
								// TODO: Expected :.
								delete node;
								delete left;

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
								// TODO: error: Expected , or }.
								delete node;

								return nullptr;
							}
						}
					}
					
					mCurrentToken++;

					return node;
				}
			
			default:
				// TODO: wrong token in primary expression.
				return nullptr;
			}
		}

		ExpressionNode *PostfixExpression()
		{
			ExpressionNode *temp = PrimaryExpression();

			if ( mCurrentToken->type == static_cast<Token::Type>( u'[' ) )
			{
				mCurrentToken++;

				PostfixExpressionNode *node = new PostfixExpressionNode( mCurrentToken->location );

				node->expression = temp;
				node->postfix = new IndexNode( mCurrentToken->location, Expression() );

				if ( mCurrentToken->type == static_cast<Token::Type>( u']' ) )
				{
					mCurrentToken++;

					return node;
				}
				else
				{
					// TODO: error: Expected ].
					delete temp;
					delete node;

					return nullptr;
				}
			}
			else if ( mCurrentToken->type == static_cast<Token::Type>( u'(' ) )
			{
				mCurrentToken++;

				PostfixExpressionNode *node = new PostfixExpressionNode( mCurrentToken->location );

				node->expression = temp;
				node->postfix = new CallNode( mCurrentToken->location );

				if ( mCurrentToken->type != static_cast<Token::Type>( u')' ) )
				{
					static_cast<CallNode *>( node->postfix )->last = static_cast<CallNode *>( node->postfix )->list.cend();
					for (;;)
					{
						static_cast<CallNode *>( node->postfix )->last = static_cast<CallNode *>( node->postfix )->list.insert_after( static_cast<CallNode *>( node->postfix )->last, Expression() );

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
							// TODO: error: Expected , or ).
							delete temp;
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
					PostfixExpressionNode *node = new PostfixExpressionNode( mCurrentToken->location );

					node->expression = temp;
					node->postfix = new MemberNode( mCurrentToken->location, new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier ) );
					mCurrentToken++;

					return node;
				}
				else
				{
					// TODO: error: Expected identifier.
					delete temp;

					return nullptr;
				}
			}
			else
			{
				return temp;
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
				mCurrentToken++;

				UnaryExpressionNode *node = new UnaryExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				node->expression = UnaryExpression();

				return node;
			}
		}

		ExpressionNode *MultiplicativeExpression()
		{
			ExpressionNode *temp = UnaryExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'*' ) && mCurrentToken->type != static_cast<Token::Type>( u'/' ) && mCurrentToken->type != static_cast<Token::Type>( u'%' ) )
			{
				return temp;
			}
			else
			{
				mCurrentToken++;

				MultiplicativeExpressionNode *node = new MultiplicativeExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				node->left = temp;
				node->right = MultiplicativeExpression();

				return node;
			}
		}

		ExpressionNode *AdditiveExpression()
		{
			ExpressionNode *temp = MultiplicativeExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'+' ) && mCurrentToken->type != static_cast<Token::Type>( u'-' ) )
			{
				return temp;
			}
			else
			{
				mCurrentToken++;

				AdditiveExpressionNode *node = new AdditiveExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				node->left = temp;
				node->right = AdditiveExpression();

				return node;
			}
		}

		ExpressionNode *ShiftExpression()
		{
			ExpressionNode *temp = AdditiveExpression();

			if ( mCurrentToken->type != Token::Type::SHIFT_LEFT && mCurrentToken->type != Token::Type::SHIFT_RIGHT )
			{
				return temp;
			}
			else
			{
				mCurrentToken++;

				ShiftExpressionNode *node = new ShiftExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				node->left = temp;
				node->right = ShiftExpression();

				return node;
			}
		}

		ExpressionNode *AndExpression()
		{
			ExpressionNode *temp = ShiftExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'&' ) )
			{
				return temp;
			}
			else
			{
				mCurrentToken++;

				AndExpressionNode *node = new AndExpressionNode( mCurrentToken->location );

				node->left = temp;
				node->right = AndExpression();

				return node;
			}
		}

		ExpressionNode *OrExpression()
		{
			ExpressionNode *temp = AndExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'|' ) && mCurrentToken->type != static_cast<Token::Type>( u'^' ) )
			{
				return temp;
			}
			else
			{
				mCurrentToken++;

				OrExpressionNode *node = new OrExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				node->left = temp;
				node->right = OrExpression();

				return node;
			}
		}

		ExpressionNode *RelationalExpression()
		{
			ExpressionNode *temp = OrExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'<' ) && mCurrentToken->type != static_cast<Token::Type>( u'>' ) && mCurrentToken->type != Token::Type::LESS_THAN_OR_EQUAL && mCurrentToken->type != Token::Type::GREATER_THAN_OR_EQUAL )
			{
				return temp;
			}
			else
			{
				mCurrentToken++;

				RelationalExpressionNode *node = new RelationalExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				node->left = temp;
				node->right = RelationalExpression();

				return node;
			}
		}

		ExpressionNode *EqualityExpression()
		{
			ExpressionNode *temp = RelationalExpression();

			if ( mCurrentToken->type != Token::Type::EQUAL && mCurrentToken->type != Token::Type::NOT_EQUAL )
			{
				return temp;
			}
			else
			{
				mCurrentToken++;

				EqualityExpressionNode *node = new EqualityExpressionNode( mCurrentToken->location );

				node->op = mCurrentToken->type;
				node->left = temp;
				node->right = EqualityExpression();

				return node;
			}
		}

		ExpressionNode *LogicalAndExpression()
		{
			ExpressionNode *temp = EqualityExpression();

			if ( mCurrentToken->type != Token::Type::AND )
			{
				return temp;
			}
			else
			{
				mCurrentToken++;

				LogicalAndExpressionNode *node = new LogicalAndExpressionNode( mCurrentToken->location );

				node->left = temp;
				node->right = LogicalAndExpression();

				return node;
			}
		}

		ExpressionNode *LogicalOrExpression()
		{
			ExpressionNode *temp = LogicalAndExpression();

			if ( mCurrentToken->type != Token::Type::OR )
			{
				return temp;
			}
			else
			{
				mCurrentToken++;

				LogicalOrExpressionNode *node = new LogicalOrExpressionNode( mCurrentToken->location );

				node->left = temp;
				node->right = LogicalOrExpression();

				return node;
			}
		}

		ExpressionNode *AssignmentExpression()
		{
			ExpressionNode *temp = LogicalOrExpression();

			if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
			{
				return temp;
			}
			else
			{
				if ( temp->GetType() == Node::Type::IDENTIFIER || temp->GetType() == Node::Type::POSTFIX_EXPRESSION )
				{
					mCurrentToken++;

					AssignmentExpressionNode *node = new AssignmentExpressionNode( mCurrentToken->location );

					node->lhs = temp;
					node->rhs = AssignmentExpression();

					return node;
				}
				else
				{
					// TODO: error: temp must be postfix expression.
					delete temp;

					return nullptr;
				}
			}
		}

		ExpressionNode *Expression()
		{
			return AssignmentExpression();
		}

		ProcedureNode *Procedure()
		{
			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				ProcedureNode *node = new ProcedureNode( mCurrentToken->location );

				node->identifier = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;

				if ( mCurrentToken->type != static_cast<Token::Type>( u'(' ) )
				{
					mCurrentToken++;

					if ( mCurrentToken->type != static_cast<Token::Type>( u')' ) || mCurrentToken->type == Token::Type::OUT )
					{
						node->lastParameter = node->parameter.cend();
						for (;;)
						{
							if ( mCurrentToken->type != Token::Type::IDENTIFIER )
							{
								// TODO: Expected identifier.
								delete node;

								return nullptr;
							}
							IdentifierNode *identifier = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
							mCurrentToken++;

							ParameterNode *parameter;
							if ( mCurrentToken->type != static_cast<Token::Type>( u'=' ) )
							{
								parameter = new ParameterNode( mCurrentToken->location, identifier );
							}
							else
							{
								mCurrentToken++;

								parameter = new ParameterNode( mCurrentToken->location, identifier, Expression() );
							}

							node->lastParameter = node->parameter.insert_after( node->lastParameter, parameter );

							if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
							{
								mCurrentToken++;
							}
							else if ( mCurrentToken->type == static_cast<Token::Type>( u')' ) || mCurrentToken->type == Token::Type::OUT )
							{
								break;
							}
							else
							{
								// TODO: error: Expected , or ) or out.
								delete node;

								return nullptr;
							}
						}
					}

					if ( mCurrentToken->type == Token::Type::OUT )
					{
						mCurrentToken++;

						if ( mCurrentToken->type != static_cast<Token::Type>( u')' ) )
						{
							node->lastOutParameter = node->outParameter.cend();
							for (;;)
							{
								if ( mCurrentToken->type != Token::Type::IDENTIFIER )
								{
									// TODO: Expected identifier.
									delete node;

									return nullptr;
								}
								node->lastOutParameter = node->outParameter.insert_after( node->lastOutParameter, new OutParameterNode( mCurrentToken->location, new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier ) ) );
								mCurrentToken++;

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
									// TODO: error: Expected , or ).
									delete node;

									return nullptr;
								}
							}
						}
					}

					mCurrentToken++;

					node->block = Block();

					return node;
				}
				else
				{
					// TODO: Expected (.
					delete node;

					return nullptr;
				}
			}
			else
			{
				// TODO: Expected Identifier.
				return nullptr;
			}
		}

		ClassNode *Class()
		{
			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				ClassNode *node = new ClassNode( mCurrentToken->location );

				node->identifier = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
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
						// TODO: Expected Identifier.
						delete node;

						return nullptr;
					}
				}

				node->block = Block();

				return node;
			}
			else
			{
				// TODO: Expected Identifier.
				return nullptr;
			}
		}

		PackageNode *Package()
		{
			mCurrentToken++;

			if ( mCurrentToken->type == Token::Type::IDENTIFIER )
			{
				PackageNode *node = new PackageNode( mCurrentToken->location );

				node->identifier = new IdentifierNode( mCurrentToken->location, mCurrentToken->value.identifier );
				mCurrentToken++;

				node->block = Block();

				return node;
			}
			else
			{
				// TODO: Expected Identifier.
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
				// TODO: Expected Identifier.
				return nullptr;
			}
		}

		IfNode* If()
		{
			mCurrentToken++;

			IfNode *node = new IfNode( mCurrentToken->location );
			ElseIfNode *tNode = new ElseIfNode( mCurrentToken->location );

			tNode->expression = Expression();
			if ( mCurrentToken->type == Token::Type::THEN || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
			{
				mCurrentToken++;
			}
			tNode->block = Block();
			node->last = node->list.cend();
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
					// TODO: Expected end or else or elseif
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
					tNode->expression = Expression();
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
							// TODO: Expected end.
							delete node;

							return nullptr;
						}
					}
					else if ( mCurrentToken->type != Token::Type::ELSEIF )
					{
						// TODO: Expected end or else or elseif.
						delete node;

						return nullptr;
					}
				}
			}
			else
			{
				// TODO: Expected end or else or elseif.
				delete node;

				return nullptr;
			}
		}

		CaseNode* Case()
		{
			mCurrentToken++;

			CaseNode *node = new CaseNode( mCurrentToken->location );

			node->expression = Expression();

			if ( mCurrentToken->type == Token::Type::WHEN )
			{
				mCurrentToken++;

				WhenNode *tNode = new WhenNode( mCurrentToken->location );
				tNode->expression = Expression();
				if ( mCurrentToken->type == Token::Type::THEN || mCurrentToken->type == static_cast<Token::Type>( u':' ) )
				{
					mCurrentToken++;
				}
				tNode->block = Block();
				node->last = node->list.cend();
				node->last = node->list.insert_after( node->last, tNode );

				for (;;)
				{
					if ( mCurrentToken->type == Token::Type::WHEN )
					{
						mCurrentToken++;

						tNode = new WhenNode( mCurrentToken->location );
						tNode->expression = Expression();
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
							// TODO: Expected end.
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
						// TODO: Expected when or else or end.
						delete node;

						return nullptr;
					}
				}
			}
			else
			{
				// TODO: Expected when.
				delete node;

				return nullptr;
			}
		}

		WhileNode* While()
		{
			mCurrentToken++;

			WhileNode *node = new WhileNode( mCurrentToken->location );

			node->expression = Expression();

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
				// TODO: Expected end.
				delete node;

				return nullptr;
			}
		}

		IterationNode* For()
		{
			mCurrentToken++;

			ExpressionNode *tExpression = Expression();

			if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
			{
				mCurrentToken++;

				ForNode *node = new ForNode( mCurrentToken->location );

				node->expression1 = tExpression;
				node->expression2 = Expression();

				if ( mCurrentToken->type == static_cast<Token::Type>( u',' ) )
				{
					mCurrentToken++;

					node->expression3 = Expression();

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
						// TODO: Expected end.
						delete node;

						return nullptr;
					}
				}
				else
				{
					// TODO: Expected , or in.
					delete node;

					return nullptr;
				}
			}
			else if ( mCurrentToken->type == Token::Type::IN )
			{
				mCurrentToken++;

				if ( tExpression->GetType() == Node::Type::IDENTIFIER || tExpression->GetType() == Node::Type::POSTFIX_EXPRESSION )
				{
					return ForEach( tExpression );
				}
				else
				{
					// TODO: Expected lhs.
					delete tExpression;

					return nullptr;
				}
			}
			else
			{
				// TODO: Expected , or in.
				delete tExpression;

				return nullptr;
			}
		}

		ForEachNode *ForEach( ExpressionNode *expression )
		{
			ForEachNode *node = new ForEachNode( mCurrentToken->location );

			node->expression1 = expression;

			node->expression2 = Expression();
			if ( node->expression2->GetType() != Node::Type::IDENTIFIER && node->expression2->GetType() != Node::Type::POSTFIX_EXPRESSION )
			{
				// TODO: Expected lhs.
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
				// TODO: Expected end.
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

			return new ReturnNode( mCurrentToken->location );
		}

		static void Warning( const WarningCode warningCode, const Location location )
		{
			using std::cout;
			using std::cerr;

			constexpr char WARNING[] = "Warning";

			switch ( warningCode )
			{
			case WarningCode::UNKNOWN_ESCAPE_SEQUENCE:
				cout << WARNING << ' ' << static_cast<unsigned int>( warningCode ) << ' ' << location << ' ' << "Unknown escape sequence.";
				break;

			default:
				cerr << WARNING << ' ' << static_cast<unsigned int>( warningCode );
				break;
			}
		}

		static void Error( const ErrorCode errorCode, const Location location )
		{
			using std::cout;
			using std::cerr;

			constexpr char ERROR[] = "Error";

			switch ( errorCode )
			{
			case ErrorCode::WRONG_CHARACTER:
				cout << ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Wrong character.";
				break;

			default:
				cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode );
				break;
			}
		}
	};
};

#endif