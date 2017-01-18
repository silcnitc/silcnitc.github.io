#ifndef _XSM_LEXER_H

#define _XSM_LEXER_H

#define TOKEN_REGISTER 1
#define TOKEN_NUMBER 2
#define TOKEN_STRING 3
#define TOKEN_DREF_L 4
#define TOKEN_DREF_R 5
#define TOKEN_INSTRUCTION 6
#define TOKEN_COMMA 7

typedef union YYSTYPE
{
	int val;
	char *str;
}
YYSTYPE;

extern YYSTYPE yylval;

void
lexer_buffer_reset ();

#endif
