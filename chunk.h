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

struct Chunk {

    std::vector<uint8_t> code;
    std::vector<int> lines;
    std::vector<Value> constants;

    void write(uint8_t byte, int line);
    void writeConstant(Value value, int line);
};

