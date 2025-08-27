#pragma once
#include "libs.h"
#include "globals.h"

void Wheel()
{
    wheelVertices.reserve((numSegments + 2) * 3);

    wheelVertices.push_back(0.0f);
    wheelVertices.push_back(0.0f);
    wheelVertices.push_back(0.0f);

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
    glBufferData(GL_ARRAY_BUFFER, wheelVertices.size() * sizeof(float), wheelVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Room()
{
    float roomVertices[] = {
        -0.5f,  0.5f,  0.0f,      0.0f, 1.0f,
         0.5f,  0.5f,  0.0f,      1.0f, 1.0f,
         0.5f, -0.5f,  0.0f,      1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f,      0.0f, 0.0f
    };

    unsigned int roomIndices[] = {
    0, 2, 1,
    0, 3, 2
    };

    glGenVertexArrays(1, &roomVAO);
    glGenBuffers(1, &roomVBO);
    glGenBuffers(1, &roomEBO);

    glBindVertexArray(roomVAO);

    glBindBuffer(GL_ARRAY_BUFFER, roomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roomVertices), roomVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roomEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roomIndices), roomIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Ring()
{
    for (int i = 0; i <= ringMajorSegments; i++) {
        float majorAngle = 2.0f * 3.1415926f * float(i) / float(ringMajorSegments);
        glm::vec3 majorPosition = glm::vec3(
            cosf(majorAngle) * ringMajorRadius,
            0.0f,
            sinf(majorAngle) * ringMajorRadius
        );

        for (int j = 0; j <= ringMinorSegments; j++) {
            float minorAngle = 2.0f * 3.1415926f * float(j) / float(ringMinorSegments);
            glm::vec3 minorPosition = glm::vec3(
                cosf(minorAngle) * ringMinorRadius,
                sinf(minorAngle) * ringMinorRadius,
                0.0f
            );

            glm::vec3 vertexPosition = majorPosition + glm::vec3(
                minorPosition.x * cosf(majorAngle),
                minorPosition.y,
                minorPosition.x * sinf(majorAngle)
            );

            ringVertices.push_back(vertexPosition.x);
            ringVertices.push_back(vertexPosition.y);
            ringVertices.push_back(vertexPosition.z);
        }
    }

    for (int i = 0; i < ringMajorSegments; i++) {
        for (int j = 0; j < ringMinorSegments; j++) {
            int current = i * (ringMinorSegments + 1) + j;
            int next = (i + 1) * (ringMinorSegments + 1) + j;

            ringIndices.push_back(current);
            ringIndices.push_back(current + 1);
            ringIndices.push_back(next);

            ringIndices.push_back(current + 1);
            ringIndices.push_back(next + 1);
            ringIndices.push_back(next);
        }
    }

    glGenVertexArrays(1, &ringVAO);
    glGenBuffers(1, &ringVBO);
    glGenBuffers(1, &ringEBO);

    glBindVertexArray(ringVAO);

    glBindBuffer(GL_ARRAY_BUFFER, ringVBO);
    glBufferData(GL_ARRAY_BUFFER, ringVertices.size() * sizeof(float), ringVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ringEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ringIndices.size() * sizeof(unsigned int), ringIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Cube() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,    0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,    0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,    0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,    0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,    0.0f,  0.0f, 1.0f,
    };

    unsigned int indices[] = {
        //Front
        4, 5, 6,
        6, 7, 4,
        //Back
        0, 3, 2,
        2, 1, 0,
        //Left
        0, 4, 7,
        7, 3, 0,
        //Right
        1, 2, 6,
        6, 5, 1,
        //Top
        3, 7, 6,
        6, 2, 3,
        //Bottom
        0, 1, 5,
        5, 4, 0
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Pyramid() {
    float vertices[] = {
        -0.5f, 0.0f, -0.5f,   0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
         0.5f, 0.0f, -0.5f,   1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
         0.5f, 0.0f,  0.5f,   1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        -0.5f, 0.0f,  0.5f,   0.0f, 1.0f,  0.0f, -1.0f, 0.0f,

         0.0f, 0.8f,  0.0f,   0.5f, 0.5f,  0.0f, 0.8f, 0.0f
    };

    unsigned int indices[] = {
        //Base
        0, 2, 1,
        2, 0, 3,

        //Front
        0, 4, 1,
        //Right
        1, 4, 2,
        //Back
        2, 4, 3,
        //Left
        3, 4, 0
    };

    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &pyramidVBO);
    glGenBuffers(1, &pyramidEBO);

    glBindVertexArray(pyramidVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Textures()
{
    int width, height, nrChannels;
    glGenTextures(1, &woodTexture);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data1 = stbi_load(FileSystem::getPath("resources/textures/wood.png").c_str(), &width, &height, &nrChannels, 0);
    if (data1)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);

    glGenTextures(1, &brickTexture);
    glBindTexture(GL_TEXTURE_2D, brickTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* data2 = stbi_load(FileSystem::getPath("resources/textures/brickwall.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data2)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load brick texture" << std::endl;
    }
    stbi_image_free(data2);
}

void Cursor()
{
    float cursorVertices[] = {
        -0.01f,  0.01f, 0.0f,
         0.01f,  0.01f, 0.0f,
         0.01f, -0.01f, 0.0f,
        -0.01f, -0.01f, 0.0f
    };

    unsigned int cursorIndices[] = {
    0, 2, 1,
    0, 3, 2
    };

    glGenVertexArrays(1, &cursorVAO);
    glGenBuffers(1, &cursorVBO);
    glGenBuffers(1, &cursorEBO);

    glBindVertexArray(cursorVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cursorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cursorVertices), cursorVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cursorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cursorIndices), cursorIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Pointer()
{
    float arrowVertices[] = {
         0.0f,      tipHeight,  0.0f,
        -baseWidth * 0.5f,  0.0f,   0.0f,
         baseWidth * 0.5f,  0.0f,   0.0f
    };

    glGenVertexArrays(1, &arrowVAO);
    glGenBuffers(1, &arrowVBO);

    glBindVertexArray(arrowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, arrowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrowVertices), arrowVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}