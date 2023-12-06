//
// Created by Alether on 10/27/2023.
//

#include <assert.h>
#include"hashmap.h"

int testHashMapPrimitive() {
    AlHashMap map;
    alHashMapInit(&map, sizeof(int), 0);

    int a = 1;
    int b = 2;
    int c = 3;

    alHashMapInsert(&map, "one", &a);
    alHashMapInsert(&map, "two", &b);
    alHashMapInsert(&map, "four", &c);

    int *aa = (int*)alHashMapGet(map, "one");
    int *bb = (int*)alHashMapGet(map, "two");
    int *cc = (int*)alHashMapGet(map, "four");

    assert(*aa == 1);
    assert(*bb == 2);
    assert(*cc == 3);

    alHashMapDeinit(&map, true);
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

    assert(aa->a == 1);
    assert(aa->b == 2.123f);
    assert(strcmp(aa->dynamicString, "grifin") == 0);

    assert(bb->a == 2);
    assert(bb->b == 12.123f);
    assert(strcmp(bb->dynamicString, "aqwerw") == 0);

    assert(cc->a == 3);
    assert(cc->b == 12321.123f);
    assert(strcmp(cc->dynamicString, "katon") == 0);

    alHashMapDeinit(&map, true);
    return 0;
}


int main(int argc, char **argv) {
    if (strcmp(argv[argc - 1], "testHashMapStruct") == 0) return testHashMapStruct();
    if (strcmp(argv[argc - 1], "testHashMapPrimitive") == 0) return testHashMapPrimitive();
}
