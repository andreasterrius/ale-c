//
// Created by Alether on 10/26/2023.
//
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "array.h"

typedef struct TestData {
    i32 id;
    char *someText;
} TestData;

int testArrayStruct() {
    AlArray myArray;
    alArrayInit(&myArray, sizeof(TestData), 0);

    alArrayPush(&myArray, &(TestData) {.id = 100, .someText="firstElement"});
    alArrayPush(&myArray, &(TestData) {.id= 200, .someText="secondelement"});
    alArrayPush(&myArray, &(TestData) {.id= 300, .someText="ThirdElement"});
    bool isOk = alArrayPush(&myArray, &(TestData) {.id= 400, .someText="Fourth"});
    assert(isOk == true);

    int i;
    for (i = 0; i < alArraySize(myArray); ++i) {
        TestData *t = (TestData *) alArrayGet(myArray, i);
        printf("Mem location: %p, id: %d\n", t, t->id);
        switch (i) {
            case 0:
                assert(t->id == 100);
                assert(strcmp(t->someText, "firstElement") == 0);
                break;
            case 1:
                assert(t->id == 200);
                assert(strcmp(t->someText, "secondelement") == 0);
                break;
            case 2:
                assert(t->id == 300);
                assert(strcmp(t->someText, "ThirdElement") == 0);
                break;
            case 3:
                assert(t->id == 400);
                assert(strcmp(t->someText, "Fourth") == 0);
                break;
        }
    }
    assert(i == 4);

    alArrayPop(&myArray);
    alArrayPop(&myArray);
    assert(alArraySize(myArray) == 2);

    TestData *t = (TestData *) alArrayGet(myArray, 0);
    assert(t->id == 100);
    assert(strcmp(t->someText, "firstElement") == 0);

    t = (TestData *) alArrayGet(myArray, 1);
    assert(t->id == 200);
    assert(strcmp(t->someText, "secondelement") == 0);

    alArrayPop(&myArray);
    isOk = alArrayPop(&myArray);
    assert(isOk == true);

    isOk = alArrayPop(&myArray);
    assert(isOk == false);

    alArrayDeinit(&myArray);

    return 0;
}

int testArrayPrimitive() {
    AlArray myArray;
    alArrayInit(&myArray, sizeof(int), 0);

    int a = 1;
    int b = 2;
    int c = 3;

    alArrayPush(&myArray, &a);

    alArrayPush(&myArray, &b);
    alArrayPush(&myArray, &c);
    assert(alArraySize(myArray) == 3);

    int *t = (int *) alArrayGet(myArray, 0);
    assert(*t == 1);
    t = (int *) alArrayGet(myArray, 1);
    assert(*t == 2);
    t = (int *) alArrayGet(myArray, 2);
    assert(*t == 3);

    alArrayDeinit(&myArray);
    return 0;
}


int main(int argc, char **argv) {
    if (strcmp(argv[argc - 1], "testArrayStruct") == 0) return testArrayStruct();
    else if (strcmp(argv[argc - 1], "testArrayPrimitive") == 0) return testArrayPrimitive();
}