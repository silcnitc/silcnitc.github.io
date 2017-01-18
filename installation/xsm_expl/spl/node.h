#ifndef NODE_H
#define NODE_H

//TODO assigning values to these defines(preferably after sorting by category)
#define NODE_IF     0
#define NODE_LOAD   1
#define NODE_STORE  2
#define NODE_LOADI  3
#define NODE_READ   4
#define NODE_READI  5
#define NODE_PRINT  6
#define NODE_REG    7
#define NODE_NUM    8
#define NODE_STRING 9
#define NODE_IDENT  10
#define NODE_NONTERM    11
#define NODE_STRCMP     12
#define NODE_STRCOPY    13 
#define NODE_WHILE      14
#define NODE_EQ         15
#define NODE_GT         16
#define NODE_LT         17
#define NODE_LE         18
#define NODE_GE         19
#define NODE_NE         20
#define NODE_AND        21
#define NODE_OR         22
#define NODE_NOT        23
#define NODE_BREAK      24
#define NODE_CONTINUE   25
#define NODE_ADDR_EXPR  26
#define NODE_HALT       27
#define NODE_BREAKPOINT 28
#define NODE_RETURN     29
#define NODE_IRETURN    30
#define NODE_INLINE     31
#define NODE_ENCRYPT    32
#define NODE_STMTLIST   33
#define NODE_ADD        34
#define NODE_SUB        35
#define NODE_MUL        36
#define NODE_DIV        37
#define NODE_MOD        38
#define NODE_ASSIGN     39
#define NODE_BACKUP     40
#define NODE_RESTORE    41
#define NODE_GOTO       42
#define NODE_CALL       43
#define NODE_PORT       44
#define NODE_LABEL_DEF  45

struct define
{
    char name[30];
    int value;
    struct define *next;
};

typedef struct tree
{
    char nodetype;        
                   
    char *name;
    int value;
    struct define *entry;
    struct tree *ptr1,*ptr2,*ptr3;
}node;

node* create_termNode(char nodetype, char *name, int value);    //nodetype,name,value
char node_getType(node *nn);
char* node_getName(node *nn);
node* create_nontermNode(char nodetype, node *a, node *b);
node * create_tree(node *a, node *b, node *c, node *d);
#endif
