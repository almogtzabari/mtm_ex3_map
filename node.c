#include "node.h"
#include <malloc.h>
#include <assert.h>

//-----------------------------------------------------------------------//
//                           STRUCT NODE                                 //
//-----------------------------------------------------------------------//

struct node_t{
    MapKeyElement key;
    MapDataElement data;
    Node next;
};

//-----------------------------------------------------------------------//
//                        STRUCT NODE FUNCTIONS                          //
//-----------------------------------------------------------------------//

Node nodeCreate(MapDataElement data, MapKeyElement key,
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

/**
 ***** Function: nodeDestroy *****
 * Description: Frees all allocated memory of the given node.
 * @param node - a pointer to a node.
 * @param freeDataElement - a pointer to a data destroy function.
 * @param freeKeyElement - a pointer to a key destroy function.
 * @return
 */
void nodeDestroy(Node node, freeMapDataElements freeDataElement,
                       freeMapKeyElements freeKeyElement){
    freeDataElement(node->data);
    freeKeyElement(node->key);
    free(node);
}

/**
 ***** Function: nodeGetKey *****
 * Description: Gets a node in the map and returns a copy of its key.
 * Notice: The user is responsible for destroying the key.
 * @param node - a pointer to a node.
 * @param copyKeyElement - a pointer to a copy function of key element.
 * @return - A copy of the node's key element.
 */
MapKeyElement nodeGetKey(Node node, copyMapKeyElements copyKeyElement){
    if(!node){
        return NULL;
    }
    MapKeyElement key_copy = copyKeyElement(node->key);
    return key_copy;
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

/**
 ***** Function: nodeGetNext *****
 * Descritpion: Returns the a copy of the next node after given node.
 * Notice: The user is responsible for destroying the copy.
 * @param map - a pointer to a map.
 * @param node - a pointer to a node.
 * @return - Pointer to a copy of the node after the given node.
 */
Node nodeGetNext(Map map, Node node){
    /* todo: check if need to return a copy */
    Node next_node =
    return node->next;
}

/**
 ***** Function: nodeSetNext *****
 * Description: Gets node and next_node and sets node's 'next'
 * to be next_node.
 * @param node
 * @param next_node
 * @return - Status success/failure of function.
 */
NodeResult nodeSetNext(Node node, Node next_node){
    if(!node){
        return NODE_NULL_ARGUMENT;
    }
    node->next = next_node;
    return NODE_SUCCESS;
}

/**
 ***** Function: nodeSetData *****
 * Description:
 * @param node
 * @param data
 * @param copyDataElement
 * @return
 */
NodeResult nodeSetData(Node node,MapDataElement data,
                       copyMapDataElements copyDataElement){
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

