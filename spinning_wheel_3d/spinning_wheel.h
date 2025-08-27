#include "libs.h"
#include "fields.h"

void wheel() {
    std::vector<float> wheelVertices;
    wheelVertices.reserve((numSegments + 2) * 3);

    // center of the disk:
    wheelVertices.push_back(0.0f);  // x
    wheelVertices.push_back(0.0f);  // y
    wheelVertices.push_back(0.0f);  // z

    // perimeter
    for (int i = 0; i <= numSegments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        wheelVertices.push_back(x);
        wheelVertices.push_back(y);
        wheelVertices.push_back(0.0f);
    }

    glGenVertexArrays(1, &wheelVAO);
    glGenBuffers(1, &wheelVBO);

    glBindVertexArray(wheelVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wheelVBO);
    glBufferData(GL_ARRAY_BUFFER, wheelVertices.size() * sizeof(float),
        wheelVertices.data(), GL_STATIC_DRAW);

    // attribute 0 -> positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}