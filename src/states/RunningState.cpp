#include "../header.h"

// TODO: remove all unnecessary includes
#include "RunningState.h"
#include "PauseState.h"
#include "EndState.h"
#include  "EndState.h"
#include "../actors/Dino.h"
#include "../monsters/Wizard.h"
#include "../monsters/Blue.h"
#include "../particles/Meteor.h"
#include "../Hfyda2TileManager.h"
#include "../Spawner.h"
#include "../components/TextBox.h"

#include <fstream>
#include <sstream>

RunningState::RunningState(Hfyda2Engine* pEngine, std::string sPlayerName) 
    :   m_levelData {0, 0, {0, 0, 0}, 0, 0, false },
        m_playerData { sPlayerName, 3, 0, false},
        m_bLevelWin(false),
        m_bDinoDamaged(false),
        m_bTimerStarted(false),
        // m_pSpawner(Spawner::getInstance(pEngine)) {
        m_pSpawner(new Spawner(pEngine)) {


    m_upWinLevelFreezeTimer = std::unique_ptr<Timer>(new Timer(2000));
    m_oDinoSpriteSheet = pEngine->loadImage("resources/images/sprites/dino/DinoSprites.png", true).resizeTo(576 * 2, 24 * 2);
}

RunningState::RunningState(Hfyda2Engine* pEngine, LevelData sLevelData, PlayerData sPlayerData) 
    :   m_bLevelWin(false),
        m_bDinoDamaged(false),
        m_bTimerStarted(false),
        m_levelData(sLevelData),
        m_playerData(sPlayerData),
        // m_pSpawner(Spawner::getInstance(pEngine)) {
        m_pSpawner(new Spawner(pEngine)) {


    m_upWinLevelFreezeTimer = std::unique_ptr<Timer>(new Timer(2000));    
    m_oDinoSpriteSheet = pEngine->loadImage("resources/images/sprites/dino/DinoSprites.png", true).resizeTo(576 * 2, 24 * 2);

}

RunningState::~RunningState() {
}

void RunningState::setupBackground(Hfyda2Engine& rEngine) {
    
    rEngine.fillBackground(0x0099db);

    SimpleImage backgroundImage = rEngine.loadImage("resources/images/background/Background.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    backgroundImage.renderImage(rEngine.getBackgroundSurface(), 0, 0, 0,0, rEngine.getWindowWidth(), rEngine.getWindowHeight());

    SimpleImage middlegroundImage = rEngine.loadImage("resources/images/background/Middleground.png", true).resizeTo(rEngine.getWindowWidth(), rEngine.getWindowHeight());
    middlegroundImage.renderImageWithMask(rEngine.getBackgroundSurface(), 0, 0, 0, 0, rEngine.getWindowWidth(), rEngine.getWindowHeight(), 0x000000);

    // Map setup
    Hfyda2TileManager& tm = rEngine.getTileManager();    
    tm.setMapSize(tm.getTileColSize(), tm.getTileRowSize()); 
    tm.loadMapData("resources/data/maps/" + m_vsLevels[m_playerData.iCurrentLevel] + ".txt");
    tm.setTopLeftPositionOnScreen(0, 0);
    tm.drawAllTiles(&rEngine, rEngine.getBackgroundSurface());

    // reset flags
    m_bDinoDamaged = false;
    m_bLevelWin = false;
    m_bTimerStarted = false;

    rEngine.redrawDisplay();
}


void RunningState::setupObjects(Hfyda2Engine& rEngine) {
    
    loadLevelData("resources/data/levels/" + m_vsLevels[m_playerData.iCurrentLevel] + ".csv");
    m_playerData.bLastLevel = m_levelData.lastLevel;
    
    rEngine.drawableObjectsChanged();
    rEngine.destroyOldObjects(true);

    rEngine.notifyObjectsAboutMouse(false);

    rEngine.createObjectArray(2);
    rEngine.storeObjectInArray(0, new Dino(&rEngine));
    rEngine.storeObjectInArray(1, new Wizard(&rEngine, m_levelData.wizardSpeed, m_levelData.meteorCooldown, m_levelData.meteorSpeed));
         
    rEngine.setAllObjectsVisible(true);

    rEngine.notifyObjectsAboutKeys(true);

    m_pSpawner->init(m_levelData.monsterTypes);
    m_pSpawner->initialSpawn();

    return;
}

void RunningState::keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) {

    switch(iKeyCode) {
        case SDLK_ESCAPE:
            if(!m_bLevelWin) {
                rEngine.setAllObjectsVisible(false);
                this->setState(rEngine, new PauseState(rEngine, m_levelData, m_playerData));
                rEngine.virtInitialise();
            }

            break;
    }
}

void RunningState::drawStringsOnTop(Hfyda2Engine& rEngine) {

    // TODO: test for centered objects, remove later
    // rEngine.drawForegroundLine(0, rEngine.getWindowHeight()/ 2, rEngine.getWindowWidth(), (rEngine.getWindowHeight() / 2) + 2, 0xffffff );
    // rEngine.drawForegroundLine(rEngine.getWindowWidth() / 2, 0, (rEngine.getWindowWidth() / 2) + 2, (rEngine.getWindowHeight()), 0xffffff );

    for(int itr = 0; itr < m_playerData.iDinoLives; itr++) {
            m_oDinoSpriteSheet.renderImageWithMask(
                    rEngine.getForegroundSurface(), 
                    4 * 2, 4 * 2, 
                    24 + (32*itr), 24, 
                    16 * 2, 16 * 2, 
                    0x000000);
    }

    std::string sLevel = "Level ";
    sLevel += std::to_string(m_playerData.iCurrentLevel+1);
    const char* cLevel = sLevel.c_str();
    rEngine.drawForegroundString(1152, 24, cLevel , 0xffffff , rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", 36));
};

void RunningState::preMainLoopHook(Hfyda2Engine& rEngine) {

    Hfyda2TileManager& tm = rEngine.getTileManager();
    std::vector<std::vector<int>> vvMapSchema = tm.getMapSchema();
    for ( int iCol = 0 ; iCol < tm.getTileColSize() ; iCol++ ) {
        for ( int iRow = 0 ; iRow < tm.getTileRowSize() ; iRow++ ) {
            switch (vvMapSchema[iRow][iCol]) {
                case 30: 
                    if(m_pSpawner->getSpawnCount() < m_levelData.minMonsters) {
                        tm.setAndRedrawMapValueAt(iCol, iRow, 31, &rEngine, rEngine.getBackgroundSurface());
                        tm.setTileValueInSchema(iCol, iRow, 31);
                    }
                    break;
                case 31:
                    if(m_pSpawner->getSpawnCount() > m_levelData.minMonsters) {
                        tm.setAndRedrawMapValueAt(iCol, iRow, 30, &rEngine, rEngine.getBackgroundSurface());
                        tm.setTileValueInSchema(iCol, iRow, 30);    
                    }                             
                    break;
            }
        }
    }    
    
    if(!m_bLevelWin) {
        m_pSpawner->m_upSpawnCooldown->update();
        m_pSpawner->spawn();
    }


    if(m_bDinoDamaged) {
        --m_playerData.iDinoLives;

        if(m_playerData.iDinoLives == 0) {
            this->setState(rEngine, new EndState(rEngine, m_playerData.iCurrentLevel, m_playerData.iDinoLives, false, m_playerData.sPlayerName));         
        }

        rEngine.virtInitialise();
        return;
    }
    
    if(m_bLevelWin) {
        if(!m_bTimerStarted) {
            m_upWinLevelFreezeTimer->startTimer();
            m_bTimerStarted = true;

            rEngine.notifyAllObjects(2, 0, 0);

            m_playerData.bLastLevel ? rEngine.appendObjectToArray(new TextBox(rEngine, rEngine.getWindowWidth() / 2 - 150, rEngine.getWindowHeight() / 2 - 48, 320, 96 , "You Won", 96, 0x91d82b))
                : rEngine.appendObjectToArray(new TextBox(rEngine, rEngine.getWindowWidth() / 2 - 200, rEngine.getWindowHeight() / 2 - 48, 400, 96 , "Next Level", 96, 0x91d82b));
            
        }

        m_upWinLevelFreezeTimer->update();

        if(m_upWinLevelFreezeTimer->check()) {
            if(m_playerData.bLastLevel) {  
                this->setState(rEngine, new EndState(rEngine, m_playerData.iCurrentLevel, m_playerData.iDinoLives, true, m_playerData.sPlayerName));
                rEngine.virtInitialise();
                return;
            }

            m_playerData.iCurrentLevel++;
            rEngine.virtInitialise();
        }
        
        return;        
    }
}

void RunningState::loadLevelData(std::string sPath) {
    std::ifstream oLevelFile;
    oLevelFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    std::cout << "Attempting to read level data from \"" <<  sPath << "\""<< std::endl;
    
    try {
        oLevelFile.open(sPath);
        oLevelFile.ignore(1000, '\n');

        std::string line;
        // while(std::getline(oLevelFile, line))  {
        std::getline(oLevelFile, line); // get 1 line (the .csv file only has 1 relevant line)
        std::istringstream iss {line};

        std::vector<std::string> tokens;
        std::string token;

        // restricted parsing, does ignore whitespace
        while(std::getline(iss, token, ',')) {
            // std::cout << token << std::endl; 
            tokens.push_back(token); // parse the stringstream into tokens
        }

        // convert tokens into level data        
        m_levelData.monsterTypes = std::stoi(tokens[0]);
        m_levelData.meteorCooldown = std::stoi(tokens[1]);
        m_levelData.meteorSpeed[0] = std::stoi(tokens[2]);
        m_levelData.meteorSpeed[1] = std::stoi(tokens[3]);
        m_levelData.meteorSpeed[2] = std::stoi(tokens[4]);
        m_levelData.wizardSpeed = std::stoi(tokens[5]);
        m_levelData.minMonsters = std::stoi(tokens[6]);
        std::istringstream(tokens[7]) >> std::boolalpha >> m_levelData.lastLevel;
            
    } catch (const std::ifstream::failure& e){     
            perror(("Exception opening file: " + sPath).c_str());
    }

    oLevelFile.close();
}