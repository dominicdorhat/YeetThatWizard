#include "../header.h"

#include "EndState.h"
#include "StartState.h"

#include "../components/Button.h"

#include <fstream>


EndState::EndState(Hfyda2Engine& rEngine, int iHighestLevel, int iLivesLeft ,bool bWin, std::string sPlayerName)
    :   m_iHighestLevel(iHighestLevel), 
        m_iLivesLeft(iLivesLeft),
        m_sPlayerName(sPlayerName),
        m_bWin(bWin),
        m_iImageScale(3) { 
    
    rEngine.notifyObjectsAboutKeys(false);

}

EndState::~EndState() {}

void EndState::setupBackground(Hfyda2Engine& rEngine) {    
    rEngine.fillBackground(0x0099db);
    SimpleImage backgroundImage = rEngine.loadImage("resources/images/background/Background.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    backgroundImage.renderImage(rEngine.getBackgroundSurface(), 0, 0, 0,0, rEngine.getWindowWidth(), rEngine.getWindowHeight());

    SimpleImage middlegroundImage = rEngine.loadImage("resources/images/background/Middleground.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    middlegroundImage.renderImageWithMask(rEngine.getBackgroundSurface(), 0, 0, 0, 0, rEngine.getWindowWidth(), rEngine.getWindowHeight(), 0x000000);    

    rEngine.redrawDisplay();
}

void EndState::setupObjects(Hfyda2Engine& rEngine) {
    rEngine.drawableObjectsChanged();
    rEngine.destroyOldObjects(true);
    rEngine.notifyObjectsAboutKeys(false);
    rEngine.notifyObjectsAboutMouse(true);

    rEngine.createObjectArray(2);
    rEngine.storeObjectInArray(0,new Button(rEngine, (rEngine.getWindowWidth() / 2) - 80, 428, 220, 36, "Back to Menu", 0xffffff, 0xDCDCDC, this, &EndStateHelpers::switchScreenToMenu, 36));
    rEngine.storeObjectInArray(1,new Button(rEngine, (rEngine.getWindowWidth() / 2) - 32, 476, 64, 36, "Quit", 0xffffff, 0xDCDCDC, this, &EndStateHelpers::quit, 36));
    
    rEngine.setAllObjectsVisible(true);
    updatePlayerData("resources/data/highscore/highscore.csv", m_sPlayerName, m_iHighestLevel + 1, m_iLivesLeft);
}

void EndState::drawStringsOnTop(Hfyda2Engine& rEngine) {

    m_oLivesSprite = rEngine.loadImage("resources/images/sprites/dino/DinoSprites.png", true).resizeTo(576 * m_iImageScale, 24 * m_iImageScale);

    // TODO: test for centered objects, remove later
    // rEngine.drawForegroundLine(0, rEngine.getWindowHeight()/ 2, rEngine.getWindowWidth(), (rEngine.getWindowHeight() / 2) + 2, 0xffffff );
    // rEngine.drawForegroundLine(rEngine.getWindowWidth() / 2, 0, (rEngine.getWindowWidth() / 2) + 2, (rEngine.getWindowHeight()), 0xffffff );
    
    rEngine.drawBackgroundRectangle(0, 200, rEngine.getWindowWidth(), 392, 0x000000);

    if(m_bWin) {
        rEngine.drawForegroundString(rEngine.getWindowWidth() / 2 - 300, 240, "You yeeted that wizard!", 0x91d82b , rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 64));
        rEngine.drawForegroundString(rEngine.getWindowWidth() / 2 - 200, 304, "Lives left ", 0x91d82b , rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 48));
            
        for(int itr = 0; itr < m_iLivesLeft; itr++) {
            m_oLivesSprite.renderImageWithMask(
                    rEngine.getForegroundSurface(), 
                    4 * m_iImageScale, 4 * m_iImageScale, 
                    rEngine.getWindowWidth() / 2 + (48*itr), 304, 
                    16 * m_iImageScale, 16 * m_iImageScale, 
                    0x000000);
        }

    } else {
        rEngine.drawForegroundString(rEngine.getWindowWidth() / 2 - 100, 240, "You died",  0xC70039  , rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 64));
        std::string sLevel = "Highest level: ";
        sLevel += std::to_string(m_iHighestLevel + 1);
        rEngine.drawForegroundString(rEngine.getWindowWidth() / 2 - 130, 304, sLevel.c_str() , 0xC70039 , rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 48));

    }

  

}

void EndState::keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) {
   
}

void EndState::preMainLoopHook(Hfyda2Engine& rEngine) {
    m_iOffset = (m_iOffset+1) % rEngine.getWindowWidth();
    rEngine.redrawDisplay();
}

// void EndState::copyBackgroundBuffer(Hfyda2Engine& rEngine) {
//     rEngine.getForegroundSurface()->copyRectangleFrom(rEngine.getBackgroundSurface(), 
//         0, 0, 
//         rEngine.getWindowWidth(), rEngine.getWindowHeight(), 
//         m_iOffset, 0);

// 	rEngine.getForegroundSurface()->copyRectangleFrom(rEngine.getBackgroundSurface(), 
//         rEngine.getWindowWidth() - m_iOffset, 0, 
//         rEngine.getWindowWidth(), rEngine.getWindowHeight(), 
//         m_iOffset - rEngine.getWindowWidth(), 0);
    
// }

void EndState::updatePlayerData(std::string sPath, std::string sPlayerName, int iHigestLevel, int iLivesLeft) {
    std::ofstream oHighScore;
    oHighScore.exceptions(std::ofstream::badbit | std::ofstream::failbit);

    std::cout << "Attempting to read level data from \"" <<  sPath << "\""<< std::endl;
    
    try {
        oHighScore.open(sPath, std::ios::app);
        oHighScore << "\n" << sPlayerName << "," << iHigestLevel << "," << iLivesLeft;
            
    } catch (const std::ofstream::failure& e){     
            perror(("Exception opening file: " + sPath).c_str());
    }

    oHighScore.close();
}

void EndStateHelpers::switchScreenToMenu(Hfyda2Engine& rEngine, State* pState) {
    pState->setState(rEngine, new StartState());
    rEngine.virtInitialise();
}

void EndStateHelpers::quit(Hfyda2Engine& rEngine, State* pState) {
    rEngine.setExitWithCode(0);
    // TODO: make sure everything is deleted
}
    