#include "chunk.h"

void Chunk::write(uint8_t byte, int line) {
    code.push_back(byte);
    lines.push_back(line);
}

void Chunk::writeConstant(Value value, int line) {
    constants.push_back(value);
    int index = constants.size() - 1;
    if(index > 255){
        write(OP_CONSTANT_LONG, line);
        write((index >> 16) & 0xFF, line);
        write((index >> 8)  & 0xFF, line);
        write( index        & 0xFF, line);
    }
    else{
        write(OP_CONSTANT, line);
        write(index, line);
    }
}