#include "chunk.h"
#include "value.h"
#include "common.h"
#include "vm.h"
#include "debug.h"
#include "compiler.h"

#include <iostream>
#include <string_view>
#include <functional>
#include <format>
#include <cassert>

void VM::push(Value value) {
    assert(stackTop < stack + STACK_MAX && "Stack overflow.");
    *stackTop = value;
    stackTop++;
}

Value VM::pop() {
    assert(stackTop > stack && "Stack underflow.");
    stackTop--;
    return *stackTop;
}

InterpretResult VM::interpret(std::string_view source) {

    Chunk chk;

    Compiler compiler(source);

    if(!compiler.compile(chk)) {
        return INTERPRET_COMPILE_ERROR;
    }

    chunk = &chk;
    ip = 0;
    stackTop = stack;

    InterpretResult result = run();
    chunk = nullptr;
    return result;
}

uint8_t VM::readByte() {
    return chunk->code[ip++];
}

InterpretResult VM::run() {

    for(;;) {
        if constexpr (DEBUG_TRACE_EXECUTION) {
            for (Value* slot = stack; slot < stackTop; slot++) {
                std::cout << std::format("[ {} ]", valueToString(*slot));
            }
            std::cout << "\n";
            disassembleInstruction(*chunk, ip);
        }

        uint8_t instruction;
        switch (instruction = readByte()) {
            case OP_CONSTANT: {
                Value constant = chunk->constants[readByte()];
                push(constant);
                break;
            }
            case OP_CONSTANT_LONG: { 
                uint8_t b1 = readByte(); // separate statements - multiple readByte() calls 
                uint8_t b2 = readByte(); // inside one expression have unspecified relative order. 
                uint8_t b3 = readByte();
                Value constant = chunk->constants[ (b1 << 16) | (b2 << 8) | b3];
                push(constant);
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
                std::cout << valueToString(pop());
                std::cout << "\n";
                return INTERPRET_OK;
            }
            default: {
                std::cerr << std::format("Unknown opcode {}\n", instruction);
                return INTERPRET_RUNTIME_ERROR;
            }
        }
    }
}