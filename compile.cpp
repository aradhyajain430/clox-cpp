#include "common.h"
#include "scanner.h"
#include "compile.h"

#include <iostream>
#include <format>
#include <string>

Compiler::Compiler(std::string_view src) 
    : scanner(src) {
}

void Compiler::compile() {
    int line = -1;
    for (;;) {
        Token token = scanner.scanToken();
        if (token.line != line){
            std::cout << std::format("{:4d} ", token.line);
            line = token.line;
        }
        else {
            std::cout << "   | ";
        }
        std::cout << std::format("{:2} '{}'", static_cast<int>(token.type), token.lexeme);

        if (token.type == TOKEN_EOF) {
            break;
        }
    }
}