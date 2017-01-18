#include "node.h"
#include <stdlib.h>
#include <string.h>

node* create_termNode(char nodetype, char *name, int value)
{
    node* tnode=malloc(sizeof(node));
    tnode->nodetype=nodetype;
    if(name!=NULL)
        tnode->name=strdup(name);
    tnode->value=value;
    tnode->entry=NULL;
    tnode->ptr1=NULL;
    tnode->ptr2=NULL;
    tnode->ptr3=NULL;
    return tnode;
}

node* create_nontermNode(char type, node *a, node *b)
{
    node *temp=malloc(sizeof(node));
    temp->nodetype=type;
    temp->name=NULL;
    temp->entry=NULL;
    temp->ptr1=a;
    temp->ptr2=b;
    temp->ptr3=NULL;
    return temp;
}

node * create_tree(node *a, node *b, node *c, node *d)
{    
    a->ptr1=b;
    a->ptr2=c;
    a->ptr3=d;
    
    return a;
}

char node_getType(node *nn)
{
    return nn->nodetype;
}

char* node_getName(node *nn)
{
    return nn->name;
}
