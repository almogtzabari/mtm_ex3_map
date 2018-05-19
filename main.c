#include <stdio.h>
#include <malloc.h>
#include <mem.h>
#include "map_mtm.h"
#include "node.h"

/* Testing Functions Declarations*/
void* numberCopy (void* number);
void* strCopy (void* string);
void numberDestroy (void* number);
void strDestroy (void* string);
int isBigger (void* number1, void* number2);

int main() {
    return 0;
}

/* Testing Functions*/

void* numberCopy (void* number){
    int* temp=malloc(sizeof(*temp));
    if(!temp){
        return NULL;
    }
    *temp=*(int*)number;
    return temp;
}

void* strCopy (void* string){
    char* temp=malloc(sizeof(strlen(string))+1);
    strcpy(temp,(char*)string);
    return temp;
}

void numberDestroy (void* number){
    free(number);
}

void strDestroy (void* string){
    free(string);
}

int isBigger (void* number1, void* number2){
    if(*(int*)number1>*(int*)number2){
        return 1;
    }
    if(*(int*)number1==*(int*)number2){
        return 0;
    }
    return -1;
}