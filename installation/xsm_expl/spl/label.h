
#ifndef LABEL_H
#define LABEL_H
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
typedef struct labelt
{
    char *name;
    struct labelt *next;
}label;

typedef struct l_whilet
{
    label *start,*end;
    struct l_whilet *next;
}l_while;
 
label* label_create();
label* label_add(const char* name);
label* _label_create(const char* name);
label* label_get(const char* name);
char* label_getName(label *ll);
void label_pushWhile(label *start, label *end);
void label_popWhile();
label* label_getWhileEnd();
label* label_getWhileStart();
#endif
