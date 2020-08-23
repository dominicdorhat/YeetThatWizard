#include "../header.h"

#include "PauseState.h"

PauseState::PauseState(Hfyda2Engine& rEngine, RunningState::LevelData sLevelData, RunningState::PlayerData sPlayerData)
    :   m_levelData(sLevelData),
        m_playerData(sPlayerData) { 

    rEngine.pause();
    rEngine.notifyObjectsAboutKeys(false);
}

PauseState::~PauseState() {}

void PauseState::setupBackground(Hfyda2Engine& rEngine) {    
}

void PauseState::setupObjects(Hfyda2Engine& rEngine) {
}

void PauseState::drawStringsOnTop(Hfyda2Engine& rEngine) {
    rEngine.drawForegroundString(rEngine.getWindowWidth() / 2 - 100, rEngine.getWindowHeight() / 2 - 48, "Paused", 0xffffff, rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 96));

}

void PauseState::keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) {
    switch(iKeyCode) {
        case SDLK_ESCAPE:

            this->setState(rEngine, new RunningState(&rEngine, m_levelData, m_playerData));            
            rEngine.notifyObjectsAboutKeys(true);
            rEngine.setAllObjectsVisible(true);
            rEngine.unpause();

            break; 
    }
}