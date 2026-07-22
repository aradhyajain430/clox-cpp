#include "chunk.h"

#include <cassert>

void Chunk::write(uint8_t byte, int line) {
    code.push_back(byte);
    lines.push_back(line);
}

void Chunk::writeConstant(Value value, int line) {
    constants.push_back(value);
    size_t index = constants.size() - 1;
    assert(index < (1u << 24) && "Constant pool exceeded 2^24 entries.");

    // 3 byte index lets you handle 2^24 constants at max. 
    if(index > 255){
        write(OP_CONSTANT_LONG, line);
        //high byte first, debug.cpp and vm.cpp reassemble in the same order.
        write((index >> 16) & 0xFF, line); 
        write((index >> 8)  & 0xFF, line);
        write( index        & 0xFF, line);
    }
    else{
        write(OP_CONSTANT, line);
        write(index, line);
    }
}