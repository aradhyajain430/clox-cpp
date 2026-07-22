#pragma once

#include "chunk.h"
#include "value.h"

#include <string_view>


enum InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VM {

    static constexpr int STACK_MAX = 256;

    Chunk* chunk = nullptr; // points into the interpret frame. only exists when called. 
    size_t ip = 0; // index, not a pointer. 

    // fixed inline storage, 256 slots, zero allocations. std::array<Value, STACK_MAX> is
    // equivalent here -- same storage, same codegen, plus .size() and no decay.
    // keep in mind that stackTop is a bare pointer walking a bare buffer. 
    Value stack[STACK_MAX];  
    Value* stackTop {stack};


    void push(Value value);
    Value pop();

    template <typename Op>
    void binaryOp(Op op) { // b then a. Otherwise non-commutative operations get messed up.
        Value b = pop();
        Value a = pop();
        push(op(a, b));
    }

    InterpretResult run();
    uint8_t readByte();

public:
    VM() = default;                              // Still want normal construction
    // you don't want copies because the copy's stackTop remains pointed at the original's memory. 
    VM(const VM&) = delete;                      // ban copy construction
    VM& operator=(const VM&) = delete;           // ban copy assignment
    VM(VM&&) = delete;                           // ban move construction
    VM& operator=(VM&&) = delete;                // ban move assignment


    InterpretResult interpret(std::string_view source);

};