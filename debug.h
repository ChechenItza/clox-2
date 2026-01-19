#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"

void disassembleChunk(const Chunk* chunk, const char* name);
int disassembleInstruction(const Chunk* chunk, int offset, int idx);
void disassembleLines(const Chunk* chunk, const char* name);

#endif //CLOX_DEBUG_H