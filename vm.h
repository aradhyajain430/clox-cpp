#pragma once

#include "chunk.h"
#include "value.h"

#include <string>


enum InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VM {

    static constexpr int STACK_MAX = 256;

    Chunk* chunk = nullptr;
    size_t ip = 0;

    Value stack[STACK_MAX]; // using a raw C array because I wanna understand the
    Value* stackTop {stack};     // specifics of stacks better.

    void push(Value value);
    Value pop();

    template <typename Op>
    void binaryOp(Op op) {
        Value b = pop();
        Value a = pop();
        push(op(a, b));
    }

    InterpretResult run();
    uint8_t readByte();

public:
    InterpretResult interpret(std::string_view source);

};