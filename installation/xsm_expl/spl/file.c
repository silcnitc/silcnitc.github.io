#include "file.h"

void file_expandPath(char *path) // To expand environment variables in path
{
    char *rem_path = strdup(path);
    char *token = strsep(&rem_path, "/");
    if(rem_path!=NULL)
        sprintf(path,"%s/%s",getenv(++token)!=NULL?getenv(token):token-1,rem_path);
    else
        sprintf(path,"%s",getenv(++token)!=NULL?getenv(token):token-1);
}

void file_remExtension(char *pathname)
{
    int l = strlen(pathname);
    int i = l-1;    
    while(pathname[i] != '.' && i>=0)
    {
        i--;
    }
    pathname[i+1]='\0';    
}

void file_getOpFileName(char *dest, const char* inpfname)
{
    strcpy(dest,inpfname);
    file_remExtension(dest);
    strcat(dest,"xsm");
}
