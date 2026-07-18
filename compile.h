#pragma once

#include "scanner.h"

#include <string_view>

class Compiler {

    Scanner scanner;

    public:
    Compiler(std::string_view source);

    void compile();
};