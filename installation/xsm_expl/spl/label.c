#include "label.h"
#include "y.tab.h"

extern int linecount;
static label *_root_label=NULL;
static int _namegen=1;

/*A stack containing labels of start and end while is needed to be stored, to generate appropiate labels for "break" and "continue"*/
static l_while *_rootWhile=NULL;

/*Create a new label with a random unused name*/
label* label_create()
{
    char name[30];
    sprintf(name,"_L%d",_namegen);
    _namegen++;
    return _label_create(name);
}

/*Create a new label with given name*/
label* _label_create(const char* name)
{
    label* new_label=(label*)malloc(sizeof(label));
    new_label->name=strdup(name);
    new_label->next=NULL;
    return new_label;
}

label* label_add(const char* name)
{
    if(label_get(name)!=NULL)
    {
        fprintf(stderr,"\n%d: Label '%s' redeclared.",linecount,name);
        exit(0);
    }
    label* new_label=_label_create(name);
    new_label->next=_root_label;
    _root_label=new_label;
    return new_label;
}

/*Get the label element with the name specified, if it exists, else return NULL*/
label* label_get(const char* name)
{
    label *temp=_root_label;
    while(temp!=NULL)
    {
        if(strcmp(temp->name, name)==0)
            return temp;
        temp=temp->next;
    }
    return NULL;
}

void label_free(label *ll)
{
    free(ll->name);
    free(ll);
}

/* Push the labels of a while block into a stack*/
void label_pushWhile(label *start, label *end)
{
    l_while *newl=(l_while*)malloc(sizeof(l_while));
    newl->start=start;
    newl->end=end;
    newl->next=_rootWhile;
    _rootWhile=newl;
}

void label_popWhile()
{
    l_while *temp=_rootWhile;
    _rootWhile=_rootWhile->next;
    free(temp);
}

/*Get the end label of the innermost while*/
label* label_getWhileEnd()
{
    return _rootWhile->end;   
}

/*Get the start label of the innermost while*/
label* label_getWhileStart()
{
    return _rootWhile->start;   
}


char* label_getName(label *ll)
{
    return ll->name;
}
