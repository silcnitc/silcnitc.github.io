Program : TypeDefBlock GDeclBlock FDefBlock MainBlock { evaluate($4); }
    ;

TypeDefBlock : TYPE TypeDefList ENDTYPE
    |                                               {}
    ;

TypeDefList: TypeDefList TypeDef
    |TypeDef
    ;

TypeDef: ID '{' FieldDeclList '}' { Tinstall($1,$3); }
    ;


FieldDeclList : FieldDeclList ';' FieldDecl
    |   FieldDecl
    ;

FieldDecl : TypeName ID     ;

TypeName : INT
    | STR
    | ID
    ;

GDeclBlock : DECL GDeclList ENDDECL
    |
    ;

GDeclList : GDecList GDecl
    | GDecl

GDecl : TypeName Gidlist ';'      ;

Gidlist : Gidlist ',' Gid
    |   Gid                     ;

Gid :   ID                      { GInstall(varname,ttableptr, 1, NULL); }
    |   ID '(' ParamList ')'      { GInstall(varname,ttableptr, 0,$3); }
    |   ID '[' NUM ']'          { GInstall(varname,ttableptr, $3,NULL); }
    ;

ParamList :  ParamList Arg  { AppendArglist($1,$2);}
        |  Param

Param : TypeName ID { CreateArglist($1,$2); }


FDefBlock : TypeName ID '(' ArgList ')' '{' LdeclBlock Body '}'
                                {
                                    GUpdate($2->name,$1,$4,$7,$8);
                                }                                }
    ;

MainBlock : INT MAIN '(' ')' '{' LdeclBlock Body '}'
                                {
                                    type = TLookup("int");
                                    gtemp = GInstall("MAIN",type,0,NULL);
                                    gtemp->LocalVariables = $6;
                                    gtemp->fbinding = $7;
                                    $$ = TreeCreate(type, NODETYPE_MAIN, gtemp->name, (Constant){}, NULL, $7, NULL, NULL);
                                    $$->Gentry = gtemp;
                                    //reset LocalSymbolHead and other binding variables used
                                }
    ;

body : BEGIN slist retstmt END { $$ = TreeCreate($3->type, NODETYPE_BODY, NULL, (union Constant){}, NULL, $2, $3, NULL); }
    ;

slist : slist stmt  { $$ = TreeCreate(TLookup("void"), NODETYPE_SLIST, NULL,(union Constant){}, NULL, $1, $2, NULL); }
    |               { $$ = NULL; }
    ;

stmt : ID ASGN expr ';' {
                            ltemp = LLookup($1->name);
                            if(ltemp == NULL){
                                gtemp = GLookup($1->name);
                                if(gtemp == NULL){
                                    yyerror("Undeclared variable is used");exit(1);
                                }
                                $1->Gentry = gtemp;
                                $1->type = gtemp->type;
                            }
                            else{
                                $1->Lentry = ltemp;
                                $1->type = ltemp->type;
                            }
                            $$ = TreeCreate(TLookup("void"),NODETYPE_ASGN,NULL,(union Constant){},NULL,$1,$3,NULL);
                        }
    | ....
    .
    .
    | IF '(' expr ')' THEN slist ELSE slist ENDIF { $$ = TreeCreate(TLookup("void"),NODETYPE_IF,NULL,(union Constant){},NULL,$3,$6,$8); }
    | ...
    .
    .
    | ID ASGN ALLOC'(' ')'      {}
    | FIELD ASGN ALLOC'(' ')'   {}
    | FREE '(' ID ')'           {}
    | FREE '(' FIELD ')'        {}
    |...
    ;

FIELD : ID '.' ID   { $$ = TreeCreate(TLookUp("void"), NODETYPE_FIELD, NULL, (Constant){}, NULL, $1, $3, NULL); }
    | FIELD '.' ID  { $$ = TreeCreate(TLookUp("void"), NODETYPE_FIELD, NULL, (Constant){}, NULL, $1, $3, NULL); }
    ;

expr : expr PLUS expr   { $$ = TreeCreate(TLookup("int"),NODETYPE_PLUS,NULL,(union Constant){},NULL,$1,$3,NULL); }
    | ....
    | '(' expr ')'      { $$ = $2; }
    | NUM               { $$ = $1; }
    | ID                {
                            ltemp = LLookup($1->name);
                            if(ltemp == NULL){
                                gtemp = GLookup($1->name);
                                if(gtemp == NULL){
                                    yyerror("Undeclared variable is used");exit(1);
                                }
                                $1->Gentry = gtemp;
                                $1->type = gtemp->type;
                            }
                            else{
                                $1->Lentry = ltemp;
                                $1->type = ltemp->type;

                            }
                            $$ = $1;
    					}

    | ID '[' expr ']'   { $$ = TreeCreate(NULL, NODETYPE_ARR_ID, NULL, (Constant){}, NULL, $1, $3, NULL); }
    | FIELD             { $$ = $1; }
    | ID '(' param ')'  {
                            gtemp = GLookup($1->name);
                            if(gtemp == NULL){
                            	yyerror("Yacc : Undefined function");exit(1);
                            }
                            $$ = TreeCreate(gtemp->type,NODETYPE_FUNCTION,$1->name,(Constant){},$3,NULL,NULL,NULL);
                            $$->Gentry = gtemp;
                        }
    ;
