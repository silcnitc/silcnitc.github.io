%{
#include "y.tab.h"
#include "data.h"
#include "node.h"
int linecount=1;
void create_node(char nodetype, char *name, int value);
%}
%option noyywrap
%%


"alias"     {
                return ALIAS;
            }

"define"    {
                return DEFINE;
            }

"encrypt"   {
                create_node(NODE_ENCRYPT,NULL,-1);
                return ENCRYPT;
            }

"breakpoint" {
                create_node(NODE_BREAKPOINT,NULL,-1);
                return CHKPT;
            }

"inline"    {
                create_node(NODE_INLINE,NULL,-1);
                return INLINE;
            }

"halt"      {
                create_node(NODE_HALT,NULL,-1);
                return HALT;
            }

"goto"      {
                create_node(NODE_GOTO,NULL,-1);
                return GOTO;
            }

"call"      {
                create_node( NODE_CALL,NULL,-1);
                return CALL;
            }

"return"    {
                create_node(NODE_RETURN,NULL,-1);
                return RETURN;
            }            

"ireturn"   {
                create_node(NODE_IRETURN,NULL,-1);
                return IRETURN;
            }

"backup"    {
                create_node(NODE_BACKUP,NULL,-1);
                return BACKUP;
            }

"restore"   {
                create_node(NODE_RESTORE,NULL,-1);
                return RESTORE;
            }

"read"      {    
                create_node(NODE_READ,NULL,-1);
                return READ;
            }

"readi"     {    
                create_node(NODE_READI,NULL,-1);
                return READI;
            }

"print"     {
                create_node(NODE_PRINT,NULL,-1);
                return PRINT;
            }

"load"      {
                create_node(NODE_LOAD,NULL,-1);
                return LOAD;
            }
            
"loadi"     {
                create_node(NODE_LOADI,NULL,-1);
                return LOADI;
            }

"store"     {
                create_node(NODE_STORE,NULL,-1);
                return STORE;
            }

"while"     {
                create_node(NODE_WHILE,NULL,-1);
                return WHILE;
            }

"do"        {
                return DO;
            }
    
"endwhile"  {
                return ENDWHILE;
            }

"break"     {
                create_node(NODE_BREAK,NULL,-1);
                return BREAK;
            }    

"continue"  {
                create_node(NODE_CONTINUE,NULL,-1);
                return CONTINUE;
            }            
                            
"if"        {
                create_node(NODE_IF,NULL,-1);
                return IF;
            }    

"then"      {
                return THEN;
            }

"else"      {
                return ELSE;
            }

"endif"     {
                return ENDIF;        
            }
            
[R]([0-9])  {
                create_node(NODE_REG,NULL,atoi(yytext+1) + R0);
                return REG;
            }    

(R1)([0-5]) {
                create_node(NODE_REG,NULL,atoi(yytext+1) + R0);
                return REG;
            }
            
[P]([0-3])  {
                create_node(NODE_PORT,NULL,atoi(yytext+1) + P0);
                return PORT;
            }        

"BP"        {
                create_node(NODE_REG,NULL,BP_REG);
                return REG;
            }

"SP"        {
                create_node(NODE_REG,NULL,SP_REG);
                return REG;
            }

"IP"        {
                create_node(NODE_REG,NULL,IP_REG);
                return REG;
            }

"PTBR"      {
                create_node(NODE_REG,NULL,PTBR_REG);
                return REG;
            }

"PTLR"      {
                create_node(NODE_REG,NULL,PTLR_REG);
                return REG;
            }

"EIP"       {
                create_node(NODE_REG,NULL,EIP_REG);
                return REG;
            }

"EC"        {
                create_node(NODE_REG,NULL,EC_REG);
                return REG;
            }
            
"EPN"       {
                create_node(NODE_REG,NULL,EPN_REG);
                return REG;
            }

"EMA"       {
                create_node(NODE_REG,NULL,EMA_REG);
                return REG;
            }
        
(==)        {
                create_node(NODE_EQ,NULL,-1);
                return RELOP;
            }
            
(<=)        {
                create_node(NODE_LE,NULL,-1);
                return RELOP;
            }
            
(>=)        {
                create_node(NODE_GE,NULL,-1);
                return RELOP;
            }
            
"!="        {
                create_node(NODE_NE,NULL,-1);
                return RELOP;
            }

            
"&&"        {
                create_node(NODE_AND,NULL,-1);
                return LOGOP;
            }

"||"        {
                create_node(NODE_OR,NULL,-1);
                return LOGOP;
            }
            
[!]         {
                create_node(NODE_NOT,NULL,-1);
                return NOTOP;
            }

[+]         {
                create_node(NODE_ADD,NULL,-1);
                return ARITHOP1;            
            }
            
[-]         {
                create_node(NODE_SUB,NULL,-1);
                return ARITHOP1;            
            }
            
[*]         {
                create_node(NODE_MUL,NULL,-1);
                return ARITHOP2;            
            }
            
[/]         {
                create_node(NODE_DIV,NULL,-1);
                return ARITHOP2;            
            }
            
[%]         {
                create_node(NODE_MOD,NULL,-1);
                return ARITHOP2;            
            }
            
[<]         {
                create_node(NODE_LT,NULL,-1);
                return RELOP;            
            }
            
[>]         {
                create_node(NODE_GT,NULL,-1);
                return RELOP;            
            }

[=]         {
                create_node(NODE_ASSIGN,NULL,-1);
                return ASSIGNOP;
            }
                        
[0-9]+      {
                create_node(NODE_NUM,NULL,atoi(yytext));
                return NUM;
            }

[\"][^\"]*[\"]        {    
                create_node(NODE_STRING,yytext,-1);
                return STRING;
            }
                    
[a-zA-Z][a-zA-Z0-9_]*    {
                create_node(NODE_IDENT,yytext,-1);
                return ID;
            }    

[(),:;\[\]\{\}&]        {    
                return *yytext;
            }

[/][/].*\n        {
                linecount++;
            }

[\n]        {    
                linecount++;
            }

[ \t]       {
            }

.           {
            }

%%

void create_node(char nodetype, char *name, int value)
{    
    yylval.n=create_termNode(nodetype, name, value);
}
