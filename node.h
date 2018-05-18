
#ifndef MTM_EX3_NODE_H
#define MTM_EX3_NODE_H

#include "map.h"
typedef struct node_t *Node;

typedef enum NodeResult_t {
    NODE_SUCCESS,
    NODE_OUT_OF_MEMORY,
    NODE_NULL_ARGUMENT
} NodeResult;

/** Data element data type for node container */
typedef void* NodeDataElement;

Node NodeCreate(MapDataElement data, MapKeyElement key,
                copyMapDataElements copyDataElement,
                copyMapKeyElements copyKeyElement,
                freeMapDataElements freeDataElement,
                freeMapKeyElements freeKeyElement);

MapKeyElement nodeGetKey(Node node);
Node nodeGetNext(Node node);
NodeResult nodeSetNext(Node current, Node next);
NodeResult nodeSetData(Node node,MapDataElement data,copyMapDataElements copyDataElement);
MapDataElement nodeGetData(Node node, copyMapDataElements copyDataElement);

#endif //MTM_EX3_NODE_H
