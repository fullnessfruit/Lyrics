#include "Parser.h"

#include <new>

#include "Tokenizer.h"

#include "ErrorCode.h"
#include "ErrorHandler.h"

#include "Utility.h"

namespace lyrics
{
	BlockNode *Parser::Parse(const string &fileName)
	{
		using std::bad_alloc;

		const forward_list<Token> *tokenList = Tokenizer().Tokenize(fileName);
		BlockNode *root = nullptr;

		mToken = tokenList->cbegin();

		try
		{
			root = Block();
		}
		catch (const bad_alloc &e)
		{
			Utility::SafeDelete(tokenList);
			Utility::SafeDelete(root);
			throw FatalErrorCode::NOT_ENOUGH_MEMORY;
		}

		if (!root)
		{
			throw FatalErrorCode::CANNOT_PARSE;
		}

		Utility::SafeDelete(tokenList);
		return root;
	}

	BlockNode *Parser::Block()
	{
		BlockNode *node = new BlockNode(mToken->location);

		while (mToken->type != Token::Type::END && mToken->type != Token::Type::ELSE && mToken->type != Token::Type::ELSEIF && mToken->type != Token::Type::PRIVATE && mToken->type != Token::Type::PUBLIC && mToken->type != Token::Type::WHEN && mToken->type != Token::Type::END_OF_FILE)
		{
			node->AddStatement(Statement());
		}

		return node;
	}

	StatementNode *Parser::Statement()
	{
		switch (mToken->type)
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

	ExpressionNode *Parser::PrimaryExpression()
	{
		switch (mToken->type)
		{
		case Token::Type::IDENTIFIER:
			return new IdentifierNode(mToken->location, mToken++->value.identifier);

		case Token::Type::INTEGER_LITERAL:
			return new IntegerLiteralNode(mToken->location, mToken++->value.integer);

		case Token::Type::STRING_LITERAL:
			return new StringLiteralNode(mToken->location, mToken++->value.string);

		case Token::Type::BOOLEAN_LITERAL:
			return new BooleanLiteralNode(mToken->location, mToken++->value.boolean);

		case Token::Type::NULL_LITERAL:
			return new NullLiteralNode(mToken++->location);

		case Token::Type::REAL_LITERAL:
			return new RealLiteralNode(mToken->location, mToken++->value.real);

		case Token::Type::DO:
			return FunctionLiteral(mToken++);

		case static_cast<Token::Type>(U'(') :
			return ParenthesizedExpression();

		case static_cast<Token::Type>(U'[') :
			return ArrayLiteral();

		case static_cast<Token::Type>(U'{') :
			return HashLiteral();

		case Token::Type::THIS:
			return new ThisNode(mToken->location);

		default:
			ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_PRIMARY_EXPRESSION);

			mToken++;

			return nullptr;
		}
	}

	ArrayLiteralNode *Parser::ArrayLiteral()
	{
		ArrayLiteralNode *node = new ArrayLiteralNode(mToken->location);

		mToken++;
		if (mToken->type == Token::Type::END_OF_FILE)
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL);
			Utility::SafeDelete(node);
			return nullptr;
		}

		if (mToken->type != static_cast<Token::Type>(U']'))
		{
			for (;;)
			{
				node->AddExpression(Expression());

				if (mToken->type == static_cast<Token::Type>(U','))
				{
					mToken++;
					if (mToken->type == Token::Type::END_OF_FILE)
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL);
						Utility::SafeDelete(node);
						return nullptr;
					}
					else if (mToken->type == static_cast<Token::Type>(U']'))
					{
						break;
					}
				}
				else if (mToken->type == static_cast<Token::Type>(U']'))
				{
					break;
				}
				else
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_ARRAY_LITERAL);
					Utility::SafeDelete(node);
					return nullptr;
				}
			}
		}

		mToken++;

		return node;
	}

	HashLiteralNode *Parser::HashLiteral()
	{
		HashLiteralNode *node = new HashLiteralNode(mToken->location);

		mToken++;
		if (mToken->type == Token::Type::END_OF_FILE)
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL);
			Utility::SafeDelete(node);
			return nullptr;
		}

		if (mToken->type != static_cast<Token::Type>(U'}'))
		{
			ExpressionNode *expression;

			for (;;)
			{
				Location tLocation = mToken->location;

				expression = Expression();

				if (mToken->type != static_cast<Token::Type>(U':'))
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_HASH);
					Utility::SafeDelete(expression);
					Utility::SafeDelete(node);
					return nullptr;
				}

				mToken++;
				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL);
					Utility::SafeDelete(expression);
					Utility::SafeDelete(node);
					return nullptr;
				}

				node->AddHash(new HashNode(tLocation, expression, Expression()));

				if (mToken->type == static_cast<Token::Type>(U','))
				{
					mToken++;
					if (mToken->type == Token::Type::END_OF_FILE)
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL);
						Utility::SafeDelete(node);
						return nullptr;
					}
					else if (mToken->type == static_cast<Token::Type>(U'}'))
					{
						break;
					}
				}
				else if (mToken->type == static_cast<Token::Type>(U'}'))
				{
					break;
				}
				else
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_HASH_LITERAL);
					Utility::SafeDelete(node);
					return nullptr;
				}
			}
		}

		mToken++;

		return node;
	}

	FunctionLiteralNode *Parser::FunctionLiteral(forward_list<Token>::const_iterator token)
	{
		if (mToken->type == static_cast<Token::Type>(U'('))
		{
			mToken++;
			if (mToken->type == Token::Type::END_OF_FILE)
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FUNCTION);
				return nullptr;
			}

			FunctionLiteralNode *node = new FunctionLiteralNode(token->location);

			if (mToken->type != static_cast<Token::Type>(U')'))
			{
				ParameterNode *parameter;
				bool isValueParameter;
				IdentifierNode *name;

				for (;;)
				{
					Location tLocation = mToken->location;

					if (mToken->type != Token::Type::OUT)
					{
						isValueParameter = true;
					}
					else
					{
						mToken++;
						if (mToken->type == Token::Type::END_OF_FILE)
						{
							ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FUNCTION);
							Utility::SafeDelete(node);
							return nullptr;
						}

						isValueParameter = false;
					}

					if (mToken->type == Token::Type::IDENTIFIER)
					{
						name = new IdentifierNode(mToken->location, mToken->value.identifier);

						mToken++;
						if (mToken->type == Token::Type::END_OF_FILE)
						{
							ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FUNCTION);
							Utility::SafeDelete(name);
							Utility::SafeDelete(node);
							return nullptr;
						}
					}
					else
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_PARAMETER_NAME);
						Utility::SafeDelete(node);
						return nullptr;
					}

					if (mToken->type != static_cast<Token::Type>(U'='))
					{
						if (isValueParameter)
						{
							parameter = new ValueParameterNode(tLocation, name);
						}
						else
						{
							parameter = new OutputParameterNode(tLocation, name);
						}
					}
					else
					{
						mToken++;
						if (mToken->type == Token::Type::END_OF_FILE)
						{
							ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FUNCTION);
							Utility::SafeDelete(name);
							Utility::SafeDelete(node);
							return nullptr;
						}

						if (isValueParameter)
						{
							parameter = new ValueParameterNode(tLocation, name, Expression());
						}
						else
						{
							ErrorHandler::OnError(mToken->location, ErrorCode::OUTPUT_PARAMETER_DEFAULT_ARGUMENT);
							Utility::SafeDelete(name);
							Utility::SafeDelete(node);
							return nullptr;
						}
					}

					node->AddParameter(parameter);

					if (mToken->type == static_cast<Token::Type>(U','))
					{
						mToken++;
						if (mToken->type == Token::Type::END_OF_FILE)
						{
							ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FUNCTION);
							Utility::SafeDelete(node);
							return nullptr;
						}
					}
					else if (mToken->type == static_cast<Token::Type>(U')'))
					{
						break;
					}
					else
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FUNCTION);
						Utility::SafeDelete(node);
						return nullptr;
					}
				}
			}

			mToken++;
			if (mToken->type == Token::Type::END_OF_FILE)
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FUNCTION);
				Utility::SafeDelete(node);
				return nullptr;
			}

			node->block = Block();

			if (mToken->type == Token::Type::END)
			{
				mToken++;

				return node;
			}
			else
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_END);
				Utility::SafeDelete(node);
				return nullptr;
			}
		}
		else
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_PARAMETER);
			return nullptr;
		}
	}

	ParenthesizedExpressionNode *Parser::ParenthesizedExpression()
	{
		ParenthesizedExpressionNode *node = new ParenthesizedExpressionNode(mToken->location);

		mToken++;
		if (mToken->type == Token::Type::END_OF_FILE)
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_EXPRESSION);
			Utility::SafeDelete(node);
			return nullptr;
		}

		node->expression = Expression();

		if (mToken->type == static_cast<Token::Type>(U')'))
		{
			mToken++;

			return node;
		}
		else
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_RIGHT_PARENTHESIS);
			Utility::SafeDelete(node);
			return nullptr;
		}
	}

	ExpressionNode *Parser::PostfixExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = PrimaryExpression();

		if (expression == nullptr || (expression->type != Node::Type::IDENTIFIER && expression->type != Node::Type::THIS))
		{
			return expression;
		}

		for (;;)
		{
			if (mToken->type == static_cast<Token::Type>(U'['))
			{
				mToken++;
				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_INDEX);
					Utility::SafeDelete(expression);
					return nullptr;
				}

				expression = new IndexReferenceNode(tLocation, expression, Expression());

				if (mToken->type == static_cast<Token::Type>(U']'))
				{
					mToken++;
					if (mToken->type == Token::Type::END_OF_FILE)
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_INDEX);
						Utility::SafeDelete(expression);
						return nullptr;
					}
				}
				else
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_INDEX);
					Utility::SafeDelete(expression);
					return nullptr;
				}
			}
			else if (mToken->type == static_cast<Token::Type>(U'('))
			{
				mToken++;
				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_FUNCTION_CALL);
					Utility::SafeDelete(expression);
					return nullptr;
				}

				FunctionCallNode *node = new FunctionCallNode(tLocation, expression);

				if (mToken->type != static_cast<Token::Type>(U')'))
				{
					for (;;)
					{
						node->AddArgument(Expression());

						if (mToken->type == static_cast<Token::Type>(U','))
						{
							mToken++;
							if (mToken->type == Token::Type::END_OF_FILE)
							{
								ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_FUNCTION_CALL);
								Utility::SafeDelete(node);
								return nullptr;
							}
						}
						else if (mToken->type == static_cast<Token::Type>(U')'))
						{
							break;
						}
						else
						{
							ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_FUNCTION_CALL);
							Utility::SafeDelete(node);
							return nullptr;
						}
					}
				}

				mToken++;

				return node;
			}
			else if (mToken->type == static_cast<Token::Type>(U'.'))
			{
				mToken++;
				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_MEMBER);
					Utility::SafeDelete(expression);
					return nullptr;
				}

				if (mToken->type == Token::Type::IDENTIFIER)
				{
					expression = new MemberReferenceNode(tLocation, expression, new IdentifierNode(mToken->location, mToken->value.identifier));

					mToken++;
					if (mToken->type == Token::Type::END_OF_FILE)
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_MEMBER);
						Utility::SafeDelete(expression);
						return nullptr;
					}
				}
				else
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_MEMBER);
					Utility::SafeDelete(expression);
					return nullptr;
				}
			}
			else
			{
				return expression;
			}
		}
	}

	ExpressionNode *Parser::UnaryExpression()
	{
		if (mToken->type == Token::Type::END_OF_FILE)
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_EXPRESSION);
			return nullptr;
		}
		else if (mToken->type != static_cast<Token::Type>(U'+') && mToken->type != static_cast<Token::Type>(U'-') && mToken->type != static_cast<Token::Type>(U'~') && mToken->type != static_cast<Token::Type>(U'!'))
		{
			return PostfixExpression();
		}
		else
		{
			auto tToken = mToken++;

			return new UnaryExpressionNode(tToken->location, tToken->type, UnaryExpression());
		}
	}

	ExpressionNode *Parser::MultiplicativeExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = UnaryExpression();

		while (mToken->type == static_cast<Token::Type>(U'*') || mToken->type == static_cast<Token::Type>(U'/') || mToken->type == static_cast<Token::Type>(U'%'))
		{
			auto tToken = mToken++;

			expression = new MultiplicativeExpressionNode(tLocation, tToken->type, expression, UnaryExpression());
		}

		return expression;
	}

	ExpressionNode *Parser::AdditiveExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = MultiplicativeExpression();

		while (mToken->type == static_cast<Token::Type>(U'+') || mToken->type == static_cast<Token::Type>(U'-'))
		{
			auto tToken = mToken++;

			expression = new AdditiveExpressionNode(tLocation, tToken->type, expression, MultiplicativeExpression());
		}

		return expression;
	}

	ExpressionNode *Parser::ShiftExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = AdditiveExpression();

		while (mToken->type == Token::Type::SHIFT_LEFT || mToken->type == Token::Type::SHIFT_RIGHT)
		{
			auto tToken = mToken++;

			expression = new ShiftExpressionNode(tLocation, tToken->type, expression, AdditiveExpression());
		}

		return expression;
	}

	ExpressionNode *Parser::AndExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = ShiftExpression();

		while (mToken->type == static_cast<Token::Type>(U'&'))
		{
			mToken++;

			if (mToken->type == Token::Type::END_OF_FILE)
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_EXPRESSION);
				Utility::SafeDelete(expression);
				return nullptr;
			}

			expression = new AndExpressionNode(tLocation, expression, ShiftExpression());
		}

		return expression;
	}

	ExpressionNode *Parser::OrExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = AndExpression();

		while (mToken->type == static_cast<Token::Type>(U'|') || mToken->type == static_cast<Token::Type>(U'^'))
		{
			auto tToken = mToken++;

			expression = new OrExpressionNode(tLocation, tToken->type, expression, AndExpression());
		}

		return expression;
	}

	ExpressionNode *Parser::RelationalExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = OrExpression();

		while (mToken->type == static_cast<Token::Type>(U'<') || mToken->type == static_cast<Token::Type>(U'>') || mToken->type == Token::Type::LESS_THAN_OR_EQUAL || mToken->type == Token::Type::GREATER_THAN_OR_EQUAL)
		{
			auto tToken = mToken++;

			expression = new RelationalExpressionNode(tLocation, tToken->type, expression, OrExpression());
		}

		return expression;
	}

	ExpressionNode *Parser::EqualityExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = RelationalExpression();

		while (mToken->type == Token::Type::EQUAL || mToken->type == Token::Type::NOT_EQUAL)
		{
			auto tToken = mToken++;

			expression = new EqualityExpressionNode(tLocation, tToken->type, expression, RelationalExpression());
		}

		return expression;
	}

	ExpressionNode *Parser::LogicalAndExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = EqualityExpression();

		while (mToken->type == Token::Type::AND)
		{
			mToken++;
			if (mToken->type == Token::Type::END_OF_FILE)
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_EXPRESSION);
				Utility::SafeDelete(expression);
				return nullptr;
			}

			expression = new LogicalAndExpressionNode(tLocation, expression, EqualityExpression());
		}

		return expression;
	}

	ExpressionNode *Parser::LogicalOrExpression()
	{
		Location tLocation = mToken->location;
		ExpressionNode *expression = LogicalAndExpression();

		while (mToken->type == Token::Type::OR)
		{
			mToken++;
			if (mToken->type == Token::Type::END_OF_FILE)
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_EXPRESSION);
				Utility::SafeDelete(expression);
				return nullptr;
			}

			expression = new LogicalOrExpressionNode(tLocation, expression, LogicalAndExpression());
		}

		return expression;
	}

	ExpressionNode *Parser::AssignmentExpression()
	{
		auto tToken = mToken;

		switch (mToken->type)
		{
		case Token::Type::CLASS:
			return Class();

		case Token::Type::PACKAGE:
			return Package();

		case Token::Type::END_OF_FILE:
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_EXPRESSION);
			return nullptr;

		default:
			ExpressionNode *expression = LogicalOrExpression();

			if (expression == nullptr || mToken->type != static_cast<Token::Type>(U'='))
			{
				return expression;
			}
			else
			{
				if (expression->type == Node::Type::IDENTIFIER || expression->type == Node::Type::MEMBER_REFERENCE || expression->type == Node::Type::INDEX_REFERENCE)
				{
					mToken++;

					if (mToken->type == Token::Type::END_OF_FILE)
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_EXPRESSION);
						Utility::SafeDelete(expression);
						return nullptr;
					}

					return new AssignmentExpressionNode(tToken->location, expression, AssignmentExpression());
				}
				else
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_LHS);
					Utility::SafeDelete(expression);
					return nullptr;
				}
			}
		}
	}

	AssignmentExpressionNode *Parser::Class()
	{
		Location tLocation = mToken->location;

		mToken++;
		if (mToken->type == Token::Type::IDENTIFIER)
		{
			IdentifierNode *name = new IdentifierNode(mToken->location, mToken->value.identifier);
			ClassNode *node = new ClassNode(tLocation);

			mToken++;
			if (mToken->type == static_cast<Token::Type>(U'('))
			{
				mToken++;
				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION);
					Utility::SafeDelete(node);
					Utility::SafeDelete(name);
					return nullptr;
				}

				if (mToken->type != static_cast<Token::Type>(U')'))
				{
					for (;;)
					{
						node->AddArgument(Expression());

						if (mToken->type == static_cast<Token::Type>(U','))
						{
							mToken++;
							if (mToken->type == Token::Type::END_OF_FILE)
							{
								ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION);
								Utility::SafeDelete(node);
								Utility::SafeDelete(name);
								return nullptr;
							}
						}
						else if (mToken->type == static_cast<Token::Type>(U')'))
						{
							break;
						}
						else
						{
							ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION);
							Utility::SafeDelete(node);
							Utility::SafeDelete(name);
							return nullptr;
						}
					}
				}
			}
			else
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION);
				Utility::SafeDelete(node);
				Utility::SafeDelete(name);
				return nullptr;
			}

			mToken++;
			if (mToken->type == static_cast<Token::Type>(U':'))
			{
				node->baseClassConstructorCall = new BaseClassConstructorCallNode(mToken->location);

				mToken++;
				if (mToken->type == Token::Type::IDENTIFIER)
				{
					node->baseClassConstructorCall->baseClass = new IdentifierNode(mToken->location, mToken->value.identifier);

					mToken++;
					if (mToken->type == static_cast<Token::Type>(U'('))
					{
						mToken++;
						if (mToken->type == Token::Type::END_OF_FILE)
						{
							ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION);
							Utility::SafeDelete(node);
							Utility::SafeDelete(name);
							return nullptr;
						}

						if (mToken->type != static_cast<Token::Type>(U')'))
						{
							for (;;)
							{
								node->baseClassConstructorCall->AddArgument(Expression());

								if (mToken->type == static_cast<Token::Type>(U','))
								{
									mToken++;
									if (mToken->type == Token::Type::END_OF_FILE)
									{
										ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION);
										Utility::SafeDelete(node);
										Utility::SafeDelete(name);
										return nullptr;
									}
								}
								else if (mToken->type == static_cast<Token::Type>(U')'))
								{
									break;
								}
								else
								{
									ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION);
									Utility::SafeDelete(node);
									Utility::SafeDelete(name);
									return nullptr;
								}
							}
						}
					}
					else
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CLASS_DEFINITION);
						Utility::SafeDelete(node);
						Utility::SafeDelete(name);
						return nullptr;
					}
				}
				else
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_BASE_CLASS);
					Utility::SafeDelete(node);
					Utility::SafeDelete(name);
					return nullptr;
				}
			}

			mToken++;
			if (mToken->type == Token::Type::INCLUDE)
			{
				node->include = Include();
			}

			node->block = Block();

			mToken++;
			if (mToken->type == Token::Type::END)
			{
				mToken++;

				return new AssignmentExpressionNode(tLocation, name, node);
			}
			else
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_END);
				Utility::SafeDelete(node);
				Utility::SafeDelete(name);
				return nullptr;
			}
		}
		else
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_CLASS_NAME);
			return nullptr;
		}
	}

	IncludeNode *Parser::Include()
	{
		IncludeNode *node = new IncludeNode(mToken->location);

		do
		{
			mToken++;
			if (mToken->type == Token::Type::IDENTIFIER)
			{
				node->AddPackage(new IdentifierNode(mToken->location, mToken++->value.identifier));
			}
			else
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_PACKAGE);
				Utility::SafeDelete(node);
				return nullptr;
			}
		} while (mToken->type == static_cast<Token::Type>(U','));

		return node;
	}

	AssignmentExpressionNode *Parser::Package()
	{
		Location tLocation = mToken->location;

		mToken++;
		if (mToken->type == Token::Type::IDENTIFIER)
		{
			IdentifierNode *name = new IdentifierNode(mToken->location, mToken->value.identifier);
			PackageNode *node = new PackageNode(tLocation, Block());

			mToken++;
			if (mToken->type == Token::Type::END)
			{
				mToken++;

				return new AssignmentExpressionNode(tLocation, name, node);
			}
			else
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_END);
				Utility::SafeDelete(node);
				Utility::SafeDelete(name);
				return nullptr;
			}
		}
		else
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_PACKAGE_NAME);
			return nullptr;
		}
	}

	ExpressionNode *Parser::Expression()
	{
		return AssignmentExpression();
	}

	ImportNode *Parser::Import()
	{
		ImportNode *node = new ImportNode(mToken->location);

		do
		{
			mToken++;
			if (mToken->type == Token::Type::IDENTIFIER)
			{
				node->AddIdentifier(new IdentifierNode(mToken->location, mToken++->value.identifier));
			}
			else
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_IDENTIFIER);
				Utility::SafeDelete(node);
				return nullptr;
			}
		} while (mToken->type == static_cast<Token::Type>(U'.'));

		return node;
	}

	IfNode *Parser::If()
	{
		IfNode *node = new IfNode(mToken->location);
		ElseIfNode *elseIfNode;

		for (;;)
		{
			elseIfNode = new ElseIfNode(mToken->location);

			mToken++;
			if (mToken->type == Token::Type::END_OF_FILE)
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT);
				Utility::SafeDelete(elseIfNode);
				Utility::SafeDelete(node);
				return nullptr;
			}

			elseIfNode->condition = Expression();

			if (mToken->type == Token::Type::THEN)
			{
				mToken++;
			}

			if (mToken->type == Token::Type::END_OF_FILE)
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT);
				Utility::SafeDelete(elseIfNode);
				Utility::SafeDelete(node);
				return nullptr;
			}

			elseIfNode->block = Block();
			node->AddElseIf(elseIfNode);

			if (mToken->type == Token::Type::END)
			{
				mToken++;

				return node;
			}
			else if (mToken->type == Token::Type::ELSE)
			{
				mToken++;
				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_IF_STATEMENT);
					Utility::SafeDelete(node);
					return nullptr;
				}

				node->block = Block();

				if (mToken->type == Token::Type::END)
				{
					mToken++;

					return node;
				}
				else
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_END);
					Utility::SafeDelete(node);
					return nullptr;
				}
			}
			else if (mToken->type != Token::Type::ELSEIF)
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_END_ELSE_ELSEIF);
				Utility::SafeDelete(node);
				return nullptr;
			}
		}
	}

	CaseNode *Parser::Case()
	{
		CaseNode *node = new CaseNode(mToken->location);

		mToken++;
		if (mToken->type == Token::Type::END_OF_FILE)
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT);
			Utility::SafeDelete(node);
			return nullptr;
		}

		node->value = Expression();

		if (mToken->type == Token::Type::WHEN)
		{
			WhenNode *whenNode;

			for (;;)
			{
				whenNode = new WhenNode(mToken->location);

				mToken++;
				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT);
					Utility::SafeDelete(whenNode);
					Utility::SafeDelete(node);
					return nullptr;
				}

				whenNode->condition = Expression();

				if (mToken->type == Token::Type::THEN)
				{
					mToken++;
				}

				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT);
					Utility::SafeDelete(whenNode);
					Utility::SafeDelete(node);
					return nullptr;
				}

				whenNode->block = Block();
				node->AddWhen(whenNode);

				if (mToken->type == Token::Type::ELSE)
				{
					mToken++;
					if (mToken->type == Token::Type::END_OF_FILE)
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_CASE_STATEMENT);
						Utility::SafeDelete(node);
						return nullptr;
					}

					node->block = Block();

					if (mToken->type == Token::Type::END)
					{
						mToken++;

						return node;
					}
					else
					{
						ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_END);
						Utility::SafeDelete(node);
						return nullptr;
					}
				}
				else if (mToken->type == Token::Type::END)
				{
					mToken++;

					return node;
				}
				else if (mToken->type != Token::Type::WHEN)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_WHEN_ELSE_ELSEIF);
					Utility::SafeDelete(node);
					return nullptr;
				}
			}
		}
		else
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_WHEN);
			Utility::SafeDelete(node);
			return nullptr;
		}
	}

	WhileNode *Parser::While()
	{
		WhileNode *node = new WhileNode(mToken->location);

		mToken++;
		if (mToken->type == Token::Type::END_OF_FILE)
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_WHILE_STATEMENT);
			Utility::SafeDelete(node);
			return nullptr;
		}

		node->condition = Expression();

		if (mToken->type == Token::Type::DO)
		{
			mToken++;
		}

		if (mToken->type == Token::Type::END_OF_FILE)
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_WHILE_STATEMENT);
			Utility::SafeDelete(node);
			return nullptr;
		}

		node->block = Block();

		if (mToken->type == Token::Type::END)
		{
			mToken++;

			return node;
		}
		else
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_END);
			Utility::SafeDelete(node);
			return nullptr;
		}
	}

	ForNode *Parser::For()
	{
		ForNode *node = new ForNode(mToken->location);

		mToken++;
		if (mToken->type == Token::Type::END_OF_FILE)
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT);
			Utility::SafeDelete(node);
			return nullptr;
		}

		node->initializer = Expression();

		if (mToken->type == static_cast<Token::Type>(U','))
		{
			mToken++;
			if (mToken->type == Token::Type::END_OF_FILE)
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT);
				Utility::SafeDelete(node);
				return nullptr;
			}

			node->condition = Expression();

			if (mToken->type == static_cast<Token::Type>(U','))
			{
				mToken++;
				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT);
					Utility::SafeDelete(node);
					return nullptr;
				}

				node->iterator = Expression();

				if (mToken->type == Token::Type::DO)
				{
					mToken++;
				}

				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT);
					Utility::SafeDelete(node);
					return nullptr;
				}

				node->block = Block();

				if (mToken->type == Token::Type::END)
				{
					mToken++;

					return node;
				}
				else
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_END);
					Utility::SafeDelete(node);
					return nullptr;
				}
			}
			else
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT);
				Utility::SafeDelete(node);
				return nullptr;
			}
		}
		else
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOR_STATEMENT);
			Utility::SafeDelete(node);
			return nullptr;
		}
	}

	ForEachNode *Parser::ForEach()
	{
		ForEachNode *node = new ForEachNode(mToken->location);

		mToken++;
		if (mToken->type == Token::Type::END_OF_FILE)
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT);
			Utility::SafeDelete(node);
			return nullptr;
		}

		node->variable = Expression();

		if (node->variable->type == Node::Type::IDENTIFIER || node->variable->type == Node::Type::MEMBER_REFERENCE || node->variable->type == Node::Type::INDEX_REFERENCE)
		{
			if (mToken->type == Token::Type::IN)
			{
				mToken++;
				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT);
					Utility::SafeDelete(node);
					return nullptr;
				}

				node->collection = Expression();

				if (node->collection->type != Node::Type::IDENTIFIER && node->collection->type != Node::Type::MEMBER_REFERENCE && node->collection->type != Node::Type::INDEX_REFERENCE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_LHS);
					Utility::SafeDelete(node);
					return nullptr;
				}

				if (mToken->type == Token::Type::DO)
				{
					mToken++;
				}

				if (mToken->type == Token::Type::END_OF_FILE)
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT);
					Utility::SafeDelete(node);
					return nullptr;
				}

				node->block = Block();

				if (mToken->type == Token::Type::END)
				{
					mToken++;

					return node;
				}
				else
				{
					ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_END);
					Utility::SafeDelete(node);
					return nullptr;
				}
			}
			else
			{
				ErrorHandler::OnError(mToken->location, ErrorCode::INCOMPLETE_FOREACH_STATEMENT);
				Utility::SafeDelete(node);
				return nullptr;
			}
		}
		else
		{
			ErrorHandler::OnError(mToken->location, ErrorCode::EXPECTED_LHS);
			Utility::SafeDelete(node);
			return nullptr;
		}
	}

	BreakNode *Parser::Break()
	{
		return new BreakNode(mToken++->location);
	}

	NextNode *Parser::Next()
	{
		return new NextNode(mToken++->location);
	}

	ReturnNode *Parser::Return()
	{
		Location tLocation = mToken->location;

		mToken++;

		if (mToken->type == Token::Type::END || mToken->type == Token::Type::ELSE || mToken->type == Token::Type::ELSEIF || mToken->type == Token::Type::PRIVATE || mToken->type == Token::Type::PUBLIC || mToken->type == Token::Type::WHEN || mToken->type == Token::Type::END_OF_FILE)
		{
			return new ReturnNode(tLocation);
		}
		else
		{
			return new ReturnNode(tLocation, Expression());
		}
	}
}