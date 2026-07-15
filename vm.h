#pragma once

#include "chunk.h"

enum InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

struct VM {
    Chunk* chunk = nullptr;
    size_t ip;
    InterpretResult interpret(Chunk& newChunk);

private:
    InterpretResult run();
    uint8_t readByte();
    
};