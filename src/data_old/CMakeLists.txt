## Hashmap
add_executable(hashmap_test hashmap_test.c hashmap.c hashmap.h)
add_test(NAME test_hashmap_struct COMMAND hashmap_test "testHashMapStruct")
add_test(NAME test_hashmap_primitive COMMAND hashmap_test "testHashMapPrimitive")
add_test(NAME test_hashmap_grow COMMAND hashmap_test "testHashMapGrow")

## Array
add_executable(array_test array_test.c array.c array.h)
add_test(NAME test_array_struct COMMAND array_test "testArrayStruct")
add_test(NAME test_array_primitive COMMAND array_test "testArrayPrimitive")