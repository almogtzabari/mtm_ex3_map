#include "node.h"
#include <malloc.h>
#include <assert.h>
#include <stdio.h>

//-----------------------------------------------------------------------//
//                           STRUCT NODE                                 //
//-----------------------------------------------------------------------//

struct node_t{
    NodeKeyElement key;
    NodeDataElement data;
    Node next;
};

//-----------------------------------------------------------------------//
//                        STRUCT NODE FUNCTIONS                          //
//-----------------------------------------------------------------------//

/**
 ***** Function: nodeCreate *****
 * Description: Creates a new node.
 * @param data - The data element which need to be assigned to the new
 * node.
 * @param key - The key element which need to be assigned to the new node.
 * @param copyDataElement - Function pointer to be used for copying data
 * elements into the node.
 * @param copyKeyElement - Function pointer to be used for copying key
 * elements into the node.
 * @param freeKeyElement - Function pointer to be used for removing key
 * element from the node. This free function will be used in case of
 * memory allocation fail of the data copy function.
 * @return - The new node.
 */
Node nodeCreate(NodeDataElement data, NodeKeyElement key,
                copyNodeDataElements copyDataElement,
                copyNodeKeyElements copyKeyElement,
                freeNodeKeyElements freeKeyElement){
    Node new_node = malloc(sizeof(*new_node));
    if(!new_node){
        /* Failed to allocate memory to node. */
        return NULL;
    }
    new_node->key = copyKeyElement(key);
    if(!new_node->key){
        /* Failed to copy key. */
        free(new_node);
        return NULL;
    }
    new_node->data = copyDataElement(data);
    if(!new_node->data){
        /* Failed to copy data. */
        freeKeyElement(new_node->key);
        free(new_node);
    }
    new_node->next = NULL;
    return new_node;
}

/**
 ***** Function: nodeDestroy *****
 * Description: Frees all allocated memory of the given node.
 * @param node - The node we want to destroy.
 * @param freeDataElement - Function pointer to be used for removing data
 * element from the node.
 * @param freeKeyElement - Function pointer to be used for removing key
 * element from the node.
 */
void nodeDestroy(Node node, freeNodeDataElements freeDataElement,
                       freeNodeKeyElements freeKeyElement){
    freeDataElement(node->data);
    freeKeyElement(node->key);
    free(node);
}

/**
 ***** Function: nodeGetKey *****
 * Description: Gets a node and returns node's key.
 * NOTICE: THE ORIGINAL KEY IS RETURNED.
 * @param node - The node which we want to get its key.
 * @return - Node's key element.
 */
NodeKeyElement nodeGetKey(Node node){
    assert(node);
    return node->key;
}

/**
 ***** Function: nodeGetData *****
 * Description: Gets a node and a copy data function and returns a copy of
 * node's data element.
 * NOTICE: USER IS RESPONSIBLE FOR DESTROYING THE DATA COPY.
 * @param node - The node which we want to get a copy of its data.
 * @param copyDataElement - Function pointer to be used for copying the
 * data element of the node.
 * @return - A copy of the given node's data.
 */
NodeDataElement nodeGetData(Node node,copyNodeDataElements copyDataElement){
    if(!node){
        /* Node is NULL. */
        return NULL;
    }
    if(!(node->data)){
        /* Node data is NULL. */
        return NULL;
    }
    /* Creating a copy of the node's data. */
    NodeDataElement data_copy = copyDataElement(node->data); //todo: bug is here!
     /* If copyDataElement failed data_copy will be NULL. */
    return data_copy;
}

/**
 ***** Function: nodeGetNext *****
 * Descritpion: Returns the next node of the given node.
 * @param node - The node which we want to find its next node.
 * @return - The next node.
 */
Node nodeGetNext(Node node){
    return node->next;
}

/**
 ***** Function: nodeSetNext *****
 * Description: Gets a node and a next_node and sets node's next
 * to be 'next_node'.
 * @param node - The node which we want to change its 'next'.
 * @param next_node - The node which we want to be pointed at by 'node'
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
 * Node's old data will be destroyed.
 * @param node - The node which we want to modify its data.
 * @param new_data - The new data to insert into the node.
 * @param copyDataElement - Pointer to the copy data element function.
 * Will be used to create a copy of the given new data.
 * @param freeDataElement - Pointer to the free data element function.
 * Will be used to destroy node's old data.
 * @return - Status success/failure of the function.
 */
NodeResult nodeSetData(Node node, NodeDataElement new_data,
                       copyNodeDataElements copyDataElement,
                       freeNodeDataElements freeDataElement){
    assert(node);
    if (!new_data){
        /* New data is NULL. */
        return NODE_NULL_ARGUMENT;
    }
    /* Creating a copy of the new Data*/
    NodeDataElement data_copy = copyDataElement(new_data);
    if(!data_copy){
        /* Couldn't create the data copy. */
        return NODE_OUT_OF_MEMORY;
    }
    /* New data copy created successfully. */
    freeDataElement(node->data); // Destroying old data.
    node->data = data_copy;
    return NODE_SUCCESS;
}


