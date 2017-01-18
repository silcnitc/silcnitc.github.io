#include "tokenize.h"
#include "types.h"

static
YYSTYPE _curr_token;

static
int _la_exists;

static
int _curr_token_type;

int
tokenize_init ()
{
	_la_exists = FALSE;
	return XSM_SUCCESS;
}

int
tokenize_next_token (YYSTYPE *token_info)
{
	int token_type;

	if (_la_exists)
	{
		*token_info = _curr_token;
		_la_exists = FALSE;
		return _curr_token_type;
	}
	else
	{
		token_type = yylex ();
		*token_info = yylval;
		return token_type;
	}
}

/* Skips the next token. */
int
tokenize_skip_token()
{
	YYSTYPE token_info;

	return tokenize_next_token(&token_info);
}

int
tokenize_peek (YYSTYPE *token_info)
{
	if (_la_exists)
	{
		*token_info = _curr_token;
		return _curr_token_type;
	}
	else
	{
		_curr_token_type = yylex ();
		_curr_token = yylval;
		*token_info = _curr_token;
		_la_exists = TRUE;
		return _curr_token_type;
	}
}

int
tokenize_close ()
{
	/* Nothing to do. */
	return XSM_SUCCESS;
}

void
tokenize_reset ()
{
	_la_exists = FALSE;
}

void
tokenize_clear_stream ()
{
	lexer_buffer_reset ();
}