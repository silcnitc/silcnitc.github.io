%{
   /* Include files. */
   #include <stdlib.h>
   #include "machine.h"
   #include "lexer.h"

   YYSTYPE yylval;

   int
   machine_serve_instruction (char *buffer, yy_size_t *read_bytes, int max);

   void
   lexer_buffer_reset ()
   {
      YY_FLUSH_BUFFER;
   }

   #define YY_INPUT(buffer,read_bytes,max)\
   {\
      machine_serve_instruction(buffer,&read_bytes,max);\
   }\

%}
%%

-?[0-9]+ {
   yylval.val = atoi(yytext);
   return TOKEN_NUMBER;
}

\"[^"]* {
   /* Hack! */
   yylval.str = yytext + 1;
   yytext[yyleng] = '\0';
   return TOKEN_STRING;
}

, {
   return TOKEN_COMMA;
}

\[ {
   return TOKEN_DREF_L;
}

\] {
   return TOKEN_DREF_R;
}

SP|BP|IP|PTBR|PTLR|EIP|EC|EPN|EMA {
   yylval.str = yytext;
   return TOKEN_REGISTER;
}

P[0-3] {
   yylval.str = yytext;
   return TOKEN_REGISTER;
}

R[0-9]+ {
   yylval.str = yytext;
   return TOKEN_REGISTER;
}

[a-zA-Z]+ {
   yylval.str = yytext;
   return TOKEN_INSTRUCTION;
}

. ;

