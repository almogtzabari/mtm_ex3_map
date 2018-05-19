
#ifndef MTM_EX3_NODE_H
#define MTM_EX3_NODE_H

#include "map_mtm.h"
typedef struct node_t *Node;

/** Type used for returning error codes from map functions */
typedef enum NodeResult_t {
    NODE_SUCCESS,
    NODE_OUT_OF_MEMORY,
    NODE_NULL_ARGUMENT
} NodeResult;

/** Data element data type for node container */
typedef void* NodeDataElement;

Node nodeCreate(MapDataElement data, MapKeyElement key,
                copyMapDataElements copyDataElement,
                copyMapKeyElements copyKeyElement,
                freeMapDataElements freeDataElement,
                freeMapKeyElements freeKeyElement);

void nodeDestroy(Node node, freeMapDataElements freeDataElement,
                 freeMapKeyElements freeKeyElement);

MapKeyElement nodeGetKey(Node node);
Node nodeGetNext(Node node);
NodeResult nodeSetNext(Node node, Node next_node);
NodeResult nodeSetData(Node node, MapDataElement new_data,
                       copyMapDataElements copyDataElement,
                       freeMapDataElements freeDataElement);
MapDataElement nodeGetData(Node node, copyMapDataElements copyDataElement);

void nodeDestroy(Node node, freeMapDataElements freeDataElement,
                 freeMapKeyElements freeKeyElement);

#endif //MTM_EX3_NODE_H
