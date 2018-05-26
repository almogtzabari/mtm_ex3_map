
#ifndef MTM_EX3_NODE_H
#define MTM_EX3_NODE_H

//-----------------------------------------------------------------------//
//                             TYPEDEFS                                  //
//-----------------------------------------------------------------------//

typedef struct node_t *Node;

/** Type used for returning error codes from node functions */
typedef enum NodeResult_t {
    NODE_SUCCESS,
    NODE_OUT_OF_MEMORY,
    NODE_NULL_ARGUMENT
} NodeResult;

/** Data element data type for node container */
typedef void* NodeDataElement;

/** Key element data type for node container */
typedef void* NodeKeyElement;

/** Type of function for copying a data element of the node */
typedef NodeDataElement(*copyNodeDataElements)(NodeDataElement);

/** Type of function for copying a key element of the node */
typedef NodeKeyElement(*copyNodeKeyElements)(NodeKeyElement);

/** Type of function for deallocating a data element of the node */
typedef void(*freeNodeDataElements)(NodeDataElement);

/** Type of function for deallocating a key element of the node */
typedef void(*freeNodeKeyElements)(NodeKeyElement);

//-----------------------------------------------------------------------//
//                      FUNCTION DECLARATIONS                            //
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
                freeNodeKeyElements freeKeyElement);

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
                 freeNodeKeyElements freeKeyElement);

/**
 ***** Function: nodeGetKey *****
 * Description: Gets a node and returns node's key.
 * NOTICE: THE ORIGINAL KEY IS RETURNED.
 * @param node - The node which we want to get its key.
 * @return - Node's key element.
 */
NodeKeyElement nodeGetKey(Node node);

/**
 ***** Function: nodeGetNext *****
 * Descritpion: Returns the next node of the given node.
 * @param node - The node which we want to find its next node.
 * @return - The next node.
 */
Node nodeGetNext(Node node);

/**
 ***** Function: nodeSetNext *****
 * Description: Gets a node and a next_node and sets node's next
 * to be 'next_node'.
 * @param node - The node which we want to change its 'next'.
 * @param next_node - The node which we want to be pointed at by 'node'
 * @return - Status success/failure of function.
 */
NodeResult nodeSetNext(Node node, Node next_node);

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
                       freeNodeDataElements freeDataElement);

/**
 ***** Function: nodeGetData *****
 * Description: Gets a node and returns node's data element.
 * @param node - The node which we want to get a copy of its data.
 * @return - Node's data.
 */
NodeDataElement nodeGetData(Node node);

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
                 freeNodeKeyElements freeKeyElement);

#endif //MTM_EX3_NODE_H
