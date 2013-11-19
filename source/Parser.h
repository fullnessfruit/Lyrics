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

			mToken = token.cbegin();
			mLastToken = token.cend();

			return Block();
		}
	
	private:
		forward_list<Token>::const_iterator mToken;
		forward_list<Token>::const_iterator mLastToken;

		BlockNode *Block()
		{
			BlockNode *node = new BlockNode( mToken->location );

			node->last = node->list.cbefore_begin();
			if ( mToken->type != Token::Type::END && mToken->type != Token::Type::ELSE && mToken->type != Token::Type::ELSEIF && mToken->type != Token::Type::WHEN && mToken != mLastToken )
			{
				node->last = node->list.insert_after( node->last, Statement() );
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
				{
					forward_list<Token>::const_iterator tToken = mToken;
					
					mToken++;

					return FunctionLiteral( tToken );
				}

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
				node->last = node->list.cbefore_begin();
				for (;;)
				{
					node->last = node->list.insert_after( node->last, Expression() );

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

				node->last = node->list.cbefore_begin();
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

					node->last = node->list.insert_after( node->last, new HashNode( tToken->location, expression, Expression() ) );

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

		FunctionLiteralNode *FunctionLiteral( forward_list<Token>::const_iterator &token )
		{
			if ( mToken->type != static_cast<Token::Type>( u'(' ) )
			{
				FunctionLiteralNode *node = new FunctionLiteralNode( token->location );

				mToken++;

				if ( mToken->type != static_cast<Token::Type>( u')' ) )
				{
					ParameterNode *parameter;
					bool isValueParameter;
					IdentifierNode *name;

					node->last = node->list.cbefore_begin();
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

						node->last = node->list.insert_after( node->last, parameter );

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

					IndexReferenceNode *node = new IndexReferenceNode( tToken->location, expression, Expression() );

					if ( mToken->type == static_cast<Token::Type>( u']' ) )
					{
						mToken++;

						expression = static_cast<ExpressionNode *>( node );
					}
					else
					{
						BuildLog::Error( ErrorCode::EXPECTED_INDEX, mToken->location );
						delete node;
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
						node->last = node->list.cbefore_begin();
						for (;;)
						{
							node->last = node->list.insert_after( node->last, Expression() );

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
								delete expression;

								return nullptr;
							}
						}
					}
					mToken++;

					return expression;
				}
				else if ( mToken->type == static_cast<Token::Type>( u'.' ) )
				{
					mToken++;

					if ( mToken->type == Token::Type::IDENTIFIER )
					{
						MemberReferenceNode *node = new MemberReferenceNode( tToken->location, expression, new IdentifierNode( mToken->location, mToken->value.identifier ) );

						mToken++;

						expression = static_cast<ExpressionNode *>( node );
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
				UnaryExpressionNode *node = new UnaryExpressionNode( mToken->location );

				node->op = mToken->type;
				mToken++;

				node->expression = UnaryExpression();

				return node;
			}
		}

		ExpressionNode *MultiplicativeExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = UnaryExpression();

			for (;;)
			{
				if ( mToken->type != static_cast<Token::Type>( u'*' ) && mToken->type != static_cast<Token::Type>( u'/' ) && mToken->type != static_cast<Token::Type>( u'%' ) )
				{
					return expression;
				}
				else
				{
					MultiplicativeExpressionNode *node = new MultiplicativeExpressionNode( tToken->location );

					node->op = mToken->type;
					mToken++;

					node->left = expression;
					node->right = MultiplicativeExpression();

					expression = static_cast<ExpressionNode *>( node );
				}
			}
		}

		ExpressionNode *AdditiveExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = MultiplicativeExpression();

			for (;;)
			{
				if ( mToken->type != static_cast<Token::Type>( u'+' ) && mToken->type != static_cast<Token::Type>( u'-' ) )
				{
					return expression;
				}
				else
				{
					AdditiveExpressionNode *node = new AdditiveExpressionNode( tToken->location );

					node->op = mToken->type;
					mToken++;

					node->left = expression;
					node->right = AdditiveExpression();

					expression = static_cast<ExpressionNode *>( node );
				}
			}
		}

		ExpressionNode *ShiftExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = AdditiveExpression();

			for (;;)
			{
				if ( mToken->type != Token::Type::SHIFT_LEFT && mToken->type != Token::Type::SHIFT_RIGHT )
				{
					return expression;
				}
				else
				{
					ShiftExpressionNode *node = new ShiftExpressionNode( tToken->location );

					node->op = mToken->type;
					mToken++;

					node->left = expression;
					node->right = ShiftExpression();

					expression = static_cast<ExpressionNode *>( node );
				}
			}
		}

		ExpressionNode *AndExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = ShiftExpression();

			for (;;)
			{
				if ( mToken->type != static_cast<Token::Type>( u'&' ) )
				{
					return expression;
				}
				else
				{
					AndExpressionNode *node = new AndExpressionNode( tToken->location );

					mToken++;

					node->left = expression;
					node->right = AndExpression();

					expression = static_cast<ExpressionNode *>( node );
				}
			}
		}

		ExpressionNode *OrExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = AndExpression();

			for (;;)
			{
				if ( mToken->type != static_cast<Token::Type>( u'|' ) && mToken->type != static_cast<Token::Type>( u'^' ) )
				{
					return expression;
				}
				else
				{
					OrExpressionNode *node = new OrExpressionNode( tToken->location );

					node->op = mToken->type;
					mToken++;

					node->left = expression;
					node->right = OrExpression();

					expression = static_cast<ExpressionNode *>( node );
				}
			}
		}

		ExpressionNode *RelationalExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = OrExpression();

			for (;;)
			{
				if ( mToken->type != static_cast<Token::Type>( u'<' ) && mToken->type != static_cast<Token::Type>( u'>' ) && mToken->type != Token::Type::LESS_THAN_OR_EQUAL && mToken->type != Token::Type::GREATER_THAN_OR_EQUAL )
				{
					return expression;
				}
				else
				{
					RelationalExpressionNode *node = new RelationalExpressionNode( tToken->location );

					node->op = mToken->type;
					mToken++;

					node->left = expression;
					node->right = RelationalExpression();

					expression = static_cast<ExpressionNode *>( node );
				}
			}
		}

		ExpressionNode *EqualityExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = RelationalExpression();

			for (;;)
			{
				if ( mToken->type != Token::Type::EQUAL && mToken->type != Token::Type::NOT_EQUAL )
				{
					return expression;
				}
				else
				{
					EqualityExpressionNode *node = new EqualityExpressionNode( tToken->location );

					node->op = mToken->type;
					mToken++;

					node->left = expression;
					node->right = EqualityExpression();

					expression = static_cast<ExpressionNode *>( node );
				}
			}
		}

		ExpressionNode *LogicalAndExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = EqualityExpression();

			for (;;)
			{
				if ( mToken->type != Token::Type::AND )
				{
					return expression;
				}
				else
				{
					LogicalAndExpressionNode *node = new LogicalAndExpressionNode( tToken->location );

					mToken++;

					node->left = expression;
					node->right = LogicalAndExpression();

					expression = static_cast<ExpressionNode *>( node );
				}
			}
		}

		ExpressionNode *LogicalOrExpression()
		{
			forward_list<Token>::const_iterator tToken = mToken;
			ExpressionNode *expression = LogicalAndExpression();

			for (;;)
			{
				if ( mToken->type != Token::Type::OR )
				{
					return expression;
				}
				else
				{
					LogicalOrExpressionNode *node = new LogicalOrExpressionNode( tToken->location );

					mToken++;

					node->left = expression;
					node->right = LogicalOrExpression();

					expression = static_cast<ExpressionNode *>( node );
				}
			}
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
						AssignmentExpressionNode *node = new AssignmentExpressionNode( tToken->location );

						mToken++;

						node->lhs = expression;
						node->rhs = AssignmentExpression();

						return node;
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
					AssignmentExpressionNode *node = new AssignmentExpressionNode( tToken->location );

					node->lhs = new IdentifierNode( mToken->location, mToken->value.identifier );
					mToken++;

					node->rhs = FunctionLiteral( tToken );
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
				PackageNode *node = new PackageNode( tToken->location );

				node->name = new IdentifierNode( mToken->location, mToken->value.identifier );
				mToken++;

				node->block = Block();

				return node;
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
				ImportNode *node = new ImportNode( tToken->location, new IdentifierNode( mToken->location, mToken->value.identifier ) );
				mToken++;

				return node;
			}
			else
			{
				BuildLog::Error( ErrorCode::EXPECTED_PACKAGE, mToken->location );

				return nullptr;
			}
		}

		IfNode* If()
		{
			IfNode *node = new IfNode( mToken->location );
			ElseIfNode *tNode = new ElseIfNode( mToken->location );

			mToken++;

			tNode->condition = Expression();
			if ( mToken->type == Token::Type::THEN || mToken->type == static_cast<Token::Type>( u':' ) )
			{
				mToken++;
			}
			tNode->block = Block();
			node->last = node->list.cbefore_begin();
			node->last = node->list.insert_after( node->last, tNode );

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
					tNode = new ElseIfNode( mToken->location );
					mToken++;

					tNode->condition = Expression();
					tNode->block = Block();
					node->last = node->list.insert_after( node->last, tNode );

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

		CaseNode* Case()
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
				node->last = node->list.cbefore_begin();
				node->last = node->list.insert_after( node->last, whenNode );

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
						node->last = node->list.insert_after( node->last, whenNode );
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

		WhileNode* While()
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

		IterationNode* For()
		{
			forward_list<Token>::const_iterator tToken = mToken;

			mToken++;

			ExpressionNode *expression = Expression();

			if ( mToken->type == static_cast<Token::Type>( u',' ) )
			{
				mToken++;

				ForNode *node = new ForNode( tToken->location );

				node->initializer = expression;
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
			else if ( mToken->type == Token::Type::IN )
			{
				mToken++;

				if ( expression->GetType() == Node::Type::IDENTIFIER || expression->GetType() == Node::Type::MEMBER_REFERENCE || expression->GetType() == Node::Type::INDEX_REFERENCE )
				{
					return ForEach( expression, tToken );
				}
				else
				{
					BuildLog::Error( ErrorCode::EXPECTED_LHS, mToken->location );
					delete expression;

					return nullptr;
				}
			}
			else
			{
				BuildLog::Error( ErrorCode::INCOMPLETE_FOR_FOR_EACH, mToken->location );
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
};

#endif