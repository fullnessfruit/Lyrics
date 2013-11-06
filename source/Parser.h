#include <iostream>
#include <list>

#include "Token.h"
#include "Node.h"
#include "Literal.h"
#include "ByteCode.h"
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
			case Token::Type::INTEGER_LITERAL:
			case Token::Type::STRING_LITERAL:
			case Token::Type::BOOLEAN_LITERAL:
			case Token::Type::NIL_LITERAL:
			case Token::Type::REAL_LITERAL:
				break;

			case static_cast<Token::Type>( u'(' ):
				{
					Expression();

					if ( mCurrentToken->type != static_cast<Token::Type>( u')' ) )
					{
						// TODO:
					}
				}
				break;

			case static_cast<Token::Type>( u'[' ):
				if ( mCurrentToken->type != static_cast<Token::Type>( u']' ) )
				{
					forward_list<ByteCode> tObjectCode;
					forward_list<ByteCode>::const_iterator tLastObjectCode;

					// TODO: tLastObjectCode = tObjectCode.emplace_after( tLastObjectCode, ByteCode::Opcode::LOAD_WORD_IMMEDIATE, static_cast<short>( Register::TA0 ), Literal::Type::ARRAY );
					// TODO: tLastObjectCode = tObjectCode.emplace_after( tLastObjectCode, ByteCode::Opcode::CONSTRUCT_ARRAY, static_cast<short>( Register::A0 ) );

					// TODO: ExpressionList();

					if ( mCurrentToken->type != static_cast<Token::Type>( u']' ) )
					{
						// TODO:
					}
				}
				else
				{
					// TODO: lastObjectCode = objectCode.emplace_after( lastObjectCode, ByteCode::Opcode::LOAD_WORD_IMMEDIATE, static_cast<short>( Register::TA0 ), Literal::Type::ARRAY );
					// TODO: lastObjectCode = objectCode.emplace_after( lastObjectCode, ByteCode::Opcode::CONSTRUCT_ARRAY, static_cast<short>( Register::A0 ) );
				}
				break;

			case static_cast<Token::Type>( u'{' ):
				if ( mCurrentToken->type != static_cast<Token::Type>( u'}' ) )
				{
					forward_list<ByteCode> tObjectCode;
					forward_list<ByteCode>::const_iterator tLastObjectCode;

					// TODO: tLastObjectCode = tObjectCode.emplace_after( tLastObjectCode, ByteCode::Opcode::LOAD_WORD_IMMEDIATE, static_cast<short>( Register::TA0 ), Literal::Type::HASH );
					// TODO: tLastObjectCode = tObjectCode.emplace_after( tLastObjectCode, ByteCode::Opcode::CONSTRUCT_HASH, static_cast<short>( Register::A0 ) );

					// TODO: AssignmentList();

					if ( mCurrentToken->type != static_cast<Token::Type>( u'}' ) )
					{
						// TODO:
					}
				}
				else
				{
					// TODO: lastObjectCode = objectCode.emplace_after( lastObjectCode, ByteCode::Opcode::LOAD_WORD_IMMEDIATE, static_cast<short>( Register::TA0 ), Literal::Type::HASH );
					// TODO: lastObjectCode = objectCode.emplace_after( lastObjectCode, ByteCode::Opcode::CONSTRUCT_HASH, static_cast<short>( Register::A0 ) );
				}
				break;
			
			default:
				// TODO:
				break;
			}
		}

		ExpressionNode *PostfixExpression()
		{
			PrimaryExpression();
		}

		ExpressionNode *UnaryExpression()
		{
			if ( mCurrentToken->type != static_cast<Token::Type>( u'+' ) && mCurrentToken->type != static_cast<Token::Type>( u'-' ) && mCurrentToken->type != static_cast<Token::Type>( u'~' ) && mCurrentToken->type != static_cast<Token::Type>( u'!' ) )
			{
				return PostfixExpression();
			}
			else
			{
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