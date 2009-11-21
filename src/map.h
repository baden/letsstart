#ifndef MAP_H_
#define MAP_H_

#include "object.h"

typedef struct{
} tMapItem;

typedef struct{
    float height; // Высота над уровнем моря :)
    tObject *static_objects;    // Объект-пол клетки
} tFloorItem;

typedef struct{
    int width;  // размер карты
    int height; // размер карты
    tFloorItem *floor;  // массив пола
} tMap;


#endif
