%{
#include<stdio.h>
#include<stdlib.h>
#include "data.h"
#include "spl.h"
#include "file.h"
#include "node.h"
%}
%union
{
    struct tree *n;
}
%token ALIAS DEFINE DO ELSE ENDIF ENDWHILE IF RETURN IRETURN LOAD  STORE THEN WHILE HALT REG NUM ASSIGNOP ARITHOP1 ARITHOP2 RELOP LOGOP NOTOP ID BREAK CONTINUE CHKPT READ READI PRINT STRING INLINE BACKUP RESTORE LOADI  GOTO CALL ENCRYPT PORT
%type<n> IF RETURN IRETURN LOAD STORE WHILE HALT REG NUM ASSIGNOP ARITHOP1 ARITHOP2 RELOP LOGOP NOTOP ID stmtlist stmt expr ids ifpad whilepad BREAK CONTINUE CHKPT READ READI PRINT STRING INLINE BACKUP RESTORE LOADI  GOTO CALL ENCRYPT PORT
%left LOGOP
%left RELOP  
%left ARITHOP1      // + and -
%left ARITHOP2      // * , / and %
%right NOTOP        // NOT Operator
%left UMIN          // unary minus

%%
body:   definelistpad stmtlist  {
                                    codegen($2);
                                    out_linecount++;
                                    fprintf(fp,"HALT");
                                }
        ;

definelistpad:  definelist      {
                                    add_predefined_constants();
                                }
                ;
        
definelist:     definelist definestmt    
                |            
                ;

definestmt:     DEFINE ID NUM ';'               {
                                                    insert_constant($2->name,$3->value);
                                                }
                |DEFINE ID ARITHOP1 NUM ';'     {
                                                    if(node_getType($3)==NODE_SUB)
                                                        insert_constant($2->name,-1*$4->value);
                                                    else
                                                        insert_constant($2->name,$4->value);
                                                }
                ;

stmtlist:       stmtlist stmt                   {
                                                    $$=create_nontermNode(NODE_STMTLIST,$1,$2);
                                                }
                |stmt                           {
                                                    $$=$1;
                                                }
                ;

stmt:           expr ASSIGNOP expr ';'          {
                                                    if(node_getType($1)==NODE_REG || node_getType($1)==NODE_ADDR_EXPR)
                                                    {
                                                        $2->value=2;
                                                        $$=create_tree($2,$1,$3,NULL);
                                                    }
                                                    else
                                                    {
                                                        printf("\n%d:Invalid operands in assignment!!\n",linecount);
                                                        exit(0);
                                                    }
                                                }
                |expr ASSIGNOP PORT ';'         {
                                                    if(node_getType($1)==NODE_REG || node_getType($1)==NODE_ADDR_EXPR)
                                                    {
                                                        $2->value=2;
                                                        $$=create_tree($2,$1,$3,NULL);
                                                    }
                                                    else
                                                    {
                                                        printf("\n%d:Invalid operands in assignment!!\n",linecount);
                                                        exit(0);
                                                    }
                                                }
                |ifpad expr THEN stmtlist ENDIF ';'     {                                
                                                            $$=create_tree($1,$2,$4,NULL);
                                                            pop_alias();
                                                            depth--;
                                                        }                                
                |ifpad expr THEN stmtlist
                    elsepad stmtlist ENDIF ';'          {    
                                                            $$=create_tree($1,$2,$4,$6);
                                                            pop_alias();
                                                            depth--;
                                                        }
                |whilepad expr DO stmtlist ENDWHILE ';' {
                                                            $$=create_tree($1,$2,$4,NULL);
                                                            pop_alias();
                                                            depth--;
                                                            flag_break--;
                                                        }
                |ALIAS ID REG ';'                       {    
                                                            push_alias($2->name,$3->value);
                                                            $$=NULL;
                                                        }
                |LOAD '(' expr ',' expr ')' ';'         {
                                                            $$=create_tree($1,$3,$5,NULL);
                                                        }
                |STORE '(' expr ',' expr ')'    ';'     {
                                                            $$=create_tree($1,$3,$5,NULL);
                                                        }
                |LOADI '(' expr ',' expr ')' ';'        {
                                                            $$=create_tree($1,$3,$5,NULL);
                                                        }
                |RETURN ';'                            {
                                                            $$=$1;
                                                        }
                |IRETURN ';'                            {
                                                            $$=$1;
                                                        }
                |BREAK ';'                              {
                                                            if(flag_break==0)
                                                            {
                                                                printf("\n%d: break or continue should be used inside while!!\n",linecount);
                                                                exit(0);                                
                                                            }
                                                            $$=$1;
                                                        }
                |CONTINUE ';'               {
                                                if(flag_break==0)
                                                {
                                                    printf("\n%d: break or continue should be used inside while!!\n",linecount);
                                                    exit(0);                                
                                                }
                                                $$=$1;
                                            }
                |HALT ';'                   {    
                                                $$=$1;
                                            }
                |INLINE STRING ';'          {
                                                $2->name++;
                                                int temp=strlen($2->name);
                                                $2->name[temp-1]='\0';
                                                $$=create_tree($1,$2,NULL,NULL);
                                            }
                |CHKPT ';'                  {    
                                                $$=$1;
                                            }
                |READ ';'                   {
                                                $$=$1;
                                            }
                |READI ids ';'              {    
                                                if(node_getType($2)!=NODE_REG||!isAllowedRegister($2->value))
                                                {
                                                    printf("\n%d:Invalid operand in read!!\n",linecount);
                                                    exit(0);
                                                }                            
                                                $$=create_tree($1,$2,NULL,NULL);
                                            }
                |PRINT expr ';'             {
                                                $$=create_tree($1,$2,NULL,NULL);
                                            }
                |BACKUP ';'     {
                                                $$ = create_tree($1, NULL, NULL, NULL);
                                            }
                |RESTORE ';'    {
                                                $$ = create_tree($1, NULL, NULL, NULL);
                                            }
                |ENCRYPT ids ';'            {    
                                                if(node_getType($2)!=NODE_REG||!isAllowedRegister($2->value))
                                                {
                                                    printf("\n%d:Invalid operand in encrypt!!\n",linecount);
                                                    exit(0);
                                                }                            
                                                $$=create_tree($1,$2,NULL,NULL);
                                            }
                |GOTO ID ';'                {
                                                if(lookup_constant(node_getName($2))!=NULL)/*if the address to jump to is a predefined value in constants file*/
                                                {
                                                    $$=create_tree($1,substitute_id($2),NULL,NULL);
                                                }
                                                else
                                                {
                                                    $$=create_tree($1,$2,NULL,NULL);
                                                }
                                                $$->value=linecount;/*Hack to show line numbers in case of syntax errors*/
                                            }
                |CALL ID ';'                {
                                                if(lookup_constant(node_getName($2))!=NULL)/*if the address to jump to is a predefined value in constants file*/
                                                {
                                                    $$=create_tree($1,substitute_id($2),NULL,NULL);
                                                }
                                                else
                                                {
                                                    $$=create_tree($1,$2,NULL,NULL);
                                                }
                                                $$->value=linecount;/*Hack to show line numbers in case of syntax errors*/
                                            }
                |ID ':'                     {
                                                label_add(node_getName($1));
                                                $$=create_nontermNode(NODE_LABEL_DEF,$1,NULL);
                                            }
                ;
    
                
expr:           expr ARITHOP1 expr          {
                                                $$=create_tree($2,$1,$3,NULL);
                                            }
                |expr ARITHOP2 expr         {
                                                $$=create_tree($2,$1,$3,NULL);
                                            }
                |expr RELOP expr            {
                                                $$=create_tree($2,$1,$3,NULL);
                                            }
                |expr LOGOP expr            {
                                                $$=create_tree($2,$1,$3,NULL);
                                            }
                |ARITHOP1 NUM   %prec UMIN  {
                                                if(node_getType($1)==NODE_SUB)
                                                    $2->value=$2->value*-1;
                                                $$=$2;
                                            }
                |NOTOP expr                 {
                                                $$=create_tree($1,$2,NULL,NULL);
                                            }
                |'['expr']'                 {
                                                $$=create_nontermNode(NODE_ADDR_EXPR,$2,NULL);
                                            }                    
                |'('expr')'                 {
                                                $$=$2;
                                            }
                |NUM                        {    
                                                $$=$1;
                                            }
                |ids                        {
                                                $$=$1;
                                            }
                |STRING                     {
                                                $$=$1;
                                            }
                ;

ifpad:          IF                          {
                                                depth++;
                                                $$=$1;
                                            }
                ;
elsepad:        ELSE                        {
                                                pop_alias();
                                            }
                ;

whilepad:       WHILE                       {
                                                depth++;
                                                flag_break++;
                                                $$=$1;
                                            }
                ;

ids:            ID                          {                            
                                                $$=substitute_id($1);
                                            }
                |REG                        {
                                                $$=$1;
                                            }
                ;
%%

int yyerror (char *msg) 
{
    return fprintf (stderr, "%d: %s\n",linecount,msg);
}
