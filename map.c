#include "map.h"
#include "node.h"
#include <malloc.h>
#include <assert.h>

static Node mapPutBefore(Map map, MapKeyElement key);
static Node mapGetNodeByKey(Map map,MapKeyElement key);
static Node mapGetPreviousNode(Map map, Node next_node);

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

Map mapCreate(copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement,
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

MapResult mapPut(Map map, MapKeyElement keyElement,
                 MapDataElement dataElement){
    if(!mapContains(map,keyElement)){
        /* Key element does not exists in map. */
        Node current_node = NodeCreate(dataElement,keyElement,map->copyDataElement,
                map->copyKeyElement,map->freeDataElement,
                               map->freeKeyElement);
        if(!current_node) {
            return MAP_OUT_OF_MEMORY;
        }
        Node next_node = mapPutBefore(map,nodeGetKey(current_node));
        nodeSetNext(current_node,next_node);
        Node previous_node = mapGetPreviousNode(map,next_node);
        if(previous_node){
            nodeSetNext(previous_node,current_node);
        }
        else{
            /* Current node should be placed at the beginning of the map.*/
            map->list = current_node;
        }
        map->iterator = NULL;
        map->mapSize++;
        return MAP_SUCCESS;
    }
    /* If we got here, the key already exists and we need to modify it */
    map->iterator = NULL;
    NodeResult result=nodeSetData(mapGetNodeByKey(map,keyElement)
            ,dataElement,map->copyDataElement);
    if(result!=NODE_SUCCESS){
        return MAP_OUT_OF_MEMORY;
    }
    return MAP_SUCCESS;

}

static Node mapPutBefore(Map map, MapKeyElement key){
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

MapKeyElement mapGetNext(Map map){
    Node current_node = mapGetNodeByKey(map,map->iterator);
    /* In case of empty map returns NULL. todo: check if that is ok.*/
    if(!current_node){
        return NULL;
    }
    Node next_node = nodeGetNext(current_node);
    if(!next_node){
        return NULL;
    }
    map->iterator = nodeGetKey(next_node);
    return map->iterator;
}

MapKeyElement mapGetFirst(Map map){
    /* In case of empty map returns NULL. todo: check if that is ok.*/
    if(!map->list){
        return NULL;
    }
    map->iterator = nodeGetKey(map->list);
    return map->iterator;
}

static Node mapGetNodeByKey(Map map,MapKeyElement key){
    if(!key){
        return NULL;
    }
    Node current_node = map->list;
    MapKeyElement current_node_key;
    while(current_node) {
        current_node_key = nodeGetKey(current_node);
        if (map->compareKeyElements(current_node_key, key) == 0) {
            return current_node;
        }
        current_node = nodeGetNext(current_node);
    }
    /* Node with that key wasn't found. */
    return NULL;
}

static Node mapGetPreviousNode(Map map, Node next_node){
    assert(!map);
    Node node_iterator = map->list;
    while(!node_iterator){
        if(nodeGetNext(node_iterator) == next_node){
            return node_iterator;
        }
        node_iterator = nodeGetNext(node_iterator);
    }
    /* If we got here then map is empty. */
    return NULL;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    assert(!keyElement);
    if (!mapContains(map,keyElement)){
        return NULL;
    }
    Node current_node=mapGetNodeByKey(map,keyElement);
    MapDataElement current_node_data=nodeGetData(current_node,map->copyDataElement);
    return current_node_data;
}

bool mapContains(Map map, MapKeyElement element){
    return (bool)mapGetNodeByKey(map,element);
}
