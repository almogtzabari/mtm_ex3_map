#include "node.h"
#include <malloc.h>
#include <assert.h>

struct node_t{
    MapKeyElement key;
    MapDataElement data;
    Node next;
};

Node NodeCreate(MapDataElement data, MapKeyElement key,
                copyMapDataElements copyDataElement,
                copyMapKeyElements copyKeyElement,
                freeMapDataElements freeDataElement,
                freeMapKeyElements freeKeyElement){
    Node new_node = malloc(sizeof(*new_node));
    if(!new_node){
        return NULL;
    }
    new_node->key = copyKeyElement(key);
    if(!new_node->key){
        free(new_node);
        return NULL;
    }
    new_node->data = copyDataElement(data);
    if(!new_node->data){
        free(new_node);
        freeKeyElement(new_node->key);
    }
    new_node->next = NULL;
    return new_node;
}

MapKeyElement nodeGetKey(Node node){
    assert(!node);
    /* todo: check if need to return a copy */
    return node->key;
}

MapDataElement nodeGetData(Node node,copyMapDataElements copyDataElement){
    assert(!node);
    if(!node->data){
        return NULL;
    }
    /* todo: check if need to return a copy */
    MapDataElement data_copy=copyDataElement(node->data);
    if(!data_copy){
        return NULL;
    }
    return data_copy;
}

Node nodeGetNext(Node node){
    /* todo: check if need to return a copy */
    return node->next;
}

NodeResult nodeSetNext(Node current, Node next){
    assert(!current);
    current->next = next;
}

NodeResult nodeSetData(Node node,MapDataElement data,copyMapDataElements copyDataElement){
    assert(!node);
    if (!data){
        return NODE_NULL_ARGUMENT;
    }
    MapDataElement data_copy =copyDataElement(data);
    if(!data_copy){
        return NODE_OUT_OF_MEMORY;
    }
    node->data=data_copy;
    return NODE_SUCCESS;
    //todo: what if data is NULL??
}
