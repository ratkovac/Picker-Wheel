#include "libs.h"
#include "globals.h"
#include "audio.h"
#include "inputCallbacks.h"
#include "utils.h"
#include "text.h"
#include "objects.h"
#include "imgui.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    srand((unsigned int)time(0));

    generateRandomColors(sectorColors, numParts);
    currentNumParts = numParts;

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Picker Wheel", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Callbacks
    {
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    Shader mainShader("main.vs", "main.fs");
    Shader textureShader("texture.vs", "texture.fs");
    Shader textShader("text.vs", "text.fs");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    Freetype();

    Wheel();
    Room();
    Ring();
    Cube();
    Pyramid();
    Textures();
    Cursor();
    Pointer();

    glm::vec3 lightPos(1.5f, 2.0f, 2.0f);
    mainShader.use();
    mainShader.setVec3("lightPos", lightPos);
    mainShader.setVec3("viewPos", camera.Position);
    mainShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(9.0f, 10.0f);
    float rotationDuration = dis(gen);

    const double targetFrameTime = 1.0 / 60.0;
    double lastFrameTime = glfwGetTime();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window))
    {
        if (numParts > currentNumParts) {
            for (int i = currentNumParts; i < numParts; ++i) {
                generateRandomColor(&sectorColors[i * 3]);
            }
            currentNumParts = numParts;
        }
        else if (numParts < currentNumParts) {
            currentNumParts = numParts;
        }

        Imgui();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if (spinning)
        {
            float elapsedTime = currentFrame - rotationStartTime;
            if (elapsedTime < rotationDuration)
            {
                rotationSpeed = maxRotationSpeed * powf(1.0f - (elapsedTime / rotationDuration), 3.0f);
            }
            else
            {
                rotationSpeed = 0.0f;
                float pointerAngle = 90.0f;
                float effectiveAngle = fmod(pointerAngle - wheelAngle, 360.0f);
                if (effectiveAngle < 0)
                    effectiveAngle += 360.0f;

                float sliceAngle = 360.0f / numParts;
                int winningIndex = static_cast<int>(effectiveAngle / sliceAngle);
                popupText = sliceTexts[winningIndex];

                showPopup = true;
                stopRotationSound();
                startClappingSound();
                spinning = false;
            }
            wheelAngle += rotationSpeed;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainShader.use();

        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 100.0f
        );
        glm::mat4 view = camera.GetViewMatrix();

        mainShader.setMat4("projection", projection);
        mainShader.setMat4("view", view);

        //Walls
        {
            textureShader.use();

            for (int i = 0; i < 6; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                switch (i)
                {
                case 0: //Pod
                    model = glm::translate(model, glm::vec3(0.0f, -roomSize / 2.0f, 0.0f));
                    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                    break;
                case 1: //Plafon
                    model = glm::translate(model, glm::vec3(0.0f, roomSize / 2.0f, 0.0f));
                    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                    break;
                case 2: //Prednji zid
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -roomSize / 2.0f));
                    break;
                case 3: //Zadnji zid
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f, roomSize / 2.0f));
                    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    break;
                case 4: //Lijevi zid
                    model = glm::translate(model, glm::vec3(-roomSize / 2.0f, 0.0f, 0.0f));
                    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    break;
                case 5: //Desni zid
                    model = glm::translate(model, glm::vec3(roomSize / 2.0f, 0.0f, 0.0f));
                    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    break;
                }
                model = glm::scale(model, glm::vec3(roomSize));
                textureShader.setMat4("model", model);

                if (i == 0)
                    textureShader.setBool("useBrick", false);
                else
                    textureShader.setBool("useBrick", true);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, woodTexture);
                textureShader.setInt("woodTexture", 0);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, brickTexture);
                textureShader.setInt("brickTexture", 1);

                glBindVertexArray(roomVAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

        //Spinning wheel
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(wheelAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(3.0f));

            mainShader.use();
            mainShader.setMat4("model", model);
            mainShader.setInt("numParts", numParts);
            mainShader.setVec3Array("sectorColors", sectorColors, currentNumParts);
            mainShader.setBool("useSectorColors", true);
            mainShader.setFloat("wheelRotationAngle", glm::radians(wheelAngle));

            glBindVertexArray(wheelVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);

            mainShader.setBool("useSectorColors", false);
        }

        //Pointer
        {
            float arrowSectorAngle = 1.57f;

            glm::vec3 arrowPositionLocal(radius * cosf(arrowSectorAngle), radius * sinf(arrowSectorAngle), 0.0f);

            glm::mat4 arrowModel = glm::mat4(1.0f);
            arrowModel = glm::translate(arrowModel, glm::vec3(0.0f, -0.98f, -0.02f));
            arrowModel = glm::rotate(arrowModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            arrowModel = glm::scale(arrowModel, glm::vec3(3.0f));
            arrowModel = glm::translate(arrowModel, arrowPositionLocal);
            arrowModel = glm::rotate(arrowModel, arrowSectorAngle + glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            float arrowScale = 0.55f;
            arrowModel = glm::scale(arrowModel, glm::vec3(arrowScale));

            mainShader.use();
            mainShader.setMat4("model", arrowModel);
            mainShader.setBool("useSectorColors", false);
            mainShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

            glBindVertexArray(arrowVAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
        }

        //Bottom wheel
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.1f, 0.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(3.0f));

            mainShader.setMat4("model", model);

            mainShader.setBool("useSectorColors", false);
            mainShader.setVec4("objectColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

            glBindVertexArray(wheelVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);
        }

        //Ring
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.05f, 0.0f));
            model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f));

            mainShader.setMat4("model", model);
            mainShader.setBool("useSectorColors", false);
            mainShader.setVec3("objectColor", glm::vec3(0.2f, 0.4f, 0.6f));

            glBindVertexArray(ringVAO);
            glDrawElements(GL_TRIANGLES, ringIndices.size(), GL_UNSIGNED_INT, 0);
        }

        //Stand
        {
            standRotation += deltaTime * 20.0f;

            textureShader.use();
            textureShader.setMat4("view", view);
            textureShader.setMat4("projection", projection);
            textureShader.setVec3("lightPos", lightPos);
            textureShader.setVec3("viewPos", camera.Position);
            textureShader.setVec3("lightColor", glm::vec3(1.0f));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, woodTexture);
            textureShader.setInt("woodTexture", 0);
            textureShader.setBool("useBrick", false);

            for (int i = 0; i < 4; i++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, -5.4001f + i * 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(standRotation), glm::vec3(0.0f, 1.0f, 0.0f));
                textureShader.setMat4("model", model);

                glBindVertexArray(cubeVAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

            //Piramida
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.9f, 0.0f));
            model = glm::rotate(model, glm::radians(standRotation), glm::vec3(0.0f, 1.0f, 0.0f));
            textureShader.setMat4("model", model);

            glBindVertexArray(pyramidVAO);
            glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        }

        //Central disk and ring
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -0.97f, 0.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.32f));

            mainShader.use();
            mainShader.setMat4("model", model);
            mainShader.setBool("useSectorColors", false);
            mainShader.setVec3("objectColor", glm::vec3(1.0f, 0.3f, 0.2f));

            glBindVertexArray(wheelVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.1f, 0.3f, 0.1f));

            mainShader.setMat4("model", model);
            mainShader.setBool("useSectorColors", false);
            mainShader.setVec3("objectColor", glm::vec3(0.7f, 0.7f, 0.9f));

            glBindVertexArray(ringVAO);
            glDrawElements(GL_TRIANGLES, ringIndices.size(), GL_UNSIGNED_INT, 0);
        }

        //Cursor
        {
            glm::mat4 cursorProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
            glm::mat4 cursorModel = glm::mat4(1.0f);
            cursorModel = glm::translate(cursorModel, glm::vec3(0.0f, 0.0f, 0.0f));

            mainShader.use();
            mainShader.setMat4("projection", cursorProjection);
            mainShader.setMat4("view", glm::mat4(1.0f));
            mainShader.setMat4("model", cursorModel);
            mainShader.setBool("useSectorColors", false);
            mainShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

            glBindVertexArray(cursorVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        Popup();

        if (showText)
        {
            float sliceAngle = 2.0f * 3.1415926f / numParts;
            float effectiveRadius = 3.0f * radius;
            float textRadialPos = effectiveRadius * 0.22f;

            glm::mat4 baseModel = glm::mat4(1.0f);
            baseModel = glm::translate(baseModel, glm::vec3(0.0f, -0.9f, 0.0f));
            baseModel = glm::rotate(baseModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            baseModel = glm::scale(baseModel, glm::vec3(3.0f));

            for (int i = 0; i < numParts; i++)
            {
                float midAngle = (i + 0.5f) * sliceAngle;

                glm::vec3 offset(
                    textRadialPos * cos(midAngle),
                    textRadialPos * sin(midAngle),
                    0.0f
                );

                glm::mat4 textModel = glm::rotate(baseModel, glm::radians(wheelAngle), glm::vec3(0.0f, 0.0f, 1.0f));
                textModel = glm::translate(textModel, offset);

                glm::vec3 textPos = glm::vec3(textModel[3]);

                glm::mat4 billboard = glm::inverse(glm::mat4(glm::mat3(camera.GetViewMatrix())));

                textModel = glm::translate(glm::mat4(1.0f), textPos) * billboard;

                textModel = glm::rotate(textModel, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                RenderText3DCustom(textShader, sliceTexts[i], textModel, 0.0025f, glm::vec3(1.0f, 1.0f, 1.0f));
                glDisable(GL_BLEND);
            }
        }

        //GUI elementi
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        textShader.use();
        RenderText2D(textShader, "Nikola Ratkovac RA 158/2021", 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastFrameTime;
        if (deltaTime < targetFrameTime) {
            glfwWaitEventsTimeout(targetFrameTime - deltaTime);
            currentTime = glfwGetTime();
            deltaTime = currentTime - lastFrameTime;
        }
        lastFrameTime = currentTime;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &wheelVAO);
    glDeleteBuffers(1, &wheelVBO);

    glDeleteVertexArrays(1, &roomVAO);
    glDeleteBuffers(1, &roomVBO);
    glDeleteBuffers(1, &roomEBO);

    glDeleteVertexArrays(1, &ringVAO);
    glDeleteBuffers(1, &ringVBO);
    glDeleteBuffers(1, &ringEBO);

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    glDeleteVertexArrays(1, &pyramidVAO);
    glDeleteBuffers(1, &pyramidVBO);
    glDeleteBuffers(1, &pyramidEBO);

    glDeleteVertexArrays(1, &cursorVAO);
    glDeleteBuffers(1, &cursorVBO);
    glDeleteBuffers(1, &cursorEBO);

    glDeleteVertexArrays(1, &arrowVAO);
    glDeleteBuffers(1, &arrowVBO);

    glDeleteTextures(1, &woodTexture);
    glDeleteTextures(1, &brickTexture);

    glfwTerminate();
    return 0;
}