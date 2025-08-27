#pragma once
#include "libs.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0f;
float lastY = (float)SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//VAO, VBO, EBO
unsigned int wheelVAO, wheelVBO;
unsigned int roomVAO, roomVBO, roomEBO;
unsigned int ringVAO, ringVBO, ringEBO;
unsigned int cursorVAO, cursorVBO, cursorEBO;
unsigned int textVAO, textVBO;
unsigned int arrowVAO, arrowVBO;
unsigned int cubeVAO, cubeVBO, cubeEBO;
unsigned int pyramidVAO, pyramidVBO, pyramidEBO;

unsigned int woodTexture, brickTexture;
bool gamePaused = false;

//Text
struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};
std::map<GLchar, Character> Characters;
bool showPopup = false;
std::string popupText = "";
bool showText = true;
std::vector<std::string> sliceTexts;

//Wheel rotation
float wheelAngle = 0.0f;
bool spinning = false;
float rotationSpeed = 0.0f;
float maxRotationSpeed = 50.0f;
float rotationStartTime = 0.0f;
int numSegments = 100;
float radius = 0.5f;
int numParts = 1;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float standRotation = 0.0f;

//Walls
float roomSize = 10.0f;
const float roomBoundary = roomSize / 2.05f;

//Ring
std::vector<float> ringVertices;
std::vector<unsigned int> ringIndices;
int ringMajorSegments = 100;
int ringMinorSegments = 50;
float ringMajorRadius = radius * 3.22f;
float ringMinorRadius = 0.12f;

//Wheel
std::vector<float> wheelVertices;
const int MAX_PARTS = 15;
float sectorColors[MAX_PARTS * 3] = { 0.0f };
int currentNumParts = 0;

//Pointer
float tipHeight = 0.25f;
float baseWidth = 0.2f;