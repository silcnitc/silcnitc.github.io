#ifndef _XSM_TOKENIZE_H

#define _XSM_TOKENIZE_H

/* Include lexer header. */
#include "lexer.h"

int
tokenize_init ();

int
tokenize_next_token (YYSTYPE *token_info);

int
tokenize_peek (YYSTYPE *token_info);

int
tokenize_skip_token();

int
tokenize_close ();

void
tokenize_reset ();

void
tokenize_clear_stream ();

/* Clang rigorously follows C99 standard. */
int
yylex();

#endif