#pragma once
#include "libs.h"
#include "globals.h"

void generateRandomColors(float* colors, int numParts) {
    for (int i = 0; i < numParts; ++i) {
        colors[i * 3] = (float)rand() / RAND_MAX;
        colors[i * 3 + 1] = (float)rand() / RAND_MAX;
        colors[i * 3 + 2] = (float)rand() / RAND_MAX;
    }
}

void generateRandomColor(float* color) {
    color[0] = (float)rand() / RAND_MAX;
    color[1] = (float)rand() / RAND_MAX;
    color[2] = (float)rand() / RAND_MAX;
}