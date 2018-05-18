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
 * Description: Gets a node in the map and returns node's key.
 * @param node - a pointer to a node.
 * @return - Node's key element.
 */
MapKeyElement nodeGetKey(Node node){
    if(!node){
        return NULL;
    }
    return node->key;
}

/**
 ***** Function: nodeGetData *****
 * Description: Gets a node and a copy data function and returns a copy of
 * node's data element.
 * Notice: User is responsible for destroying the data copy.
 * @param node - a pointer to a node.
 * @param copyDataElement - a pointer to a copy function of the data.
 * @return - A pointer to a copy of node's data.
 */
MapDataElement nodeGetData(Node node,copyMapDataElements copyDataElement){
    if(!node){
        /* Node is NULL. */
        return NULL;
    }
    if(!(node->data)){
        /* Node data is NULL. */
        return NULL;
    }
    /* Creating a copy of the node's data. */
    MapDataElement data_copy = copyDataElement(node->data);
    if(!data_copy){
        /* Failed to create a copy. */
        return NULL;
    }
    /* Copy succeeded. */
    return data_copy;
}

/**
 ***** Function: nodeGetNext *****
 * Descritpion: Returns the next node (the node after the
 * given node).
 * @param map - a pointer to a map.
 * @param node - a pointer to a node.
 * @return - Pointer to the node after the given node.
 */
Node nodeGetNext(Node node){
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
        /* Node is NULL. */
        return NODE_NULL_ARGUMENT;
    }
    node->next = next_node;
    return NODE_SUCCESS;
}

/**
 ***** Function: nodeSetData *****
 * Description: The function gets a node and a new data and creates a copy
 * of the data. The copy of the new data will be inserted to node's data.
 * Notice: Node's old data will be destroyed.
 * @param node - a pointer to the node which we want to modify its data.
 * @param new_data - a pointer to the new data.
 * @param copyDataElement - a pointer to the copy data function.
 * @return
 */
NodeResult nodeSetData(Node node, MapDataElement new_data,
                       copyMapDataElements copyDataElement,
                       freeMapDataElements freeDataElement){
    assert(!node);
    if (!new_data){
        /* New data is NULL. */
        return NODE_NULL_ARGUMENT;
    }
    /* Creating a copy of the new Data*/
    MapDataElement data_copy = copyDataElement(new_data);
    if(!data_copy){
        /* Couldn't create the data copy. */
        return NODE_OUT_OF_MEMORY;
    }
    /* New data copy created successfully. */
    freeDataElement(node->data); // Destroying old data.
    node->data = data_copy;
    return NODE_SUCCESS;
}

/**
 ***** Function: nodeCopy *****
 * Description: Gets a node and freeing&copying functions of key and data,
 * and returns a copy of that node.
 * Notice: The copy of the node has the same key&data but it's next is set
 * to NULL! Also, if the function fails a NULL pointer will be returned.
 * @param node - a pointer to the node we want to copy.
 * @param copyDataElement - data copy function.
 * @param copyKeyElement - key copy function.
 * @param freeDataElement - data destroy function.
 * @param freeKeyElement - key destroy function.
 * @return - A copy of the given node.
 */
Node nodeCopy(Node node, copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement){
    /* Creating a new node with the same key&data. */
    Node node_copy = nodeCreate(node->data,node->key, copyDataElement,
            copyKeyElement,freeDataElement,freeKeyElement);
    return node_copy;

}

