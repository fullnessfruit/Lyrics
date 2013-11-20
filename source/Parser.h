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
			mLastToken = token.cend();

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
		forward_list<Token>::const_iterator mLastToken;

		BlockNode *Block()
		{
			BlockNode *node = new BlockNode( mToken->location );

			while ( mToken->type != Token::Type::END && mToken->type != Token::Type::ELSE && mToken->type != Token::Type::ELSEIF && mToken->type != Token::Type::WHEN && mToken != mLastToken )
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

			case Token::Type::CLASS:
				return Class();

			case Token::Type::WHILE:
				return While();

			case Token::Type::IMPORT:
				return Import();

			case Token::Type::PUBLIC:
				return Public();

			case Token::Type::FOREACH:
				return ForEach();

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

				return nullptr;
			}
		}

		ArrayLiteralNode *ArrayLiteral()
		{
			ArrayLiteralNode *node = new ArrayLiteralNode( mToken->location );

			mToken++;

			if ( mToken->type != static_cast<Token::Type>( u']' ) )
			{
				for (;;)
				{
					node->AddExpression( Expression() );

					if ( mToken->type == static_cast<Token::Type>( u',' ) )
					{
						mToken++;
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

					forward_list<Token>::const_iterator tToken = mToken;

					mToken++;

					expression = Expression();

					if ( mToken->type != static_cast<Token::Type>( u']' ) )
					{
						BuildLog::Error( ErrorCode::EXPECTED_HASH, mToken->location );
						delete expression;
						delete node;

						return nullptr;
					}
					mToken++;

					if ( mToken->type != static_cast<Token::Type>( u'=' ) )
					{
						BuildLog::Error( ErrorCode::EXPECTED_HASH, mToken->location );
						delete expression;
						delete node;

						return nullptr;
					}
					mToken++;

					node->AddHash( new HashNode( tToken->location, expression, Expression() ) );

					if ( mToken->type == static_cast<Token::Type>( u',' ) )
					{
						mToken++;
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
			if ( mToken->type == static_cast<Token::Type>( u'(' ) )
			{
				FunctionLiteralNode *node = new FunctionLiteralNode( token->location );

				mToken++;

				if ( mToken->type != static_cast<Token::Type>( u')' ) )
				{
					ParameterNode *parameter;
					bool isValueParameter;
					IdentifierNode *name;

					for (;;)
					{
						forward_list<Token>::const_iterator tToken = mToken;

						if ( mToken->type != Token::Type::OUT )
						{
							isValueParameter = true;
						}
						else
						{
							mToken++;

							isValueParameter = false;
						}

						if ( mToken->type == Token::Type::IDENTIFIER )
						{
							name = new IdentifierNode( mToken->location, mToken->value.identifier );
							mToken++;
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

				node->block = Block();

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_PARAMETER, mToken->location );

				return nullptr;
			}
		}

		ParenthesizedExpressionNode *ParenthesizedExpression()
		{
			ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode( mToken->location, Expression() );

			mToken++;

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
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = PrimaryExpression();

			if ( expression->GetType() != Node::Type::IDENTIFIER )
			{
				return expression;
			}

			for (;;)
			{
				if ( mToken->type == static_cast<Token::Type>( u'[' ) )
				{
					mToken++;

					expression = new IndexReferenceNode( tToken->location, expression, Expression() );

					if ( mToken->type == static_cast<Token::Type>( u']' ) )
					{
						mToken++;
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

					FunctionCallNode *node = new FunctionCallNode( tToken->location, expression );

					if ( mToken->type != static_cast<Token::Type>( u')' ) )
					{
						for (;;)
						{
							node->AddArgument( Expression() );

							if ( mToken->type == static_cast<Token::Type>( u',' ) )
							{
								mToken++;
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

					if ( mToken->type == Token::Type::IDENTIFIER )
					{
						expression = new MemberReferenceNode( tToken->location, expression, new IdentifierNode( mToken->location, mToken++->value.identifier ) );
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

		ExpressionNode *UnaryExpression()
		{
			if ( mToken->type != static_cast<Token::Type>( u'+' ) && mToken->type != static_cast<Token::Type>( u'-' ) && mToken->type != static_cast<Token::Type>( u'~' ) && mToken->type != static_cast<Token::Type>( u'!' ) )
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
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = UnaryExpression();

			while ( mToken->type == static_cast<Token::Type>( u'*' ) || mToken->type == static_cast<Token::Type>( u'/' ) || mToken->type == static_cast<Token::Type>( u'%' ) )
			{
				expression = new MultiplicativeExpressionNode( tToken->location, mToken++->type, expression, UnaryExpression() );
			}

			return expression;
		}

		ExpressionNode *AdditiveExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = MultiplicativeExpression();

			while ( mToken->type == static_cast<Token::Type>( u'+' ) || mToken->type == static_cast<Token::Type>( u'-' ) )
			{
				expression = new AdditiveExpressionNode( tToken->location, mToken++->type, expression, MultiplicativeExpression() );
			}

			return expression;
		}

		ExpressionNode *ShiftExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = AdditiveExpression();

			while ( mToken->type == Token::Type::SHIFT_LEFT || mToken->type == Token::Type::SHIFT_RIGHT )
			{
				expression = new ShiftExpressionNode( tToken->location, mToken++->type, expression, AdditiveExpression() );
			}

			return expression;
		}

		ExpressionNode *AndExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = ShiftExpression();

			while ( mToken->type == static_cast<Token::Type>( u'&' ) )
			{
				mToken++;
				expression = new AndExpressionNode( tToken->location, expression, ShiftExpression() );
			}

			return expression;
		}

		ExpressionNode *OrExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = AndExpression();

			while ( mToken->type == static_cast<Token::Type>( u'|' ) || mToken->type == static_cast<Token::Type>( u'^' ) )
			{
				expression = new OrExpressionNode( tToken->location, mToken++->type, expression, AndExpression() );
			}

			return expression;
		}

		ExpressionNode *RelationalExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = OrExpression();

			while ( mToken->type == static_cast<Token::Type>( u'<' ) || mToken->type == static_cast<Token::Type>( u'>' ) || mToken->type == Token::Type::LESS_THAN_OR_EQUAL || mToken->type == Token::Type::GREATER_THAN_OR_EQUAL )
			{
				expression = new RelationalExpressionNode( tToken->location, mToken++->type, expression, OrExpression() );
			}

			return expression;
		}

		ExpressionNode *EqualityExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = RelationalExpression();

			while ( mToken->type == Token::Type::EQUAL || mToken->type == Token::Type::NOT_EQUAL )
			{
				expression = new EqualityExpressionNode( tToken->location, mToken++->type, expression, RelationalExpression() );
			}

			return expression;
		}

		ExpressionNode *LogicalAndExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = EqualityExpression();

			while ( mToken->type == Token::Type::AND )
			{
				mToken++;
				expression = new LogicalAndExpressionNode( tToken->location, expression, EqualityExpression() );
			}

			return expression;
		}

		ExpressionNode *LogicalOrExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = LogicalAndExpression();

			while ( mToken->type == Token::Type::OR )
			{
				mToken++;
				expression = new LogicalOrExpressionNode( tToken->location, expression, LogicalAndExpression() );
			}

			return expression;
		}

		ExpressionNode *AssignmentExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;

			if ( mToken->type != Token::Type::DEF )
			{
				ExpressionNode *expression = LogicalOrExpression();

				if ( mToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return expression;
				}
				else
				{
					if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::MEMBER_REFERENCE || expression->GetType() == Node::Type::INDEX_REFERENCE )
					{
						mToken++;

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
			else
			{
				mToken++;

				if ( mToken->type == Token::Type::IDENTIFIER )
				{
					return new AssignmentExpressionNode( tToken->location, new IdentifierNode( mToken->location, mToken++->value.identifier ), FunctionLiteral( tToken ) );
				}
				else
				{
					mToken = tToken;

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
			forward_list<Token>::const_iterator tToken = mToken;

			mToken++;

			if ( mToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( mToken->location, mToken->value.identifier );
				mToken++;

				if ( mToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return new PublicNode( tToken->location, name );
				}
				else
				{
					mToken++;

					return new PublicNode( tToken->location, name, Expression() );
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_VARIABLE_NAME, mToken->location );

				return nullptr;
			}
		}

		PrivateNode *Private()
		{
			forward_list<Token>::const_iterator tToken = mToken;

			mToken++;

			if ( mToken->type == Token::Type::IDENTIFIER )
			{
				IdentifierNode *name = new IdentifierNode( mToken->location, mToken->value.identifier );
				mToken++;

				if ( mToken->type != static_cast<Token::Type>( u'=' ) )
				{
					return new PrivateNode( tToken->location, name );
				}
				else
				{
					mToken++;

					return new PrivateNode( tToken->location, name, Expression() );
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_VARIABLE_NAME, mToken->location );

				return nullptr;
			}
		}

		ClassNode *Class()
		{
			forward_list<Token>::const_iterator tToken = mToken;

			mToken++;

			if ( mToken->type == Token::Type::IDENTIFIER )
			{
				ClassNode *node = new ClassNode( tToken->location );

				node->name = new IdentifierNode( mToken->location, mToken->value.identifier );
				mToken++;

				if ( mToken->type == static_cast<Token::Type>( u':' ) )
				{
					mToken++;

					if ( mToken->type == Token::Type::IDENTIFIER )
					{
						node->base = new IdentifierNode( mToken->location, mToken->value.identifier );
						mToken++;
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_BASE_CLASS, mToken->location );
						delete node;

						return nullptr;
					}
				}

				node->block = Block();

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_CLASS_NAME, mToken->location );

				return nullptr;
			}
		}

		PackageNode *Package()
		{
			forward_list<Token>::const_iterator tToken = mToken;

			mToken++;

			if ( mToken->type == Token::Type::IDENTIFIER )
			{
				return new PackageNode( tToken->location, new IdentifierNode( mToken->location, mToken++->value.identifier ), Block() );
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_PACKAGE_NAME, mToken->location );

				return nullptr;
			}
		}

		ImportNode *Import()
		{
			forward_list<Token>::const_iterator tToken = mToken;

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

		IfNode *If()
		{
			IfNode *node = new IfNode( mToken->location );
			ElseIfNode *elseIfNode = new ElseIfNode( mToken->location );

			mToken++;

			elseIfNode->condition = Expression();
			if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
			{
				mToken++;
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
			else if ( mToken->type == Token::Type::ELSEIF )
			{
				for (;;)
				{
					elseIfNode = new ElseIfNode( mToken->location );
					mToken++;

					elseIfNode->condition = Expression();
					if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
					{
						mToken++;
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
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_END_ELSE_ELSEIF, mToken->location );
				delete node;

				return nullptr;
			}
		}

		CaseNode *Case()
		{
			CaseNode *node = new CaseNode( mToken->location );

			mToken++;

			node->value = Expression();

			if ( mToken->type == Token::Type::WHEN )
			{
				WhenNode *whenNode = new WhenNode( mToken->location );

				mToken++;

				whenNode->condition = Expression();
				if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
				{
					mToken++;
				}
				whenNode->block = Block();
				node->AddWhen( whenNode );

				for (;;)
				{
					if ( mToken->type == Token::Type::WHEN )
					{
						whenNode = new WhenNode( mToken->location );
						mToken++;

						whenNode->condition = Expression();
						if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
						{
							mToken++;
						}
						whenNode->block = Block();
						node->AddWhen( whenNode );
					}
					else if ( mToken->type == Token::Type::ELSE )
					{
						mToken++;

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
					else
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

			node->condition = Expression();

			if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
			{
				mToken++;
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

			node->initializer = Expression();

			if ( mToken->type == static_cast<Token::Type>( u',' ) )
			{
				mToken++;

				node->condition = Expression();

				if ( mToken->type == static_cast<Token::Type>( u',' ) )
				{
					mToken++;

					node->iterator = Expression();

					if ( mToken->type == Token::Type::DO || mToken->type == static_cast<Token::Type>( u':' ) )
					{
						mToken++;
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
					BuildLog::Error( ErrorCode::INCOMPLETE_FOR, mToken->location );
					delete node;

					return nullptr;
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_FOR, mToken->location );
				delete node;

				return nullptr;
			}
		}

		ForEachNode *ForEach()
		{
			ForEachNode *node = new ForEachNode( mToken->location );

			mToken++;

			node->variable = Expression();
			if ( node->variable ->GetType() == Node::Type::IDENTIFIER || node->variable ->GetType() == Node::Type::MEMBER_REFERENCE || node->variable ->GetType() == Node::Type::INDEX_REFERENCE )
			{
				if ( mToken->type == Token::Type::IN )
				{
					mToken++;

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
					BuildLog::Error( ErrorCode::INCOMPLETE_FOREACH, mToken->location );
					delete node;

					return nullptr;
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_LHS, mToken->location );
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
			forward_list<Token>::const_iterator tToken = mToken;

			mToken++;

			if ( mToken->type == Token::Type::END || mToken->type == Token::Type::ELSE || mToken->type == Token::Type::ELSEIF || mToken->type == Token::Type::WHEN || mToken == mLastToken )
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
