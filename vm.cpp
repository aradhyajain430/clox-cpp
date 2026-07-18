#include "chunk.h"
#include "value.h"
#include "common.h"
#include "vm.h"
#include "debug.h"
#include "compile.h"

#include <iostream>
#include <string_view>

void VM::push(Value value) {
    *stackTop = value;
    stackTop++;
}

Value VM::pop() {
    stackTop--;
    return *stackTop;
}

InterpretResult VM::interpret(std::string_view source) {

    Compiler compiler(source);

    compiler.compile();
    return INTERPRET_OK;
}

uint8_t VM::readByte() {
    return chunk->code[ip++];
}

InterpretResult VM::run() {

    for(;;) {
        if constexpr (DEBUG_TRACE_EXECUTION) {
            for (Value* slot = stack; slot < stackTop; slot++) {
                std::cout << "[ ";
                printValue(*slot);
                std::cout << " ]";
            }
            std::cout << "\n";
            disassembleInstruction(*chunk, ip);
        }

        uint8_t instruction;
        switch (instruction = readByte()) {
            case OP_CONSTANT: {
                Value constant = chunk->constants[readByte()];
                push(constant);
                std::cout << "\n";
                break;
            }
            case OP_CONSTANT_LONG: {
                uint8_t b1 = readByte();
                uint8_t b2 = readByte();
                uint8_t b3 = readByte();
                Value constant = chunk->constants[ (b1 << 16) | (b2 << 8) | b3];
                push(constant);
                std::cout << "\n";
                break;
            }
            case OP_NEGATE: {
                push(-pop());
                break;
            }
            case OP_ADD: {
                binaryOp(std::plus<>{});
                break;
            }
            case OP_SUBTRACT: {
                binaryOp(std::minus<>{});
                break;
            }
            case OP_MULTIPLY: {
                binaryOp(std::multiplies<>{});
                break;
            }
            case OP_DIVIDE: {
                binaryOp(std::divides<>{});
                break;
            }
            case OP_RETURN: {
                printValue(pop());
                std::cout << "\n";
                return INTERPRET_OK;
            }
        }
    }
}