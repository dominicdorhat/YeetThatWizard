#include "../header.h"

#include "NewGameState.h"
#include "RunningState.h"
#include "StartState.h"
#include "../components/Button.h"

#include <fstream>
#include <sstream>

NewGameState::NewGameState()
    :   m_sPlayerName(std::string("Guest")),
        m_oFlip() { 
}

NewGameState::~NewGameState() { }

void NewGameState::setupBackground(Hfyda2Engine& rEngine) {

    SimpleImage backgroundImage = rEngine.loadImage("resources/images/background/Background.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    backgroundImage.renderImage(rEngine.getBackgroundSurface(), 0, 0, 0,0, rEngine.getWindowWidth(), rEngine.getWindowHeight());

    SimpleImage middlegroundImage = rEngine.loadImage("resources/images/background/Middleground.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    middlegroundImage.renderImageWithMask(rEngine.getBackgroundSurface(), 0, 0, 0, 0, rEngine.getWindowWidth(), rEngine.getWindowHeight(), 0x000000);

    SimpleImage cloudImage = rEngine.loadImage("resources/images/background/cloud.png", true);
    cloudImage.renderImageApplyingMapping(&rEngine, rEngine.getBackgroundSurface(), 100, 100, cloudImage.getWidth(), cloudImage.getHeight(), &m_oFlip);

    cloudImage.renderImage(rEngine.getBackgroundSurface(), 0, 0, 600, 50, cloudImage.getWidth(), cloudImage.getHeight());

    rEngine.redrawDisplay();
}

void NewGameState::setupObjects(Hfyda2Engine& rEngine) {
    rEngine.notifyObjectsAboutMouse(true);
    rEngine.drawableObjectsChanged();
    rEngine.destroyOldObjects(true);
    rEngine.createObjectArray(2);
    rEngine.storeObjectInArray(0,
        new Button(rEngine, 100, rEngine.getWindowHeight() - 100, 66, 36, "Back", 0xffffff, 0xDCDCDC, this, &NewGameStateHelpers::switchScreenToMenu, 36));
    rEngine.storeObjectInArray(1,
        new Button(rEngine, 200, rEngine.getWindowHeight() - 100, 66, 36, "Play", 0xffffff, 0xDCDCDC, this, &NewGameStateHelpers::switchScreenToGame, 36));
}

void NewGameState::keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) {

    if(m_sPlayerName.size() < 10) {
        if(iKeyCode >= SDLK_a && iKeyCode <= SDLK_z ) {
            m_sPlayerName.push_back(char(iKeyCode));
        }
    }

    if(m_sPlayerName.size() && iKeyCode == SDLK_BACKSPACE) { 
        m_sPlayerName.pop_back();
        // m_sPlayerName.erase(std::begin(m_sPlayerName) + m_sPlayerName.size() - 1);
    }

    rEngine.redrawDisplay();
    return;
}

void NewGameState::drawStringsOnTop(Hfyda2Engine& rEngine) {  
    // rEngine.drawForegroundRectangle(96, 96, rEngine.getWindowWidth() - 96, rEngine.getWindowHeight() - 96, 0x0099db);

    if(m_sPlayerName.size()) rEngine.drawForegroundString(300, 100, (m_sPlayerName + "|").c_str() , 0x000000, 0);
    rEngine.drawForegroundString(100, 100, "Enter Name: ", 0xffffff, 0);

    rEngine.drawForegroundString(100, 200, "Controls: ", 0x000000, 0);
    rEngine.drawForegroundString(250, 200, "WASD to move, Spacebar to shoot", 0xffffff, 0);

    rEngine.drawForegroundString(100, 250, "Objective: ", 0x000000, 0);
    rEngine.drawForegroundString(250, 250, "Get to the green flag, avoid the meteors!", 0xffffff, 0);
    rEngine.drawForegroundString(100, 300, "           If it's red, there's too many monsters in the map, kill them!", 0xffffff, 0);
    rEngine.drawForegroundString(100, 350, "           Your fireballs can destroy enemy beams!", 0xffffff, 0);
    rEngine.drawForegroundString(100, 400, "           Stuck? The yellow orbs may help!", 0xffffff, 0);

};

void NewGameState::preMainLoopHook(Hfyda2Engine& rEngine) {
    m_iOffset = (m_iOffset+1) % rEngine.getWindowWidth();
    rEngine.redrawDisplay();
}

void NewGameState::copyBackgroundBuffer(Hfyda2Engine& rEngine) {
    rEngine.getForegroundSurface()->copyRectangleFrom(rEngine.getBackgroundSurface(), 
        0, 0, 
        rEngine.getWindowWidth(), rEngine.getWindowHeight(), 
        m_iOffset, 0);

	rEngine.getForegroundSurface()->copyRectangleFrom(rEngine.getBackgroundSurface(), 
        rEngine.getWindowWidth() - m_iOffset, 0, 
        rEngine.getWindowWidth(), rEngine.getWindowHeight(), 
        m_iOffset - rEngine.getWindowWidth(), 0);
    
}

// ===================
// NewGameStateHelpers
// ===================

void NewGameStateHelpers::switchScreenToGame(Hfyda2Engine& rEngine, State* pState) {
    NewGameState& rState = dynamic_cast<NewGameState&>(*pState);
    pState->setState(rEngine, new RunningState(&rEngine, rState.getPlayerName()));    
    rEngine.virtInitialise();
}


void NewGameStateHelpers::switchScreenToMenu(Hfyda2Engine& rEngine, State* pState) {
    pState->setState(rEngine, new StartState());    
    rEngine.virtInitialise();
}
