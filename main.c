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
    int key1 = 2; char data1[7] = "hello2";
    int key2 = 40; char data2[8] = "hello40";
    int key3 = 1; char data3[7] = "hello1";
    mapPut(map,&key1,data1);
    mapPut(map,&key2,data2);
    mapPut(map,&key3,data3);
    printf("Print by order (low to high):\n");
    printf("First key is:%d\n",*(int*)mapGetFirst(map));
    printf("First data is:%s\n",(char*)mapGet(map,mapGetFirst(map)));
    int next = *(int*)mapGetNext(map);
    printf("Second key is:%d\n",next);
    printf("Second data is:%s\n",(char*)mapGet(map,&next));
    printf("Map size:%d\n",mapGetSize(map));
    Map new_map = mapCopy(map);
    printf("New map size:%d",mapGetSize(new_map));
    printf("Print by order (low to high):\n");
    printf("First key is:%d\n",*(int*)mapGetFirst(map));
    printf("First data is:%s\n",(char*)mapGet(map,mapGetFirst(map)));
    int next = *(int*)mapGetNext(map);
    printf("Second key is:%d\n",next);
    printf("Second data is:%s\n",(char*)mapGet(map,&next));
    printf("Map size:%d\n",mapGetSize(map));


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