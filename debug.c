#include <stdio.h>

#include "debug.h"
#include "value.h"

static int constantInstruction(const Chunk* chunk, const int offset) {
    const uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", "OP_CONSTANT", constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

static int constantLongInstruction(const Chunk* chunk, const int offset) {
    const unsigned char bytes[] = { chunk->code[offset + 1], chunk->code[offset + 2], chunk->code[offset + 3] };
    const int idx = ((bytes[2] << 16) | (bytes[1] << 8) | (bytes[0]));
    printf("%-16s %4d '", "OP_CONSTANT_LONG", idx);
    printValue(chunk->constants.values[idx]);
    printf("'\n");
    return offset + 4;
}

static int simpleInstruction(const char* name, const int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(const Chunk* chunk, const int offset, const int idx) {
    printf("%04d ", offset);
    if (offset > 0 && cmpIdxRleArray(&chunk->lines, idx, idx-1) == 1) {
        printf("   | ");
    } else {
        printf("%4d ", getFromRleArray(&chunk->lines, idx));
    }

    const uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction(chunk, offset);
        case OP_CONSTANT_LONG:
            return constantLongInstruction(chunk, offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

void disassembleChunk(const Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    int i = 0;
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset, i);
        i++;
    }
}

void disassembleLines(const Chunk* chunk, const char* name) {
    printf("== %s lines ==\n", name);
    for (int i = 0; i < chunk->lines.count; i += 2) {
        printf("%d, %d; ", chunk->lines.values[i], chunk->lines.values[i+1]);
    }
    printf("\n");
}