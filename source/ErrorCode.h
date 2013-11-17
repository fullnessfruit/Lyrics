#ifndef ERROR_CODE
#define ERROR_CODE

namespace lyrics
{
	enum class ErrorCode: unsigned int
	{
		WRONG_CHARACTER = 2001, STRING_NOT_TERMINATED,
		EXPECTED_RIGHT_PARENTHESIS = 3001, INCOMPLETE_ARRAY_LITERAL, EXPECTED_HASH_PAIR, INCOMPLETE_HASH_LITERAL, EXPECTED_PRIMARY_EXPRESSION, EXPECTED_INDEX, EXPECTED_PROCEDURE_CALL, EXPECTED_MEMBER, EXPECTED_LHS, EXPECTED_PARAMETER_NAME, INCOMPLETE_PROCEDURE, EXPECTED_PARAMETER, EXPECTED_PROCEDURE_NAME, EXPECTED_BASE_CLASS, EXPECTED_CLASS_NAME, EXPECTED_PACKAGE_NAME, EXPECTED_PACKAGE, EXPECTED_END, EXPECTED_END_ELSE_ELSEIF, EXPECTED_WHEN_ELSE_ELSEIF, EXPECTED_WHEN, INCOMPLETE_FOR, INCOMPLETE_FOR_FOR_EACH
	};
};

#endif