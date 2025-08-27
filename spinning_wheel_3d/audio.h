#pragma once
#include "libs.h"

using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice();

void startRotationSound() {
    SoundEngine->stopAllSounds();
    SoundEngine->play2D(FileSystem::getPath("resources/audio/jeopardy.ogg").c_str(), false);
    
    SoundEngine->play2D(FileSystem::getPath("resources/audio/wheel.ogg").c_str(), false);
}

void startClappingSound() {
    SoundEngine->stopAllSounds();
    SoundEngine->play2D(FileSystem::getPath("resources/audio/clapping.ogg").c_str(), false);
}

void stopRotationSound() {
    SoundEngine->stopAllSounds();
}