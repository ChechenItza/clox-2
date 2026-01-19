#include <stdlib.h>
#include <math.h>

#include "chunk.h"

#include <string.h>

#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initRleArray(&chunk->lines);
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeRleArray(&chunk->lines);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, const uint8_t byte, const int line) {
    if (chunk->capacity < chunk->count + 1) {
        const int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    writeRleArray(&chunk->lines, line);
    chunk->count++;
}

void writeConstant(Chunk* chunk, const Value value, const int line) {
    const int constIdx = addConstant(chunk, value);

    if (chunk->capacity < chunk->count + 4) {
        const int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    if (constIdx >= pow(2, 8)) {
        unsigned char numberBytes[3];
        memcpy(numberBytes, &constIdx, 3);

        chunk->code[chunk->count] = OP_CONSTANT_LONG;
        chunk->code[chunk->count + 1] = numberBytes[0];
        chunk->code[chunk->count + 2] = numberBytes[1];
        chunk->code[chunk->count + 3] = numberBytes[2];
        chunk->count += 4;
    } else {
        chunk->code[chunk->count] = OP_CONSTANT;
        chunk->code[chunk->count + 1] = constIdx;
        chunk->count += 2;
    }

    writeRleArray(&chunk->lines, line);
}

int addConstant(Chunk* chunk, const Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}