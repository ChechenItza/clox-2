#ifndef CLOX_RLE_H
#define CLOX_RLE_H

typedef struct {
    int capacity;
    int count;
    int* values;
} RleArray;

void initRleArray(RleArray* array);
void writeRleArray(RleArray* array, int value);
void freeRleArray(RleArray* array);
int getFromRleArray(const RleArray* array, int idx);
int cmpIdxRleArray(const RleArray* array, int idx1, int idx2);

#endif //CLOX_RLE_H