#pragma once

#include "common.h"
#include "value.h"

#include <vector>
#include <cstdint>

enum OpCode {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
};

//lines.size() is always equal to code.size(). Every byte of a multi-byte instruction has the line
//This allows the disassembler to compare lines[offset] against lines[offset - 1]. 
//Both of these can only be modified by write(), so this always holds. 

struct Chunk {

    std::vector<uint8_t> code;
    std::vector<int> lines;
    std::vector<Value> constants;

    void write(uint8_t byte, int line);
    void writeConstant(Value value, int line);
};

