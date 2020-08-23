#include "../header.h"

#include "HighScoreState.h"
#include "RunningState.h"
#include "StartState.h"

#include "../components/Button.h"

#include <fstream>
#include <sstream>
#include <algorithm>

HighScoreState::HighScoreState(Hfyda2Engine& rEngine)
    :   m_iOffset(0),
        m_oFlip(),
        m_vScoreList() {}        

HighScoreState::~HighScoreState() {}

void HighScoreState::setupBackground(Hfyda2Engine& rEngine) {

    SimpleImage backgroundImage = rEngine.loadImage("resources/images/background/Background.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    backgroundImage.renderImage(rEngine.getBackgroundSurface(), 0, 0, 0,0, rEngine.getWindowWidth(), rEngine.getWindowHeight());

    SimpleImage middlegroundImage = rEngine.loadImage("resources/images/background/Middleground.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    middlegroundImage.renderImageWithMask(rEngine.getBackgroundSurface(), 0, 0, 0, 0, rEngine.getWindowWidth(), rEngine.getWindowHeight(), 0x000000);

    SimpleImage cloudImage = rEngine.loadImage("resources/images/background/cloud.png", true);
    cloudImage.renderImageApplyingMapping(&rEngine, rEngine.getBackgroundSurface(), 100, 100, cloudImage.getWidth(), cloudImage.getHeight(), &m_oFlip);

    cloudImage.renderImage(rEngine.getBackgroundSurface(), 0, 0, 600, 50, cloudImage.getWidth(), cloudImage.getHeight());

    // =============
    // SCORE SORTING
    // =============
    loadHighScore("resources/data/highscore/highscore.csv");
    std::sort(m_vScoreList.begin(), m_vScoreList.end(), &HighScoreStateHelpers::sort);

    rEngine.redrawDisplay();
}

void HighScoreState::setupObjects(Hfyda2Engine& rEngine) {
    rEngine.notifyObjectsAboutMouse(true);
    rEngine.drawableObjectsChanged();    
    rEngine.destroyOldObjects(true);
    rEngine.createObjectArray(1);
    rEngine.storeObjectInArray(0,
        new Button(rEngine, 100, rEngine.getWindowHeight() - 100, 200, 25, "Back", 0xffffff, 0xDCDCDC, this, &HighScoreStateHelpers::switchScreenToMenu, 36));
}

void HighScoreState::drawStringsOnTop(Hfyda2Engine& rEngine) {
    // rEngine.drawForegroundLine(0, rEngine.getWindowHeight()/ 2, rEngine.getWindowWidth(), (rEngine.getWindowHeight() / 2) + 2, 0xffffff );
    // rEngine.drawForegroundLine(rEngine.getWindowWidth() / 2, 0, (rEngine.getWindowWidth() / 2) + 2, (rEngine.getWindowHeight()), 0xffffff );
    Font* m_pFont64 = rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 64);
    Font* m_pFont48 = rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 48);
    Font* m_pFont36 = rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 36);

    rEngine.drawForegroundString(rEngine.getWindowWidth() / 2 - 148, 100, "LEADERBOARD", 0x3A5611, m_pFont64);

    rEngine.drawForegroundString(224, 200, "NAME", 0xC5DBD6, m_pFont48);
    rEngine.drawForegroundString(424, 200, "HIGHEST LEVEL", 0xC5DBD6, m_pFont48);
    rEngine.drawForegroundString(824, 200, "LIVES LEFT", 0xC5DBD6, m_pFont48);

    
    for(int itr = 1; (itr < m_vScoreList.size()) && (itr <= 5); itr++ ) {
        std::string number = std::to_string(itr) + ")";        
        rEngine.drawForegroundString(124, 200 + (80 * itr), number.c_str(), 0xffffff, m_pFont48);

        rEngine.drawForegroundString(224, 200 + (80 * itr), (m_vScoreList[itr].name).c_str(), 0xffffff, m_pFont36);
        rEngine.drawForegroundString(550, 200 + (80 * itr), std::to_string(m_vScoreList[itr].highestLevel).c_str(), 0xffffff, m_pFont36);
        rEngine.drawForegroundString(924, 200 + (80 * itr), std::to_string(m_vScoreList[itr].livesLeft).c_str(), 0xffffff, m_pFont36);


    }

};


void HighScoreState::keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) {
}


void HighScoreState::preMainLoopHook(Hfyda2Engine& rEngine) {
    m_iOffset = (m_iOffset+1) % rEngine.getWindowWidth();
    rEngine.redrawDisplay();
}

void HighScoreState::copyBackgroundBuffer(Hfyda2Engine& rEngine) {
    rEngine.getForegroundSurface()->copyRectangleFrom(rEngine.getBackgroundSurface(), 
        0, 0, 
        rEngine.getWindowWidth(), rEngine.getWindowHeight(), 
        m_iOffset, 0);

	rEngine.getForegroundSurface()->copyRectangleFrom(rEngine.getBackgroundSurface(), 
        rEngine.getWindowWidth() - m_iOffset, 0, 
        rEngine.getWindowWidth(), rEngine.getWindowHeight(), 
        m_iOffset - rEngine.getWindowWidth(), 0);
    
}


void HighScoreState::loadHighScore(std::string sPath) {
    std::ifstream oHighScoreFile;
    oHighScoreFile.exceptions(std::ifstream::badbit);

    std::cout << "Attempting to read level data from \"" <<  sPath << "\""<< std::endl;
    
    try {
        oHighScoreFile.open(sPath);
        oHighScoreFile.ignore(1000, '\n'); // ignore title line

        std::string line;
        while(std::getline(oHighScoreFile, line))  {
            std::istringstream iss {line};

            std::vector<std::string> tokens;
            std::string token;

            // restricted parsing, does ignore whitespace
            while(std::getline(iss, token, ',')) {
                tokens.push_back(token); // parse the stringstream into tokens
            }

            // convert tokens into score data
            Score sScore {"", 0, 0};
            std::istringstream(tokens[0]) >> sScore.name;
            sScore.highestLevel = std::stoi(tokens[1]);
            sScore.livesLeft = std::stoi(tokens[2]);

            m_vScoreList.push_back(sScore);
        }
            
    } catch (const std::ifstream::failure& e){     
            perror(("Exception opening file: " + sPath).c_str());
    }

    oHighScoreFile.close();

}

// ======================
// HIGHSCORESTATE HELPERS
// ======================

void HighScoreStateHelpers::switchScreenToMenu(Hfyda2Engine& rEngine, State* pState) {
    pState->setState(rEngine, new StartState());
    rEngine.virtInitialise();
}

// implmentation of Java's Comparator (sorts according to highest level first, then lives left)
bool HighScoreStateHelpers::sort(HighScoreState::Score const& lhs, HighScoreState::Score const& rhs) {
    // if(lhs.highestLevel != rhs.highestLevel)
    //     return lhs.highestLevel > rhs.highestLevel;
    // if(lhs.livesLeft != rhs.livesLeft) 
    //     return lhs.livesLeft > rhs.livesLeft;    
    return std::tie(lhs.highestLevel, lhs.livesLeft) > std::tie(rhs.highestLevel, rhs.livesLeft);
}
