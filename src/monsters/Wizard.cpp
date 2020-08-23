#include "../header.h"

#include "Wizard.h"
#include "../actors/Dino.h"

#include "../Hfyda2TileManager.h"
#include "../Hfyda2Engine.h"
#include "../particles/Meteor.h"
#include "../helper/Timer.h"

Wizard::Wizard(Hfyda2Engine* pEngine, int iWizardSpeed, int iSummonSpeed, std::vector<int> iMeteorSpeed)
    :   Monster(pEngine),
        m_bCompletedMovement(false),
        m_iCooldown(iSummonSpeed),
        m_iWizardSpeed(iWizardSpeed),
        m_iMeteorSpeed(iMeteorSpeed)
        {

    m_iImageScale = 7;
    m_iXVelocity = 20;
    m_oSpriteSheet = pEngine->loadImage("resources/images/sprites/enemies/wizard/wizard.png", true).resizeTo(323 * m_iImageScale, 15 * m_iImageScale);
    m_oSpriteSheetFlipped = pEngine->loadImage("resources/images/sprites/enemies/wizard/wizard_left.png", true).resizeTo(323 * m_iImageScale, 15 * m_iImageScale);
    
    m_iDrawWidth = 12 * m_iImageScale;
    m_iDrawHeight = 15 * m_iImageScale;

    m_bFreeze = false;

    m_eActionState = IDLE;
    m_eDirectionState = RIGHT;
    
    Hfyda2TileManager& tm = m_pEngine->getTileManager();

    Dino& rDino = dynamic_cast<Dino&>(*(pEngine->getDisplayableObject(0)));
    rDino.attachObserver(this);
    
    m_pTimerCooldown = std::unique_ptr<Timer>(new Timer(m_iCooldown));
    m_pTimerMeteor = std::unique_ptr<Timer>(new Timer(5000));
    m_pTimerCooldown->startTimer();
    m_pTimerMeteor->startTimer();
    
    // setPosition(tm.getMapWidth() * tm.getTileWidth() / 2, 26); // initial position    
    setPosition(tm.getMapWidth() * tm.getTileWidth() / 2, 40); // initial position    
    
}

Wizard::~Wizard() {}

void Wizard::virtDraw() {

    // TODO: improve animation for wizard

    int iTick = getEngine()->getModifiedTime() / 400; 
    int iTickWalk = getEngine()->getModifiedTime() / 600;
    int iTickAttack = getEngine()->getModifiedTime() / 200;
	int iFrameWalk = iTickWalk % 4;
    int iFrameIdle = iTick % 6;
    int iFrameAttack = iTickAttack % 6;

    // std::cout << iFrameWalk << std::endl;

    if(m_eActionState == IDLE) {
        m_iDrawWidth = 12 * m_iImageScale;
        switch(m_eDirectionState) {
            case LEFT:            
                m_oSpriteSheetFlipped.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                    m_mFrameIdleLeftLookUpTable[iFrameIdle] * m_iImageScale, 0, 
                    m_iCurrentScreenX, m_iCurrentScreenY, 
                    m_iDrawWidth, m_iDrawHeight, 
                    0x000000);
                break;

            case RIGHT:
                m_oSpriteSheet.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                    m_mFrameIdleLookUpTable[iFrameIdle] * m_iImageScale, 0, 
                    // m_mFrameWalkingLookUpTable[iFrameWalk] * m_iImageScale, 0, 
                    m_iCurrentScreenX, m_iCurrentScreenY, 
                    m_iDrawWidth, m_iDrawHeight, 
                    0x000000);
                break;
        }

    } else if(m_eActionState == WALK) {
        m_iDrawWidth = 12 * m_iImageScale;
        switch(m_eDirectionState) {
            case LEFT:
                m_oSpriteSheetFlipped.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                    m_mFrameWalkingLeftLookUpTable[iFrameWalk] * m_iImageScale, 0, 
                    m_iCurrentScreenX, m_iCurrentScreenY, 
                    m_iDrawWidth, m_iDrawHeight, 
                    0x000000);
                break;

            case RIGHT:
                m_oSpriteSheet.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                        m_mFrameWalkingLookUpTable[iFrameWalk] * m_iImageScale, 0, 
                        m_iCurrentScreenX, m_iCurrentScreenY, 
                        m_iDrawWidth, m_iDrawHeight, 
                        0x000000);
                break;
        }

    } else if(m_eActionState == ATTACK) {

        m_iDrawWidth = 16 * m_iImageScale;
        switch(m_eDirectionState) {
            case LEFT:
                m_oSpriteSheetFlipped.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                    m_mFrameAttackLeftLookUpTable[iFrameAttack] * m_iImageScale, 0, 
                    m_iCurrentScreenX, m_iCurrentScreenY, 
                    m_iDrawWidth, m_iDrawHeight, 
                    0x000000);
                break;
                
            case RIGHT:
                m_oSpriteSheet.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                        m_mFrameAttackLookUpTable[iFrameAttack] * m_iImageScale, 0, 
                        m_iCurrentScreenX, m_iCurrentScreenY, 
                        m_iDrawWidth, m_iDrawHeight, 
                        0x000000);
                break;
        }
    }
}

void Wizard::virtDoUpdate(int iCurrentTime) {

    if(m_pEngine->isPaused()) return;
    if(m_bFreeze) return;

    m_pTimerCooldown->update();
    m_pTimerMeteor->update();    
    makeMove(iCurrentTime);

    
    if(!m_oMovement.hasMovementFinished(iCurrentTime)) {     
        m_oMovement.calculate(iCurrentTime);
        m_iCurrentScreenX = m_oMovement.getX();
    }
    
    if(m_oMovement.hasMovementFinished(iCurrentTime)) {
        m_bCompletedMovement = true;
    }
    
    redrawDisplay();       
}

long Wizard::virtNotify( int iSignalNumber, int x, int y ) {
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

void Wizard::setPosition(int iX, int iY) {
    this->m_iCurrentScreenX = iX;
    this->m_iCurrentScreenY = iY;
}


void Wizard::attack() {
    m_eActionState = ATTACK;

    using namespace std;

    m_pEngine->drawableObjectsChanged();
    // cout << "non null count: " << m_pEngine->getNonNullObjectContentCount() << endl;

    // cout << "count: " << m_pEngine->getContentCount() << endl;
    
    // int iIndex = m_pEngine->getNonNullObjectContentCount();
    // m_pEngine->storeObjectInArray(iIndex, new Meteor(m_pEngine, m_iCurrentScreenX, iIndex));

    // set the next index to null to indicate last element
    // m_pEngine->storeObjectInArray(m_pEngine->getNonNullObjectContentCount(), nullptr);

    int iMeteorSpeedIndex = rand() % 2;
    
    if(m_pTimerMeteor->check()) iMeteorSpeedIndex = 2; // summon fast meteor every 5 seconds

    m_pEngine->appendObjectToArray(new Meteor(m_pEngine, m_iCurrentScreenX, m_iMeteorSpeed[iMeteorSpeedIndex]));
}

void Wizard::die() {
    std::cout << "Wizard can't die :)\n";
}

void Wizard::updateDinoPos(int iXPos) {
    m_iLastDinoPosX = iXPos;
}

void Wizard::makeMove(int iCurrentTime) {    
    int iChoice = rand() % 3;
    
    if(iChoice == 1) { // summon

        // TODO: summon meteor fro random location?
        if(m_pTimerCooldown->check()) attack();
        
    } else if (iChoice == 0) {
        if(m_bCompletedMovement) { // only setup new movement if previous movement has completed            
            m_eActionState = WALK;
            if (m_iCurrentScreenX >= m_iLastDinoPosX) 
                m_eDirectionState = LEFT;
            else 
                m_eDirectionState = RIGHT;
                
            m_oMovement.setup(m_iCurrentScreenX, m_iCurrentScreenY, m_iLastDinoPosX, m_iCurrentScreenY, iCurrentTime, iCurrentTime + m_iWizardSpeed);
            m_bCompletedMovement = false;
        }
    } else {
        return;
    }

}
