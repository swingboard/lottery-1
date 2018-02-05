#ifndef LOTTERY_DYNAMIC_STACK_ARRAY_HPP
#define LOTTERY_DYNAMIC_STACK_ARRAY_HPP


/**
    Declares a dynamic array on the stack, using alloca.
 */
#define LOTTERY_DYNAMIC_STACK_ARRAY(TYPE, NAME, SIZE, DEFAULT_VALUE)\
    TYPE *NAME = (TYPE *)alloca(sizeof(TYPE) * SIZE);\
    for (size_t i = 0; i < SIZE; ++i) { NAME[i] = DEFAULT_VALUE; }


#endif //LOTTERY_DYNAMIC_STACK_ARRAY_HPP
