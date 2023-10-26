//
// Created by Alether on 10/27/2023.
//

#include"hashmap.h"

int testHashMapStruct() {
    AlHashMap map;
    alHashMapInit(&map, sizeof(int), 3);

    int a = 1;
    int b = 2;
    int c = 3;

    alHashMapInsert(&map, "one", &a);
    alHashMapInsert(&map, "two", &b);
    alHashMapInsert(&map, "four", &c);

    int *aa = (int*)alHashMapGet(map, "one");
    int *bb = (int*)alHashMapGet(map, "two");
    int *cc = (int*)alHashMapGet(map, "four");

    printf("Value for 'one': %d\n", *aa);
    printf("Value for 'two': %d\n", *bb);
    printf("Value for 'four': %d\n", *cc);

    return 0;
}

int testHashMapPrimitive() {

}

int main(int argc, char **argv) {
    if (strcmp(argv[argc - 1], "testHashMapStruct") == 0) return testHashMapStruct();
    if (strcmp(argv[argc - 1], "testHashMapPrimitive") == 0) return testHashMapPrimitive();
}