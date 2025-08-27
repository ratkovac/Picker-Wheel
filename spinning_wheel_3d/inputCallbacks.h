#pragma once
#include "libs.h"
#include "globals.h"

void processInput(GLFWwindow* window)
{
    if (gamePaused) return;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 newPosition = camera.Position;

    //Pomijeranje kamere unaprijed
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        newPosition += camera.Front * deltaTime * 3.5f;

    //Unazad
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        newPosition -= camera.Front * deltaTime * 3.5f;

    //Lijevo
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        newPosition -= camera.Right * deltaTime * 3.5f;

    //Desno
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        newPosition += camera.Right * deltaTime * 3.5f;

    //Gore
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        newPosition += camera.Up * deltaTime * 3.5f;

    //Dole
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        newPosition -= camera.Up * deltaTime * 3.5f;

    //Ograničenje kamere unutar sobe
    newPosition.x = glm::clamp(newPosition.x, -roomBoundary, roomBoundary);
    newPosition.y = glm::clamp(newPosition.y, -roomBoundary, roomBoundary);
    newPosition.z = glm::clamp(newPosition.z, -roomBoundary, roomBoundary);

    camera.Position = newPosition;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        spinning = true;
        rotationStartTime = glfwGetTime();
        rotationSpeed = maxRotationSpeed;
        startRotationSound();
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && spinning) {
        spinning = false;
        rotationSpeed = 0.0f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (gamePaused) return;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (gamePaused) return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        float screenX = SCR_WIDTH / 2.0f;
        float screenY = SCR_HEIGHT / 2.0f;

        glm::vec3 worldPos(0.0f, -0.97f, 0.0f);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f,
            100.0f
        );
        glm::vec4 viewport(0.0f, 0.0f, SCR_WIDTH, SCR_HEIGHT);

        glm::vec3 screenPos = glm::project(worldPos, view, projection, viewport);

        float distance = glm::distance(
            glm::vec2(screenX, screenY),
            glm::vec2(screenPos.x, screenPos.y)
        );

        float clickRadius = 30.0f;

        if (distance <= clickRadius) {
            startRotationSound();
            spinning = true;
            rotationStartTime = glfwGetTime();
            rotationSpeed = maxRotationSpeed;
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        if (spinning) {
            spinning = false;
            rotationSpeed = 0.0f;

            float pointerAngle = 90.0f;
            float effectiveAngle = fmod(pointerAngle - wheelAngle, 360.0f);
            if (effectiveAngle < 0) effectiveAngle += 360.0f;

            float sliceAngle = 360.0f / numParts;
            int winningIndex = static_cast<int>(effectiveAngle / sliceAngle);
            popupText = sliceTexts[winningIndex];
            showPopup = true;

            stopRotationSound();
            startClappingSound();
        }
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        numParts = (numParts % 15) + 1;
        sliceTexts.resize(numParts);
        for (int i = 0; i < numParts; ++i) {
            if (sliceTexts[i].empty()) {
                sliceTexts[i] = std::to_string(i + 1);
            }
        }
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        if (numParts > 1) {
            numParts = numParts - 1;
            sliceTexts.resize(numParts);
            for (int i = 0; i < numParts; ++i) {
                if (sliceTexts[i].empty()) {
                    sliceTexts[i] = std::to_string(i + 1);
                }
            }
        }
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        gamePaused = !gamePaused;

        if (gamePaused) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            firstMouse = true;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
        }
    }
}