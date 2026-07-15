#include "chunk.h"
#include "common.h"
#include "vm.h"


InterpretResult VM::interpret(Chunk& newChunk) {
    this->chunk = &newChunk;
    ip = 0;
    return run();
}

uint8_t VM::readByte() {
    return chunk->code[this->ip++];
}

InterpretResult VM::run() {
    for(;;) {
        uint8_t instruction;
        switch (instruction = readByte()) {
            case OP_RETURN: {
                return INTERPRET_OK;
            }
        }
    }
}