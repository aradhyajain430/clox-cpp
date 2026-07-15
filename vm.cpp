#include "chunk.h"
#include "value.h"
#include "common.h"
#include "vm.h"

#include <iostream>


InterpretResult VM::interpret(Chunk& newChunk) {
    chunk = &newChunk;
    ip = 0;
    return run();
}

uint8_t VM::readByte() {
    return chunk->code[ip++];
}

InterpretResult VM::run() {
    for(;;) {
        uint8_t instruction;
        switch (instruction = readByte()) {
            case OP_CONSTANT: {
                Value constant = chunk->constants[readByte()];
                printValue(constant);
                std::cout << "\n";
                break;
            }
            case OP_CONSTANT_LONG: {
                uint8_t b1 = readByte();
                uint8_t b2 = readByte();
                uint8_t b3 = readByte();
                Value constant = chunk->constants[ (b1 << 16) | (b2 << 8) | b3];
                printValue(constant);
                std::cout << "\n";
                break;
            }
            case OP_RETURN: {
                return INTERPRET_OK;
            }
        }
    }
}