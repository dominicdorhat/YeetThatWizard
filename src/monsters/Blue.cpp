#include "../header.h"

#include "Blue.h"

#include "../states/RunningState.h"
#include "../Hfyda2TileManager.h"
#include "../Hfyda2Engine.h"
#include "../particles/BlueBeam.h"
#include "../CollisionDetection.h"
#include "../Spawner.h"

#include "../helper/Helper.h"
#include "../helper/Timer.h"

Blue::Blue(Hfyda2Engine* pEngine, int iXPos, int iYPos)
    :   Monster(pEngine),
        m_bCompletedMovement(false)        
        {

    m_iImageScale = 3;
    m_iXVelocity = 20;

    m_oSpriteSheet = pEngine->loadImage("resources/images/sprites/enemies/blue/Blue_Idle.png", true).resizeTo(128 * m_iImageScale, 54 * m_iImageScale);
    m_oSpriteSheetRun = pEngine->loadImage("resources/images/sprites/enemies/blue/Blue_Run.png", true).resizeTo(192 * m_iImageScale, 54 * m_iImageScale);
    m_oSpriteSheetAttack =  pEngine->loadImage("resources/images/sprites/enemies/blue/Blue_Attack.png", true).resizeTo(96 * m_iImageScale, 54 * m_iImageScale);
    m_oSpriteSheetDamaged =  pEngine->loadImage("resources/images/sprites/enemies/blue/Blue_Damaged.png", true).resizeTo(64 * m_iImageScale, 54 * m_iImageScale);
    
    m_iDrawWidth = 11 * m_iImageScale;
    m_iDrawHeight = 16 * m_iImageScale;

    setPosition(iXPos, iYPos);

    m_bFreeze = false;
    
    rand() % 2 ? m_eDirectionState = LEFT : m_eDirectionState = RIGHT;
    m_eActionState = IDLE;
    
    m_upMoveCooldown = std::unique_ptr<Timer>(new Timer(2200));
    m_upDeathFreezeTimer = std::unique_ptr<Timer>(new Timer(900));
}

Blue::~Blue() {}

void Blue::virtDraw() {

    // TODO: improve animation for wizard

    int iTick = getEngine()->getModifiedTime() / 400; 
    int iTickWalk = getEngine()->getModifiedTime() / 200;

	int iFrameWalk = iTickWalk % 6;
    int iFrameIdle = iTick % 4;
    int iFrameAttack = iTickWalk % 3;
    int iFrameDamaged = iTickWalk % 2;

    // std::cout << iFrameWalk << std::endl;
    // m_pEngine->drawForegroundRectangle(getDrawingRegionLeft(), getDrawingRegionTop(), getDrawingRegionRight(), getDrawingRegionBottom(), 0x00000);

    if(m_eActionState == IDLE) {
        m_oSpriteSheet.renderImageWithMask(
            m_pEngine->getForegroundSurface(), 
            m_mFrameIdleLookUpTable[iFrameIdle] * m_iImageScale, (m_eDirectionState ? 8 : 35) * m_iImageScale, 
            m_iCurrentScreenX, m_iCurrentScreenY, 
            m_iDrawWidth, m_iDrawHeight, 
            0x000000);

    } else if(m_eActionState == WALK) {
        m_oSpriteSheetRun.renderImageWithMask(
            m_pEngine->getForegroundSurface(), 
            m_mFrameWalkingLookUpTable[iFrameWalk] * m_iImageScale, (m_eDirectionState ? 5 : 31) * m_iImageScale, 
            m_iCurrentScreenX, m_iCurrentScreenY, 
            m_iDrawWidth, m_iDrawHeight, 
            0x000000);
    

    } else if(m_eActionState == DAMAGED) {

        m_oSpriteSheetDamaged.renderImageWithMask(
            m_pEngine->getForegroundSurface(), 
            m_mFrameDamagedLookUpTable[iFrameDamaged] * m_iImageScale, (m_eDirectionState ? 7 : 34) * m_iImageScale, 
            m_iCurrentScreenX, m_iCurrentScreenY, 
            m_iDrawWidth, m_iDrawHeight, 
            0x000000);

    } else if(m_eActionState == ATTACK) {

        m_oSpriteSheetAttack.renderImageWithMask(
            m_pEngine->getForegroundSurface(), 
            m_mFrameAttackLookUpTable[iFrameAttack] * m_iImageScale, (m_eDirectionState ? 7 : 34) * m_iImageScale, 
            m_iCurrentScreenX, m_iCurrentScreenY, 
            m_iDrawWidth, m_iDrawHeight, 
            0x000000);
    }
}

void Blue::virtDoUpdate(int iCurrentTime) {

    if(m_pEngine->isPaused()) return;
    if(m_bFreeze) return;

    m_upMoveCooldown->update();

    if(m_eActionState == DAMAGED) {
        m_upDeathFreezeTimer->update();
        
        redrawRectangle();

        if(m_upDeathFreezeTimer->check())  {
            // Spawner& rSpawner = Spawner::getInstance(m_pEngine);
            Spawner& rSpawner = *(dynamic_cast<RunningState*>(m_pEngine->getState())->getSpawner());
            rSpawner.updateSpawnCount();

            m_pEngine->drawableObjectsChanged();
            m_pEngine->removeDisplayableObject(this);
            delete this;       
        }
        return;
    } else {
        if(m_upMoveCooldown->check()) {        
            makeMove(iCurrentTime);                    
        } 

        if(!m_oMovement.hasMovementFinished(iCurrentTime)) {     
            if(!(m_eActionState == ATTACK)) {
                m_oMovement.calculate(iCurrentTime); 
                m_iCurrentScreenX = m_oMovement.getX();
            }
        }
        
        if(m_oMovement.hasMovementFinished(iCurrentTime)) {
            m_bCompletedMovement = true;
            m_eActionState = IDLE;
        }
    }
    
    redrawDisplay();       
}

long Blue::virtNotify( int iSignalNumber, int x, int y ) {
    switch (iSignalNumber) {
        case 1:
        case 2:
            m_bFreeze = true;
            break;
        default:
            break;
    }
    
    return 1;
}


void Blue::attack() {
  
    m_pEngine->drawableObjectsChanged();
    m_pEngine->appendObjectToArray(new BlueBeam(m_pEngine, m_iCurrentScreenX, m_iCurrentScreenY, m_eDirectionState));
}

void Blue::die() {
    m_eActionState = DAMAGED;
    m_upDeathFreezeTimer->startTimer();
}

void Blue::updateDinoPos(int iXPos) {
    m_iLastDinoPosX = iXPos;
}

void Blue::makeMove(int iCurrentTime) {    

    int iChoice = rand() % 2;
    
    if(iChoice) { // attack
        m_eActionState = ATTACK;
        attack();

        
    } else {

        if(m_bCompletedMovement) { 
            m_eActionState = WALK;
            int iLocation = rand() % (m_pEngine->getWindowWidth()) + 1;

            if (m_iCurrentScreenX >= iLocation) 
                m_eDirectionState = LEFT;
            else 
                m_eDirectionState = RIGHT;
                
            m_oMovement.setup(m_iCurrentScreenX, m_iCurrentScreenY, iLocation, m_iCurrentScreenY, iCurrentTime, iCurrentTime + 5000);
            m_bCompletedMovement = false;
        }
    } 
    // else {
    //     if(m_bCompletedMovement)
    //         m_eActionState = IDLE;
    //     return;
    // }

}


void Blue::setPosition(int iX, int iY) {
    this->m_iCurrentScreenX = iX;
    this->m_iCurrentScreenY = iY;
}
