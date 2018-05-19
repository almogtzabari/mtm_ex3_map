#include "map_mtm.h"
#include "node.h"
#include <malloc.h>
#include <assert.h>
#include <stdio.h>

//-----------------------------------------------------------------------//
//                  DECLARATIONS OF STATIC FUNCTIONS                     //
//-----------------------------------------------------------------------//

static Node mapNodeToPlaceBefore(Map map, MapKeyElement key);
static Node mapGetNodeByKey(Map map,MapKeyElement key);
static Node mapGetPreviousNode(Map map, Node node);

//-----------------------------------------------------------------------//
//                            STRUCT MAP                                 //
//-----------------------------------------------------------------------//
struct Map_t{
    Node list;
    MapKeyElement iterator;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
    int mapSize;
};

//-----------------------------------------------------------------------//
//                         STRUCT MAP FUNCTIONS                          //
//-----------------------------------------------------------------------//

/**
 ***** Function: mapCreate *****
* Description: Allocates a new empty map.
*
* @param copyDataElement - Function pointer to be used for copying data
* elements into the map or when copying the map.
* @param copyKeyElement - Function pointer to be used for copying key
* elements into the map or when copying the map.
* @param freeDataElement - Function pointer to be used for removing data
* elements from the map.
* @param freeKeyElement - Function pointer to be used for removing key
* elements from the map.
* @param compareKeyElements - Function pointer to be used for comparing key
* elements inside the map. Used to check if new elements already exist
* in the map.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Map in case of success.
*/
Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements){

    Map map = malloc(sizeof(*map));
    if(!map || !copyDataElement || !copyKeyElement || !freeDataElement
       || !freeKeyElement || !compareKeyElements){
        return NULL;
    }
    map->copyDataElement = copyDataElement;
    map->copyKeyElement = copyKeyElement;
    map->freeDataElement = freeDataElement;
    map->freeKeyElement = freeKeyElement;
    map->compareKeyElements = compareKeyElements;
    map->list = NULL;
    map->iterator = NULL;
    map->mapSize=0;
    return map;
}

/**
****** Function: mapPut *****
* Description: Gives a specific key a given value.
* If the key exists, the value is overridden.
* @param map - The map for which to reassign the data element.
* @param keyElement - The key element which need to be reassigned.
* @param dataElement- The new data element to associate with the given key.
* A copy of the element will be inserted as supplied by the copying
* function which is given at initialization and old data memory would be
* deleted using the free function given at initialization.
* @return
*  	MAP_NULL_ARGUMENT if a NULL was sent as map
* 	MAP_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
* 	MAP_SUCCESS the paired elements had been inserted successfully
*/
MapResult mapPut(Map map, MapKeyElement keyElement,
                 MapDataElement dataElement){
    if(!map){
        return MAP_NULL_ARGUMENT;
    }
    if(!keyElement || !dataElement){
        map->iterator = NULL;
        return MAP_NULL_ARGUMENT;
    }
    if(!mapContains(map,keyElement)){
        /* Key element does not exists in map.
         * Creating a new node. */
        Node new_node = nodeCreate(dataElement,keyElement,
                                   map->copyDataElement,map->copyKeyElement,
                                   map->freeDataElement,
                                   map->freeKeyElement);
        if(!new_node){ // Node creation failed.
            map->iterator = NULL;
            return MAP_OUT_OF_MEMORY;
        }
        MapKeyElement key = nodeGetKey(new_node);
        Node after_node = mapNodeToPlaceBefore(map,key);
        /* Next node is the node that supposed to be after the new node.
         * In case of NULL, new node is the current last node.*/
        nodeSetNext(new_node,after_node);
        Node before_node = mapGetPreviousNode(map,after_node);
        if(before_node){
            nodeSetNext(before_node,new_node);
        }
        else{
            /*  If we got here then new node should be placed at the
             * beginning of the map.*/
            map->list = new_node;
        }
        map->iterator = NULL; // Resetting iterator.
        map->mapSize++; // Added new element.
        return MAP_SUCCESS;
    }

    /* If we got here, the key already exists and we need to modify its
     * data. */
    map->iterator = NULL; // Resetting iterator.
    // Modify data of key.
    NodeResult result = nodeSetData(mapGetNodeByKey(map,keyElement)
            ,dataElement,map->copyDataElement,map->freeDataElement);

    if(result!=NODE_SUCCESS){
        /* Couldn't create a copy of the new data.
         * Node's data remained as it was. */
        return MAP_OUT_OF_MEMORY;
    }
    return MAP_SUCCESS;
}

/** Checked
 ***** Function: mapGetNext *****
 * Description: Advances the internal iterator to the next key and returns
 * it.
 * @param map - a pointer to a map.
 * @return - Next key in the map.
 */
MapKeyElement mapGetNext(Map map){
    if(!map){
        return NULL;
    }
    if(!map->iterator){
        return NULL;
    }
    Node current_node = mapGetNodeByKey(map,map->iterator);
    if(!current_node){
        /* Empty map */
        return NULL;
    }
    Node next_node = nodeGetNext(current_node);
    if(!next_node){
        /* Current node is the last node */
        return NULL;
    }
    map->iterator = nodeGetKey(next_node);
    return map->iterator;
}

/**
***** Function: mapGetFirst *****
* Description: Sets the internal iterator (also called current key element)
* to the first key element in the map. There doesn't need to be an internal
* order of the keys so the "first" key element is any key element.
* Use this to start iteraing over the map.
* To continue iteration use mapGetNext.
* @param map - The map for which to set the iterator and return the first
* key element.
* @return
*  	NULL if a NULL pointer was sent or the map is empty.
* 	The first key element of the map otherwise
*/
MapKeyElement mapGetFirst(Map map){
    if(!map){
        return NULL;
    }
    /* In case of empty map returns NULL.*/
    if(!map->list){
        return NULL;
    }
    map->iterator = nodeGetKey(map->list);
    return map->iterator;
}

/** Checked
***** Function: mapGet *****
* Description: Returns the data paired to a key which matches the given
* key.
* Iterator status unchanged.
* @param map - The map for which to get the data element from.
* @param keyElement - The key element which need to be found and whos data
we want to get.
* @return
*   NULL if a NULL pointer was sent or if the map does not contain the requested key.
* 	The data element associated with the key otherwise.
*/
MapDataElement mapGet(Map map, MapKeyElement keyElement){
    if(!map || !keyElement){
        return NULL;
    }
    Node current_node = mapGetNodeByKey(map,keyElement);
    if(!current_node){
        /* Key does not exist. */
        return NULL;
    }
    assert(current_node);
    MapDataElement current_node_data = nodeGetData
            (current_node,map->copyDataElement);
    /* Current_node_data will be NULL if copyDataElement failed*/
    return current_node_data;
}

/**
 ***** Function: mapContaints *****
 * Description: Checks if a key element exists in the map. The key element
 * will be considered in the map if one of the key elements in the map it
 * determined equal using the comparison function used to initialize the
 * map.
 * @param map - The map to search in.
 * @param element - The element to look for. Will be compared using the
 * comparison function.
 * @return
 * 	false - if one or more of the inputs is null, or if the key element was
 * 	not found.
 * 	true - if the key element was found in the map.
 */
bool mapContains(Map map, MapKeyElement element){
    if(!map){
        return false;
    }
    if(!element){
        map->iterator = NULL;
        return false;
    }
    Node node = mapGetNodeByKey(map,element);
    if(!node){
        /*If the key doesn't exist returns false */
        map->iterator = NULL;
        return false;
    }
    /*If we got here, the key exists*/
    map->iterator = NULL;
    return true;
}

/**
 ***** Function: mapDestroy *****
 * Description: Deallocates an existing map. Clears all elements by using
 * the stored free functions.
 * @param map - A pointer to map.
 */
void mapDestroy(Map map){
    if(!map){
        return;
    }
    mapClear(map);
    free(map);
}

/** Checked
 ***** Function: mapCopy *****
 * Description: Creates a copy of target map.
 * Iterator values for both maps is undefined after this operation.
 * @param map - Target map.
 * @return
 * 	NULL if a NULL was sent or a memory allocation failed.
 * 	A Map containing the same elements as map otherwise.
 */
Map mapCopy(Map map){
    if(!map){
        return NULL;
    }
    Map new_map=mapCreate(map->copyDataElement,map->copyKeyElement,
                          map->freeDataElement,map->freeKeyElement,
                          map->compareKeyElements);
    if(!new_map){
        map->iterator=NULL;
        return NULL;
    }
    MapDataElement current_node_data_copy;
    MapResult result;
    MAP_FOREACH(MapKeyElement,current_node_key,map){
        /* mapGet creates a copy of the current node data */
        current_node_data_copy = mapGet(map,current_node_key);
        if(!current_node_data_copy){
            /* Failed to copy data */
            mapDestroy(new_map);
            map->iterator=NULL;
            return NULL;
        }
        result = mapPut(new_map,current_node_key,current_node_data_copy);
        if(result!=MAP_SUCCESS){
            map->freeDataElement(current_node_data_copy);
            mapDestroy(new_map);
            map->iterator=NULL;
            return NULL;
        }
        /* Finished using current node data copy, frees it */
        map->freeDataElement(current_node_data_copy);
    }
    map->iterator = NULL; // Resetting iterator.
    return new_map;
}

/**
 ***** Function: mapGetSize *****
 * Description: Returns the number of elements in a map
 * @param map - The map which size is requested
 * @return
 * 	-1 if a NULL pointer was sent.
 * 	Otherwise the number of elements in the map.
 */
int mapGetSize(Map map){
    if(!map){
        return -1;
    }
    return map->mapSize;
}

/**
***** Function: mapRemove *****
* Description: Removes a pair of key and data elements from the map.
* The elements are found using the comparison function given at
* initialization. Once found, the elements are removed and deallocated
* using the free functions supplied at initialzation.
*  Iterator's value is undefined after this operation.
* @param map - The map to remove the elements from.
* @param keyElement - The key element to find and remove from the map.
* The element will be freed using the free function given at
* initialization. The data element associated with this key will also be
* freed using the free function given at initialization.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent to the function
*  MAP_ITEM_DOES_NOT_EXIST if an equal key item does not already exists in
*  the map
* 	MAP_SUCCESS the paired elements had been removed successfully
*/
MapResult mapRemove(Map map, MapKeyElement keyElement){
    if(!map){
        return MAP_NULL_ARGUMENT;
    }
    if(!keyElement){
        map->iterator = NULL;
        return MAP_NULL_ARGUMENT;
    }
    Node node = mapGetNodeByKey(map,keyElement);
    if(!node){
        /* Key element does not exist in map. */
        map->iterator = NULL; // Resetting iterator.
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    Node previous_node = mapGetPreviousNode(map,node);
    if(!previous_node){
        /* Node with given key is first. */
        map->list = nodeGetNext(node);
        nodeDestroy(node,map->freeDataElement,map->freeKeyElement);
        map->iterator = NULL; // Resetting iterator.
        return MAP_SUCCESS;
    }
    nodeSetNext(previous_node,nodeGetNext(node));
    nodeDestroy(node,map->freeDataElement,map->freeKeyElement);
    map->iterator = NULL; // Resetting iterator.
    return MAP_SUCCESS;
}

/**
 ***** Function: mapClear *****
 * Description: Removes all key and data elements from target map.
 * The elements are deallocated using the stored free functions.
 * @param map - Target map to remove all element from.
 * @return
 * 	MAP_NULL_ARGUMENT - if a NULL pointer was sent.
 * 	MAP_SUCCESS - Otherwise.
 */
MapResult mapClear(Map map) {
    if(!map){
        return MAP_NULL_ARGUMENT;
    }
    Node node_iterator = map->list, temp_node;
    while (node_iterator) {
        temp_node = node_iterator;
        node_iterator = nodeGetNext(node_iterator);
        nodeDestroy(temp_node, map->freeDataElement,
                    map->freeKeyElement);
    }
    return MAP_SUCCESS;
}

//-----------------------------------------------------------------------//
//                        MAP STATIC FUNCTIONS                           //
//-----------------------------------------------------------------------//

/** Checked.
 ***** Static function: mapPutBefore *****
 * Description: Gets a map and a key and returns a pointer to the node
 * that should be placed after the node with the given key.
 * @param map - a pointer to a map.
 * @param key - a pointer to a key.
 * @return - Pointer to the node that should be placed after the node with
 * given key.
 */
static Node mapNodeToPlaceBefore(Map map, MapKeyElement key){
    MAP_FOREACH(MapKeyElement,iterator,map) {
        if(map->compareKeyElements(iterator,key)<0){
            continue;
        }
        return mapGetNodeByKey(map,iterator);
    }
    /* If we got here then the key should be placed at the end of the
     * map. */
    return NULL;
}

/** Checked
 ***** Function: mapGetNodeByKey *****
 * Description: Gets a map and a key and returns the node of that key in
 * the map.
 * @param map - a pointer to a map.
 * @param key - a pointer to the key that we wish to find its node in the
 * map.
 * @return - The node of that key in the map.
 */
static Node mapGetNodeByKey(Map map,MapKeyElement key){
    assert(key);
    Node current_node = map->list; // Resetting to first node.
    MapKeyElement current_node_key;
    while(current_node) {
        current_node_key = nodeGetKey(current_node);
        if (map->compareKeyElements(current_node_key, key) == 0){
            return current_node;
        }
        current_node = nodeGetNext(current_node);
    }
    /* Node with that key wasn't found. */
    return NULL;
}
/** Checked
 ***** Function: mapGetPreviousNode *****
 * Description: Gets a map and a node and returns the node before the given
 * node in the map. If the given node is first or the map is empty the
 * function returns NULL.
 * @param map - a pointer to a map.
 * @param node - a pointer to the node we want to find its previous.
 * @return - Returns the node before the given node in the map.
 */
static Node mapGetPreviousNode(Map map, Node node) {
    assert(map);
    Node node_iterator;
    MAP_FOREACH(MapKeyElement, iterator, map) {
        node_iterator = mapGetNodeByKey(map, iterator);
        if (nodeGetNext(node_iterator) == node) {
            return node_iterator;
        }
    }
    /* If we got here then map is empty or the given node is first. */
    return NULL;
}

