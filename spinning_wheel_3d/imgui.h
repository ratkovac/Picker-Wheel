#pragma once
#include "libs.h"
#include "globals.h"

void Imgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Press TAB to edit");

    if (!gamePaused) {
        ImGui::BeginDisabled(true);
    }

    bool disableKeyboard = ImGui::IsKeyPressed(ImGuiKey_Tab);
    if (disableKeyboard) {
        ImGui::BeginDisabled();
    }

    ImGui::Text("Adjust the number of parts:");
    if (ImGui::SliderInt("Slider", &numParts, 1, 15)) {
        numParts = glm::clamp(numParts, 1, 15);
        sliceTexts.resize(numParts);
        for (int i = 0; i < numParts; ++i) {
            if (sliceTexts[i].empty()) {
                sliceTexts[i] = std::to_string(i + 1);
            }
        }
    }

    if (disableKeyboard) {
        ImGui::EndDisabled();
    }

    if (ImGui::InputInt("Input", &numParts, 0, 0, ImGuiInputTextFlags_CharsDecimal)) {
        numParts = glm::clamp(numParts, 1, 15);
        sliceTexts.resize(numParts);
        for (int i = 0; i < numParts; ++i) {
            if (sliceTexts[i].empty()) {
                sliceTexts[i] = std::to_string(i + 1);
            }
        }
    }

    if (ImGui::Button("Shuffle")) {
        static std::random_device rd;
        static std::mt19937 g(rd());
        std::shuffle(sliceTexts.begin(), sliceTexts.end(), g);
    }
    if (ImGui::Button("Start Rotation")) {
        startRotationSound();
        spinning = true;
        rotationStartTime = glfwGetTime();
        rotationSpeed = maxRotationSpeed;
    }

    if (ImGui::Button(showText ? "Hide Text" : "Show Text")) {
        showText = !showText;
    }
    ImGui::Text("Set custom text:");
    if (sliceTexts.size() != numParts) {
        sliceTexts.resize(numParts, "");
    }

    for (int i = 0; i < numParts;) {
        char buffer[128]; //privremeni bafer za unos
        snprintf(buffer, sizeof(buffer), "%s", sliceTexts[i].c_str()); //kopiram postojeći tekst u bafer

        ImGui::PushID(i); //jedinstveni ID za trenutni sektor
        if (ImGui::InputText("##Input", buffer, sizeof(buffer))) {
            sliceTexts[i] = std::string(buffer); //updateujem tekst ako je promijenjen
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            sliceTexts.erase(sliceTexts.begin() + i);
            --numParts;
            ImGui::PopID();
            continue; //preskacem `i++` jer je element obrisan
        }
        ImGui::PopID();
        ++i; //povećavam indeks samo ako sektor nije obrisan
    }

    if (!gamePaused) {
        ImGui::EndDisabled();
    }

    ImGui::End();
}

void Popup() {
    if (showPopup) {
        ImGui::OpenPopup("Results");
        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);

        if (ImGui::BeginPopupModal("Results", NULL)) {
            ImGui::SetWindowFontScale(2.0f);

            std::string fullText = "We have a winner!\n\t\t" + popupText;
            ImVec2 textSize = ImGui::CalcTextSize(fullText.c_str());

            ImVec2 winSize = ImGui::GetWindowSize();

            ImGui::SetCursorPosX((winSize.x - textSize.x) * 0.5f);
            ImGui::Text("%s", fullText.c_str());

            ImGui::Spacing();
            ImGui::Spacing();

            ImVec2 btnSize(100, 50);

            float btnX = (winSize.x - btnSize.x) * 0.5f;
            ImGui::SetCursorPosX(btnX);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 10.0f));
            if (ImGui::Button("OK", btnSize)) {
                showPopup = false;
                ImGui::CloseCurrentPopup();
                //showText = true;
            }
            ImGui::PopStyleVar();

            ImGui::EndPopup();
        }
    }
}