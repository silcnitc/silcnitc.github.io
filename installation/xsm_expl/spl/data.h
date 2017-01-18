/*
  Contains basic declarations for register etc.
*/

#ifndef DATA_H
#define DATA_H
#define R0		0
#define R1		1
#define R2		2
#define R3		3
#define R4		4
#define R5		5
#define R6		6
#define R7		7
#define R8		8
#define R9		9
#define R10		10
#define R11		11
#define R12		12
#define R13		13
#define R14		14
#define R15		15
#define R16		16
#define R17		17
#define R18		18
#define R19		19

#define P0		20
#define P1		21
#define P2		22
#define P3		23

#define BP_REG		24
#define IP_REG		25
#define SP_REG		26
#define PTBR_REG	27
#define PTLR_REG	28
#define EIP_REG		29
#define EPN_REG		30
#define EC_REG		31
#define EMA_REG		32


#define NO_GEN_REG		20
#define NO_PORTS		4
#define NO_SPECIAL_REG	9

#define NUM_REGS		(NO_GEN_REG + NO_SPECIAL_REG + NO_PORTS)

#define C_REG_BASE	    16 //Base address of registers reserved for compiler only use(16 because they start from R16)

int isAllowedRegister(int value);
#endif
