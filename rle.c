#include "common.h"
#include "memory.h"
#include "rle.h"

#include "value.h"

void initRleArray(RleArray* array) {
    array->capacity = 0;
    array->count = 0;
    array->values = NULL;
}

void writeRleArray(RleArray* array, const int value) {
    if (array->capacity < array->count + 2) {
        const int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(int, array->values, oldCapacity, array->capacity);
    }

    if (array->count == 0) {
        array->values[array->count] = value;
        array->values[array->count+1] = 1;
        array->count++;
    } else if (array->values[array->count-1] == value) {
        array->values[array->count]++;
    } else {
        array->values[array->count+1] = value;
        array->values[array->count+2] = 1;
        array->count += 2;
    }
}

int getFromRleArray(const RleArray* array, const int idx) {
    int runningLen = 0;
    for (int i = 0; i < array->count; i += 2) {
        runningLen += array->values[i + 1];
        if (runningLen > idx) {
            return array->values[i];
        }
    }

    return -1;
}

int cmpIdxRleArray(const RleArray* array, const int idx1, const int idx2) {
    if (getFromRleArray(array, idx1) == getFromRleArray(array, idx2)) {
        return 1;
    }

    return 0;
}

void freeRleArray(RleArray* array) {
    FREE_ARRAY(int, array->values, array->capacity);
    initRleArray(array);
}
