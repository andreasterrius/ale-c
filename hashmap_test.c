//
// Created by Alether on 10/27/2023.
//

#include"hashmap.h"

int testHashMapPrimitive() {
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

typedef struct HashMapTestData {
    int a;
    float b;
    char *dynamicString;
} HashMapTestData;

int testHashMapStruct() {
    AlHashMap map;
    alHashMapInit(&map, sizeof(HashMapTestData), 3);

    alHashMapInsert(&map, "one", &(HashMapTestData){
        .a = 1,
        .b = 2.123f,
        .dynamicString = "grifin"
    });
    alHashMapInsert(&map, "two", &(HashMapTestData){
            .a = 2,
            .b = 12.123f,
            .dynamicString = "aqwerw"
    });
    alHashMapInsert(&map, "four", &(HashMapTestData){
            .a = 3,
            .b = 12321.123f,
            .dynamicString = "katon"
    });

    HashMapTestData *aa = (int*)alHashMapGet(map, "one");
    HashMapTestData *bb = (int*)alHashMapGet(map, "two");
    HashMapTestData *cc = (int*)alHashMapGet(map, "four");

    printf("Value for 'one': %d\n", aa->a);
    printf("Value for 'two': %d\n", bb->a);
    printf("Value for 'four': %d\n", cc->a);

    return 0;
}

int testHashMapGrow() {
    AlHashMap map;
    alHashMapInit(&map, sizeof(int), 1);

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

int main(int argc, char **argv) {
    if (strcmp(argv[argc - 1], "testHashMapStruct") == 0) return testHashMapStruct();
    if (strcmp(argv[argc - 1], "testHashMapPrimitive") == 0) return testHashMapPrimitive();
    if (strcmp(argv[argc - 1], "testHashMapGrow") == 0) return testHashMapGrow();
}
