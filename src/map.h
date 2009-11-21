#ifndef MAP_H_
#define MAP_H_

#include "object.h"

typedef struct{
} tMapItem;

typedef struct{
    float height; // ������ ��� ������� ���� :)
    tObject *static_objects;    // ������-��� ������
} tFloorItem;

typedef struct{
    int width;  // ������ �����
    int height; // ������ �����
    tFloorItem *floor;  // ������ ����
} tMap;


#endif
