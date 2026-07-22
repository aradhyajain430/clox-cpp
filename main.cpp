#include "common.h"
#include "vm.h"

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <utility>

static void repl(VM& vm){ 
    std::string line;
    for (;;) {
        std::cout << "> ";

        if (!std::getline(std::cin, line)) {
            std::cout << "\n";
            break;
        }

        vm.interpret(line);
    }
}

static std::string readFile(const char* path) {

    //binary file, which may contain a \0. This is why Scanner::match tests size().
    std::ifstream file(path, std::ios::binary);

    if (!file) {
        throw std::runtime_error(std::string("Could not open file: ") + path);
    }

    std::stringstream ss;
    ss << file.rdbuf();

    return std::move(ss).str();
}

static int runFile(const char* path, VM& vm) { // int to report exit code

    //SOURCE OWNER. The scanner's 'source' and EVERY TOKEN (except TOKEN_ERROR)
    //IS A VIEW INTO THIS STRING. NO TOKEN SHOULD ESCAPE THIS SCOPE. 
    std::string source = readFile(path);
    InterpretResult result = vm.interpret(source);

    if (result == INTERPRET_COMPILE_ERROR) {
        return 65;
    }
    if (result == INTERPRET_RUNTIME_ERROR) {
        return 70;
    }
    return 0;
}

int main(int argc, char* argv[]){

    if (argc > 2) {
        std::cerr << "Usage: clox [path]\n";
        return 64;
    }

    VM vm;

    if (argc <= 1) {
        repl(vm);
    }
    else {
        try{
            return runFile(argv[1], vm);
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
            return 74;
        }
    }
    return 0;
}

