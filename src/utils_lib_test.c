#include "../my_shell.h"

int ligneNb=0; //for debuging

void rlg(){
ligneNb=0;
}

void lg(){
    printf("ligne %d\n",ligneNb++);
}

char *getPrompt()
{
    char* cwd = malloc(sizeof(char)*1000);
    getwd(cwd);
    int len=strlen(cwd);
    char* prompt = (char *)malloc(len+3);
    strcpy(prompt,cwd);
    prompt[len++]='%';
    prompt[len++]=' ';
    prompt[len++]='\0';
    free(cwd);

    return prompt;
}

void print_string_list(string_list* list){
    while (list!=NULL){
        printf("[%s]\n",list->string);
        list=list->next;
    }
}

