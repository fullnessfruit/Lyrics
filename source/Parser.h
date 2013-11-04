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
			mCurrentToken = token.begin();

			BlockNode *block = Block();
		}
	
	private:
		list<Token>::const_iterator mCurrentToken;

		static void Warning( const WarningCode warningCode, const Location location )
		{
			using std::cerr;

			constexpr char WARNING[] = "Warning";

			switch ( warningCode )
			{
			case WarningCode::UNKNOWN_ESCAPE_SEQUENCE:
				cerr << WARNING << ' ' << static_cast<unsigned int>( warningCode ) << ' ' << location << ' ' << "Unknown escape sequence.";
				break;

			default:
				cerr << WARNING << ' ' << static_cast<unsigned int>( warningCode );
				break;
			}
		}

		static void Error( const ErrorCode errorCode, const Location location )
		{
			using std::cerr;

			constexpr char ERROR[] = "Error";

			switch ( errorCode )
			{
			case ErrorCode::WRONG_CHARACTER:
				cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode ) << ' ' << location << ' ' << "Wrong character.";
				break;

			default:
				cerr << ERROR << ' ' << static_cast<unsigned int>( errorCode );
				break;
			}
		}

		BlockNode *Block()
		{
			if ( mCurrentToken->type != Token::Type::END_OF_FILE )
			{
				Statement();
			}
		}

		StatementNode *Statement()
		{
			switch ( mCurrentToken->type )
			{
			case Token::Type::IF:
			case Token::Type::CASE:
				Selection();
				break;

			case Token::Type::FOR:
			case Token::Type::WHILE:
				Iteration();
				break;

			case Token::Type::PROC:
				Procedure();
				break;

			case Token::Type::BREAK:
			case Token::Type::RETURN:
			case Token::Type::REDO:
				Jump();
				break;

			case Token::Type::CLASS:
				Class();
				break;

			case Token::Type::PACKAGE:
				Package();
				break;

			case Token::Type::IMPORT:
				Import();
				break;
			
			default:
				Expression();
				break;
			}
		}

		PrimaryExpressionNode *PrimaryExpression()
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

		PostfixExpressionNode *PostfixExpression()
		{
			PrimaryExpression();
		}

		UnaryExpressionNode *UnaryExpression()
		{
			PostfixExpression();
		}

		MultiplicativeExpressionNode *MultiplicativeExpression()
		{
			UnaryExpression();
		}

		AdditiveExpressionNode *AdditiveExpression()
		{
			MultiplicativeExpression();
		}

		ShiftExpressionNode *ShiftExpression()
		{
			AdditiveExpression();
		}

		AndExpressionNode *AndExpression()
		{
			ShiftExpression();
		}

		OrExpressionNode *OrExpression()
		{
			AndExpression();
		}

		RelationalExpressionNode *RelationalExpression()
		{
			OrExpression();
		}

		EqualityExpressionNode *EqualityExpression()
		{
			RelationalExpression();
		}

		LogicalAndExpressionNode *LogicalAndExpression()
		{
			EqualityExpression();
		}

		LogicalOrExpressionNode *LogicalOrExpression()
		{
			LogicalAndExpression();
		}

		ExpressionNode *Expression()
		{
			LogicalOrExpression();
		}

		AssignmentNode *Assignment()
		{
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
	};
};

#endif