#ifndef BYTE_CODE
#define BYTE_CODE

namespace lyrics
{
	struct ByteCode
	{
		enum class Opcode: unsigned short
		{
			NO_OPERATION,
			BREAK,
			SYSTEM_CALL,

			ADD,
			SUBTRACT,
			MULTIPLY,
			DIVIDE,
			ADD_IMMEDIATE,
			FLOATING_POINT_ADD,
			FLOATING_POINT_SUBTRACT,
			FLOATING_POINT_MULTIPLY,
			FLOATING_POINT_DIVIDE,

			LOAD_WORD,
			STORE_WORD,
			LOAD_WORD_IMMEDIATE,
			MOVE,
			
			NOT,
			NAND,
			NOR,
			AND,
			OR,
			XOR,
			AND_IMMEDIATE,
			OR_IMMEDIATE,
			SHIFT_LEFT,
			SHIFT_RIGHT,
			
			BRANCH_ON_EQUAL,
			BRANCH_ON_NOT_EQUAL,
			BRANCH_IF_LESS_THAN,
			BRANCH_IF_GREATER_THAN,
			BRANCH_IF_LESS_THAN_OR_EQUAL,
			BRANCH_IF_GREATER_THAN_OR_EQUAL,
			SET_ON_LESS_THAN,
			SET_ON_LESS_THAN_IMMEDIATE,

			JUMP,
			JUMP_REGISTER,
			CALL,

			NEW,
			CONSTRUCT_ARRAY,
			REFERENCE_ARRAY_ELEMENT,
			CONSTRUCT_HASH,
			REFERENCE_HASH_ELEMENT,
			CONSTRUCT_IMAGE,
			CONSTRUCT_TEXT,
			CONSTRUCT_SOUND,
			CONSTRUCT_VIDEO,
		};

		explicit ByteCode( Opcode opcode ) : opcode( opcode ), operand16( 0 ), operand32( 0 )
		{
		}

		ByteCode( Opcode opcode, short operand16 ) : opcode( opcode ), operand16( operand16 ), operand32( 0 )
		{
		}

		ByteCode( Opcode opcode, short operand16, long operand32 ) : opcode( opcode ), operand16( operand16 ), operand32( operand32 )
		{
		}

		ByteCode( Opcode opcode, short operand16, short operand32High, short operand32Low ) : opcode( opcode ), operand16( operand16 ), operand32( operand32High << 16 | ( operand32Low & 0x0000FFFFu ) )
		{
		}

		Opcode opcode;
		short operand16;
		long operand32;
	};

	enum struct Register : short { TV0, V0, TV1, V1, TA0, A0, TA1, A1, TT0, T0, TT1, T1, GP, SP, FP, RA };
}

#endif
