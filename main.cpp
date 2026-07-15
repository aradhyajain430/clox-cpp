#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

#include <vector>
#include <string>

int main(int argc, const char* argv[]){
    const std::vector<std::string> args(argv, argv+argc);

    VM vm;

    Chunk chunk;

    for (int i = 0; i < 300; i++) {
        chunk.writeConstant(i, 1);
    }
    chunk.write(OP_RETURN, 1);



    disassembleChunk(chunk, "test chunk");
    vm.interpret(chunk);
    return 0;
}

