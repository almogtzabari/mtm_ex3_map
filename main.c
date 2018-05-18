#include <stdio.h>
#include <malloc.h>
#include <mem.h>
#include "map.h"
#include "node.h"

/* Testing Functions Declarations*/
void* numberCopy (void* number);
void* strCopy (void* string);
void numberDestroy (void* number);
void strDestroy (void* string);
int isBigger (void* number1, void* number2);
/*zoi*/
int main() {
    Map map=mapCreate(strCopy,numberCopy,strDestroy,numberDestroy,isBigger);
    int temp=4;
    int temp2=5;
    char stringtoCopy [20]= "sababa";
    char stringtoCopy2 [20]= "sababa egozim";
    mapPut(map,&temp,stringtoCopy);
    mapPut(map,&temp,stringtoCopy2);
    printf("%s",(char*)mapGet(map,&temp));
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