//
// Created by alether on 10/5/23.
//

#ifndef HELLO_C_DEFER_H
#define HELLO_C_DEFER_H

#define CCATLN_1(x, y) x##y
#define CCATLN_2(x, y) CCATLN_1(x, y)
#define CONCAT_LINE(x) CCATLN_2(x, __LINE__)

#define defer __attribute__((cleanup(defer_cleanup))) void (^CONCAT_LINE($deferfn_))(void)  = ^
#define nocapture __block

static void defer_cleanup(void *ptr)
{
    void (^*fn)(void) = (void (^*)(void))ptr;
    (*fn)();
}

#endif //HELLO_C_DEFER_H
