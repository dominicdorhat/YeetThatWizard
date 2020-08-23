#include "../header.h"

#include "StartState.h"
#include "NewGameState.h"
#include "HighScoreState.h"
#include "../components/Button.h"

StartState::StartState(void) 
    :   m_iOffset(0),
        m_oFlip() {}

StartState::~StartState(void) { printf("destroyed\n");}

void StartState::setupBackground(Hfyda2Engine& rEngine) {
    // TODO: temporary
    for(int xPos = 100; xPos < rEngine.getWindowWidth(); xPos += 200) {
        rEngine.drawBackgroundRectangle(xPos, 100, xPos+50, 700, 0xffffff);  
    }

    SimpleImage backgroundImage = rEngine.loadImage("resources/images/background/Background.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    backgroundImage.renderImage(rEngine.getBackgroundSurface(), 0, 0, 0,0, rEngine.getWindowWidth(), rEngine.getWindowHeight());

    SimpleImage middlegroundImage = rEngine.loadImage("resources/images/background/Middleground.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    middlegroundImage.renderImageWithMask(rEngine.getBackgroundSurface(), 0, 0, 0, 0, rEngine.getWindowWidth(), rEngine.getWindowHeight(), 0x000000);

    SimpleImage cloudImage = rEngine.loadImage("resources/images/background/cloud.png", true);
    cloudImage.renderImageApplyingMapping(&rEngine, rEngine.getBackgroundSurface(), 100, 100, cloudImage.getWidth(), cloudImage.getHeight(), &m_oFlip);

    cloudImage.renderImage(rEngine.getBackgroundSurface(), 0, 0, 600, 50, cloudImage.getWidth(), cloudImage.getHeight());

       
    rEngine.redrawDisplay();
}

void StartState::setupObjects(Hfyda2Engine& rEngine) {
    rEngine.notifyObjectsAboutMouse(true);
    rEngine.drawableObjectsChanged();
    rEngine.destroyOldObjects(true);

    rEngine.createObjectArray(3);
    rEngine.storeObjectInArray(0, 
        new Button(rEngine, (rEngine.getWindowWidth() / 2) - 64, 476, 140, 36, "New Game", 0xffffff, 0xDCDCDC, this, &StartStateHelpers::switchScreenToNewGame, 36));
    rEngine.storeObjectInArray(1, 
        new Button(rEngine, (rEngine.getWindowWidth() / 2) - 64, 524, 150, 36, "High Score", 0xffffff, 0xDCDCDC, this, &StartStateHelpers::switchScreenToLoadGame, 36));
    rEngine.storeObjectInArray(2, 
        new Button(rEngine, (rEngine.getWindowWidth() / 2) - 64, 572, 136, 36, "Exit Game", 0xffffff, 0xDCDCDC,this, &StartStateHelpers::exitGame, 36));
}

void StartState::keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) { 
    switch(iKeyCode) {
        case SDLK_ESCAPE:
            printf("Exiting game...\n");
            rEngine.setExitWithCode(0);
            break; 
    }
}

void StartState::drawStringsOnTop(Hfyda2Engine& rEngine) {

    // rEngine.drawForegroundLine(0, rEngine.getWindowHeight()/ 2, rEngine.getWindowWidth(), (rEngine.getWindowHeight() / 2) + 2, 0xffffff );
    // rEngine.drawForegroundLine(rEngine.getWindowWidth() / 2, 0, (rEngine.getWindowWidth() / 2) + 2, (rEngine.getWindowHeight()), 0xffffff );

    rEngine.drawForegroundString((rEngine.getWindowWidth() / 2) - 48, 300, "yeet  that", 0xFFFFFF, rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 48));
    rEngine.drawForegroundString((rEngine.getWindowWidth() / 2) - 166, 300, "W", 0xFFFFFF, rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 128));
    rEngine.drawForegroundString((rEngine.getWindowWidth() / 2) - 64, 330, "izard", 0xFFFFFF, rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 112));            
       
}

void StartState::preMainLoopHook(Hfyda2Engine& rEngine) {
    m_iOffset = (m_iOffset+1) % rEngine.getWindowWidth();
    rEngine.redrawDisplay();
}

void StartState::copyBackgroundBuffer(Hfyda2Engine& rEngine) {
    rEngine.getForegroundSurface()->copyRectangleFrom(rEngine.getBackgroundSurface(), 
        0, 0, 
        rEngine.getWindowWidth(), rEngine.getWindowHeight(), 
        m_iOffset, 0);

	rEngine.getForegroundSurface()->copyRectangleFrom(rEngine.getBackgroundSurface(), 
        rEngine.getWindowWidth() - m_iOffset, 0, 
        rEngine.getWindowWidth(), rEngine.getWindowHeight(), 
        m_iOffset - rEngine.getWindowWidth(), 0);
    
}


void StartStateHelpers::switchScreenToNewGame(Hfyda2Engine& rEngine, State* pState) {
    pState->setState(rEngine, new NewGameState());
    rEngine.virtInitialise();
}

void StartStateHelpers::switchScreenToLoadGame(Hfyda2Engine& rEngine, State* pState) {
    pState->setState(rEngine, new HighScoreState(rEngine));
    rEngine.virtInitialise();
}

void StartStateHelpers::exitGame(Hfyda2Engine& rEngine, State* pState) {
    // TODO: cleanup objects
    rEngine.setExitWithCode(0);
}

// void StartState::switchScreenToNewGame(Hfyda2Engine& rEngine) {
//     this->setCurrentScreen(NEW_GAME);
//     this->setupBackground(rEngine);
//     this->setupObjects(rEngine);
// }