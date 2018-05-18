#include "map.h"
#include "node.h"
#include <malloc.h>
#include <assert.h>

//-----------------------------------------------------------------------//
//                  DECLARATIONS OF STATIC FUNCTIONS                     //
//-----------------------------------------------------------------------//

static Node mapNodeToPlaceBefore(Map map, MapKeyElement key);
static Node mapGetNodeByKey(Map map,MapKeyElement key);
static Node mapGetPreviousNode(Map map, Node next_node);

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

/** Checked
 ***** Function: mapCreate *****
 * Description: Creates a new empty map.
 * @param copyDataElement - a pointer to a copy function of data elements.
 * @param copyKeyElement - a pointer to a copy function of key elements.
 * @param freeDataElement - a pointer to a destroy function of data
 * elements.
 * @param freeKeyElement - a pointer to a destroy function of key elements.
 * @param compareKeyElements - a pointer to a compare function of
 * key elements.
 * @return - A pointer to a new map if function succeeded, else NULL.
 */
Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements){

    Map map = malloc(sizeof(*map));
    if(!map){
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

/** Checked
 ****** Function: mapPut *****
 * Description: Gives a specific key a given value.
 * If the key exists, the value is overridden.
 * @param map - a pointer to a map.
 * @param keyElement - a pointer to a key element.
 * @param dataElement - a pointer to a data element.
 * @return - Success/failure of the function.
 */
MapResult mapPut(Map map, MapKeyElement keyElement,
                 MapDataElement dataElement){
    if(!map){
        return MAP_NULL_ARGUMENT;
    }
    if(!mapContains(map,keyElement)){
        /* Key element does not exists in map.
         * Creating a new node. */
        Node new_node = NodeCreate(dataElement,keyElement,map->copyDataElement,
                map->copyKeyElement,map->freeDataElement,
                               map->freeKeyElement);
        if(!new_node){ // Node creation failed.
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
    NodeResult result = nodeSetData(mapGetNodeByKey(map,keyElement)
            ,dataElement,map->copyDataElement,map->freeDataElement); // Modify data of key.

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
    Node current_node = mapGetNodeByKey(map,map->iterator);
    if(!current_node){
        /* Empty map */
        return NULL;
    }
    Node next_node = nodeGetNext(current_node);
    if(!next_node){
        /* Current node is the last node  */
        return NULL;
    }
    map->iterator = nodeGetKey(next_node);
    return map->iterator;
}

/** Checked
 ***** Function: mapGetFirst *****
 * Description: Sets the internal iterator to the first key in the map,
 * and returns it.
 * @param map - a pointer to a map.
 * @return - First key in the map.
 */
MapKeyElement mapGetFirst(Map map){
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
 * key. Iterator status unchanged.
 * @param map
 * @param keyElement
 * @return
 */
MapDataElement mapGet(Map map, MapKeyElement keyElement){
    assert(!keyElement);
    if (!mapContains(map,keyElement)){
        /*Key does not exists in map */
        return NULL;
    }
    Node current_node = mapGetNodeByKey(map,keyElement);
    assert(!current_node);
    MapDataElement current_node_data=nodeGetData
            (current_node,map->copyDataElement);
    map->iterator=NULL;
    /* Current_node_data will be NULL if copyDataElement failed*/
    return current_node_data;
}

/** Checked
 ***** Function: mapContaints *****
 * Description: Returns weather or not a key exists in the map.
 * This resets the internal iterator.
 * @param map
 * @param element
 * @return
 */
bool mapContains(Map map, MapKeyElement element){
    if(!map || !element){
        return false;
    }
    Node node = mapGetNodeByKey(map,element);
    if(!node){
        /*If the key doesn't exist return's Null */
        return false;
    }
    /*If we got here, the key exists*/
    return true;
}

/** Checked
 ***** Function: mapDestroy *****
 * Description: destroys all map's resources and the pointer to map.
 * @param map - A pointer to map.
 */
void mapDestroy(Map map){
    if(map==NULL){
        return;
    }
    Node node_iterator = map->list, temp_node;
    while(node_iterator){
        temp_node = node_iterator;
        node_iterator = nodeGetNext(node_iterator);
        nodeDestroy(temp_node,map->freeDataElement,
                    map->freeKeyElement);
    }
    free(map);
}

/** Checked
 ***** Function: mapCopy *****
 * Description: Creates a copy of target map.
 * Iterator values for both maps is undefined after this operation.
 * @param map - A pointer to map.
 * @return - NULL in case of failure, else returns a pointer to a copy of
 * the map.
 */
Map mapCopy(Map map){
    if(!map){
        return NULL;
    }
    Map new_map=mapCreate(map->copyDataElement,map->copyKeyElement,
                          map->freeDataElement,map->freeKeyElement,
                          map->compareKeyElements);
    if(!new_map){
        return NULL;
    }
    MapDataElement current_node_data_copy;
    MapResult result;
    MAP_FOREACH(MapKeyElement,current_node_key,map){
        current_node_data_copy = mapGet(map,current_node_key);
        if(!current_node_data_copy){
            /* Failed to copy data */
            mapDestroy(new_map);
            map->iterator=NULL;
            return NULL;
        }
        result=mapPut(new_map,current_node_key,current_node_data_copy);
        if(result!=MAP_SUCCESS){
            map->freeDataElement(current_node_data_copy);
            mapDestroy(new_map);
            map->iterator=NULL;
            return NULL;
        }
        map->freeDataElement(current_node_data_copy);
    }
    map->iterator=NULL;
    return new_map;
}

/** Checked
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
       if(map->compareKeyElements(iterator,key)==-1){
           continue;
       }
       return mapGetNodeByKey(map,iterator);
    }
    /* If we got here then the key should be placed at the end of the
     * map. */
    return NULL;
}

/** Checked
 *
 * @param map
 * @param key
 * @return
 */
static Node mapGetNodeByKey(Map map,MapKeyElement key){
    assert(!key);
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
/*Checked */
static Node mapGetPreviousNode(Map map, Node next_node) {
    assert(!map);
    Node node_iterator;
    MAP_FOREACH(MapKeyElement, iterator, map) {
        node_iterator = mapGetNodeByKey(map, iterator);
        if (nodeGetNext(node_iterator) == next_node) {
            return node_iterator;
        }
    }
    /* If we got here then map is empty or contains one node. */
    return NULL;
}


