#include <iostream>
#include <list>

#include "Token.h"
#include "Node.h"
#include "Literal.h"
#include "WarningCode.h"
#include "ErrorCode.h"
#include "Location.h"

#ifndef PARSER
#define PARSER

namespace lyrics
{
	using std::list;

	class Parser
	{
	public:
		void Parse( const list<Token> &token )
		{
			mCurrentToken = token.cbegin();
			mLastToken = token.cend();

			BlockNode *block = Block();
		}
	
	private:
		list<Token>::const_iterator mCurrentToken;
		list<Token>::const_iterator mLastToken;

		BlockNode *Block()
		{
			BlockNode *node = new BlockNode();

			if ( mCurrentToken != mLastToken )
			{
				node->last = node->list.insert_after( node->last, Statement() );
			}

			return node;
		}

		StatementNode *Statement()
		{
			StatementNode *node;

			switch ( mCurrentToken->type )
			{
			case Token::Type::IF:
			case Token::Type::CASE:
				node = Selection();
				break;

			case Token::Type::FOR:
			case Token::Type::WHILE:
				node = Iteration();
				break;

			case Token::Type::PROC:
				node = Procedure();
				break;

			case Token::Type::BREAK:
			case Token::Type::RETURN:
			case Token::Type::REDO:
				node = Jump();
				break;

			case Token::Type::CLASS:
				node = Class();
				break;

			case Token::Type::PACKAGE:
				node = Package();
				break;

			case Token::Type::IMPORT:
				node = Import();
				break;
			
			default:
				node = Expression();
				break;
			}

			return node;
		}

		ExpressionNode *PrimaryExpression()
		{
			switch ( mCurrentToken->type )
			{
			case Token::Type::IDENTIFIER:
				return new IdentifierNode( ( mCurrentToken++ )->value.identifier );

			case Token::Type::INTEGER_LITERAL:
				{
					LiteralNode *node = new LiteralNode();

					node->literal.type = Literal::Type::INTEGER;
					node->literal.value.integer = mCurrentToken->value.integer;

					mCurrentToken++;

					return node;
				}

			case Token::Type::STRING_LITERAL:
				{
					LiteralNode *node = new LiteralNode();

					node->literal.type = Literal::Type::STRING;
					node->literal.value.string = mCurrentToken->value.string;

					mCurrentToken++;

					return node;
				}

			case Token::Type::BOOLEAN_LITERAL:
				{
					LiteralNode *node = new LiteralNode();

					node->literal.type = Literal::Type::BOOLEAN;
					node->literal.value.boolean = mCurrentToken->value.boolean;

					mCurrentToken++;

					return node;
				}

			case Token::Type::NIL_LITERAL:
				{
					LiteralNode *node = new LiteralNode();

					node->literal.type = Literal::Type::NIL;

					mCurrentToken++;

					return node;
				}

			case Token::Type::REAL_LITERAL:
				{
					LiteralNode *node = new LiteralNode();

					node->literal.type = Literal::Type::REAL;
					node->literal.value.real = mCurrentToken->value.real;

					mCurrentToken++;

					return node;
				}

			case static_cast<Token::Type>( u'(' ):
				{
					mCurrentToken++;

					ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode( Expression() );

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
				if ( mCurrentToken->type != static_cast<Token::Type>( u']' ) )
				{
					if ( mCurrentToken->type != static_cast<Token::Type>( u']' ) )
					{
						// TODO:
					}
				}
				else
				{
				}
				break;

			case static_cast<Token::Type>( u'{' ):
				if ( mCurrentToken->type != static_cast<Token::Type>( u'}' ) )
				{
					if ( mCurrentToken->type != static_cast<Token::Type>( u'}' ) )
					{
						// TODO:
					}
				}
				else
				{
				}
				break;
			
			default:
				// TODO:
				break;
			}
		}

		ExpressionNode *PostfixExpression()
		{
			ExpressionNode *temp = PrimaryExpression();

			if ( mCurrentToken->type == static_cast<Token::Type>( u'[' ) )
			{
				mCurrentToken++;

				PostfixExpressionNode *node = new PostfixExpressionNode();

				node->expression = temp;
				node->postfix = new IndexNode();
				static_cast<IndexNode *>( node->postfix )->expression = Expression();

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

				PostfixExpressionNode *node = new PostfixExpressionNode();

				node->expression = temp;
				node->postfix = new CallNode();

				if ( mCurrentToken->type != static_cast<Token::Type>( u')' ) )
				{
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
					PostfixExpressionNode *node = new PostfixExpressionNode();

					node->expression = temp;
					node->postfix = new MemberNode();
					static_cast<MemberNode *>( node->postfix )->identifier = new IdentifierNode( mCurrentToken->value.identifier );
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

				UnaryExpressionNode *node = new UnaryExpressionNode();

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

				MultiplicativeExpressionNode *node = new MultiplicativeExpressionNode();

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

				AdditiveExpressionNode *node = new AdditiveExpressionNode();

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

				ShiftExpressionNode *node = new ShiftExpressionNode();

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

				AndExpressionNode *node = new AndExpressionNode();

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

				OrExpressionNode *node = new OrExpressionNode();

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

				RelationalExpressionNode *node = new RelationalExpressionNode();

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

				EqualityExpressionNode *node = new EqualityExpressionNode();

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

				LogicalAndExpressionNode *node = new LogicalAndExpressionNode();

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

				LogicalOrExpressionNode *node = new LogicalOrExpressionNode();

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

					AssignmentExpressionNode *node = new AssignmentExpressionNode();

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
		}

		ClassNode *Class()
		{
		}

		PackageNode *Package()
		{
		}

		ImportNode *Import()
		{
		}

		SelectionNode *Selection()
		{
		}

		IterationNode *Iteration()
		{
		}

		JumpNode *Jump()
		{
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