#pragma once

#include "chunk.h"

void disassembleChunk(const Chunk& chunk, const char* name);

//returns offset of the next instruction since they're variable length. 
size_t disassembleInstruction(const Chunk& chunk, size_t offset);