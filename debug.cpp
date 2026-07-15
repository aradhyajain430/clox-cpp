#include "debug.h"
#include "chunk.h"
#include "value.h"

#include <iostream>
#include <format>

void disassembleChunk(const Chunk& chunk, const char* name){
    std::cout << "== " << name << " ==\n";
    for (int offset = 0; offset < chunk.code.size(); ){
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset){
    std::cout << name << "\n";
    return offset + 1;
}

static int longConstantInstruction(const char* name, const Chunk& chunk, int offset){

    uint32_t constantIndex = (chunk.code[offset + 1]<<16) | (chunk.code[offset + 2]<<8) | (chunk.code[offset + 3]);
    std::cout << std::format("{:<16} {:4d} '", name, constantIndex);
    printValue(chunk.constants[constantIndex]);
    std::cout << "'\n";
    return offset+4;
}


static int constantInstruction(const char* name, const Chunk& chunk, int offset){

    uint8_t constantIndex = chunk.code[offset + 1];
    std::cout << std::format("{:<16} {:4d} '", name, constantIndex);
    printValue(chunk.constants[constantIndex]);
    std::cout << "'\n";
    return offset+2;
}

int disassembleInstruction(const Chunk& chunk, int offset){
    std::cout << std::format("{:04d} ", offset);

    if (offset > 0 && chunk.lines[offset] == chunk.lines[offset - 1]) {
        std::cout << "   | ";
    }
    else{
        std::cout << std::format("{:4d} ", chunk.lines[offset]);
    }

    uint8_t instruction = chunk.code[offset];
    switch (instruction){
        case OP_CONSTANT_LONG: {
            return longConstantInstruction("OP_CONSTANT_LONG", chunk, offset);
        }
        case OP_CONSTANT: {
            return constantInstruction("OP_CONSTANT", chunk, offset);
        }
        case OP_RETURN: {
            return simpleInstruction("OP_RETURN", offset);
        }
        default:
            std::cout << "Unknown OpCode " << (int)instruction << "\n";
            return offset + 1;
    }

    return offset + 1;
}

