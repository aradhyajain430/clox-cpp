#include "debug.h"
#include "chunk.h"
#include "value.h"

#include <iostream>
#include <format>

void disassembleChunk(const Chunk& chunk, const char* name){
    std::cout << "== " << name << " ==\n";
    for (size_t offset = 0; offset < chunk.code.size(); ){
        offset = disassembleInstruction(chunk, offset);
    }
}

static size_t simpleInstruction(const char* name, size_t offset){
    std::cout << name << "\n";
    return offset + 1;
}

static size_t longConstantInstruction(const char* name, const Chunk& chunk, size_t offset){

    uint32_t constantIndex = (chunk.code[offset + 1]<<16) | (chunk.code[offset + 2]<<8) | (chunk.code[offset + 3]);
    std::cout << std::format("{:<16} {:4d} '{}'\n", name, constantIndex, 
                                        valueToString(chunk.constants[constantIndex]));
    return offset+4;
}


static size_t constantInstruction(const char* name, const Chunk& chunk, size_t offset){

    uint8_t constantIndex = chunk.code[offset + 1];
    std::cout << std::format("{:<16} {:4d} '{}'\n", name, constantIndex, 
                                        valueToString(chunk.constants[constantIndex]));
    return offset+2;
}

size_t disassembleInstruction(const Chunk& chunk, size_t offset){
    std::cout << std::format("{:04d} ", offset);

    //lines[offset-1] is the previous byte, because code and lines are byte parallel
    //and every byte of an opcode has the same line. 
    if (offset > 0 && chunk.lines[offset] == chunk.lines[offset - 1]) {
        std::cout << "   | ";
    }
    else{
        std::cout << std::format("{:4d} ", chunk.lines[offset]);
    }

    uint8_t instruction = chunk.code[offset];
    switch (instruction){
        case OP_CONSTANT_LONG:  return longConstantInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_CONSTANT:       return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_NEGATE:         return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:       return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:       return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:         return simpleInstruction("OP_DIVIDE", offset);
        case OP_RETURN:         return simpleInstruction("OP_RETURN", offset);
    }

    return offset + 1;
}

