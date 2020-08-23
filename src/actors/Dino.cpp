#include "../header.h"

#include "Dino.h"
#include "../InputHandler.h"
#include "../commands/Command.h"
#include "../CollisionDetection.h"
#include "../states/RunningState.h"

#include "../monsters/Monster.h"
#include "../particles/Meteor.h"
#include "../particles/Fireball.h"
#include "../particles/BlueBeam.h"

#include "../helper/Helper.h"
#include "../helper/Timer.h"

Dino::Dino(Hfyda2Engine* pEngine)
    :   GameActor(pEngine),
        m_eActionState(IDLE),
        m_eDirectionState(RIGHT)
        {

    m_pInputHandler = std::unique_ptr<InputHandler>(new InputHandler());
    m_pInputHandler->initCommands();

    m_iImageScale = 3;
    m_iXVelocity = m_iYVelocity = 2;
    m_oSpriteSheet = pEngine->loadImage("resources/images/sprites/dino/DinoSprites.png", true).resizeTo(576 * m_iImageScale, 24 * m_iImageScale);
    m_oSpriteSheetFlipped = pEngine->loadImage("resources/images/sprites/dino/DinoSprites-left.png", true).resizeTo(576 * m_iImageScale, 24 * m_iImageScale);
    
    this->m_iDrawWidth = 16 * m_iImageScale;
    this->m_iDrawHeight = 16 * m_iImageScale;

    m_pDeathFreezeTimer = std::unique_ptr<Timer>(new Timer(2000));
    m_pAttackCooldown = std::unique_ptr<Timer>(new Timer(1000));

    m_bFreeze = false;
    
    setPosition(0, 672); // initial position
}

Dino::~Dino() {
    m_vMonsterObservers.clear();
}

void Dino::virtDraw() {

    // BEWARE, VERY MENIAL CODE AHEAD 
    // ===========
    // ANIMATION
    // ===========

    int iTick = getEngine()->getModifiedTime() / 300; 
    int iTickWalk = getEngine()->getModifiedTime() / 150;
    int iTickAttack = getEngine()->getModifiedTime() / 50;
	int iFrameWalk = iTickWalk % 6;
    int iFrameIdle = iTick % 4;
    int iFrameDamage = iTickWalk % 3;
    int iFrameAttack = iTickAttack % 2;

    if(m_eActionState == IDLE) {
        switch(m_eDirectionState) {
            case LEFT:     
                m_oSpriteSheetFlipped.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                    m_mFrameIdleLeftLookUpTable[iFrameIdle] * m_iImageScale, 4 * m_iImageScale, 
                    m_iCurrentScreenX, m_iCurrentScreenY, 
                    16 * m_iImageScale, 16 * m_iImageScale, 
                    0x000000);
                break;

            case RIGHT:
                m_oSpriteSheet.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                    m_mFrameIdleLookUpTable[iFrameIdle] * m_iImageScale, 4 * m_iImageScale, 
                    m_iCurrentScreenX, m_iCurrentScreenY, 
                    16 * m_iImageScale, 16 * m_iImageScale, 
                    0x000000);
                break;
        }

    } else if(m_eActionState == WALK || m_eActionState == CLIMB) {
        switch(m_eDirectionState) {
            case LEFT:
                m_oSpriteSheetFlipped.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                    m_mFrameWalkingLeftLookUpTable[iFrameWalk] * m_iImageScale, 4 * m_iImageScale, 
                    m_iCurrentScreenX, m_iCurrentScreenY, 
                    16 * m_iImageScale, 16 * m_iImageScale, 
                    0x000000);
                break;
            case RIGHT:
                m_oSpriteSheet.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                        m_mFrameWalkingLookUpTable[iFrameWalk] * m_iImageScale, 4 * m_iImageScale, 
                        m_iCurrentScreenX, m_iCurrentScreenY, 
                        16 * m_iImageScale, 16 * m_iImageScale, 
                        0x000000);
                break;
        }
    } else if(m_eActionState == DAMAGED) {
        switch(m_eDirectionState) {
            case LEFT:
                m_oSpriteSheetFlipped.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                        m_mFrameDamageLeftLookUpTable[iFrameDamage] * m_iImageScale, 4 * m_iImageScale, 
                        m_iCurrentScreenX, m_iCurrentScreenY, 
                        16 * m_iImageScale, 16 * m_iImageScale, 
                        0x000000);
                break;
            case RIGHT:
                m_oSpriteSheet.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                        m_mFrameDamageLookUpTable[iFrameDamage] * m_iImageScale, 4 * m_iImageScale, 
                        m_iCurrentScreenX, m_iCurrentScreenY, 
                        16 * m_iImageScale, 16 * m_iImageScale, 
                        0x000000);
                break;
        }
    } else if(m_eActionState == ATTACK) {
        switch(m_eDirectionState) {
            case LEFT:
                m_oSpriteSheetFlipped.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                        m_mFrameAttackLeftLookUpTable[iFrameAttack] * m_iImageScale, 4 * m_iImageScale, 
                        m_iCurrentScreenX, m_iCurrentScreenY, 
                        16 * m_iImageScale, 16 * m_iImageScale, 
                        0x000000);
                break;
            case RIGHT:
                m_oSpriteSheet.renderImageWithMask(
                    m_pEngine->getForegroundSurface(), 
                        m_mFrameAttackLookUpTable[iFrameAttack] * m_iImageScale, 4 * m_iImageScale, 
                        m_iCurrentScreenX, m_iCurrentScreenY, 
                        16 * m_iImageScale, 16 * m_iImageScale, 
                        0x000000);
                break;
        }
    }
}

void Dino::virtDoUpdate(int iCurrentTime) {

    if(m_pEngine->isPaused()) return;
    if(m_bFreeze) return;
    
    m_pAttackCooldown->update();
        
    // ========================
    // DEATH ANIMATION + TIMER
    // ========================
    if(m_eActionState == DAMAGED) {
        m_pDeathFreezeTimer->update();
        
        // only redraw dino and everything else freezes (like Mario's death animation in Donkey Kong Arcade)
        redrawRectangle();    

        if(m_pDeathFreezeTimer->check())  {
            static_cast<RunningState*>(m_pEngine->getState())->respawnAll(); // flag to reset level            
        }
        return;
    } else {
    
        notifyLastXPosition(); // notifies monsters of its X location every tick

    // =====================
    // COLLISION DETECTION
    // =====================
        DisplayableObject* pObject;
        for(int itr = 0; (pObject = m_pEngine->getDisplayableObject(itr)) != NULL; itr++) {

            if(pObject == this) continue;
            if(pObject == nullptr) continue;
            
            // meteor collison and monster collision
            if(Helper::instanceof<Meteor>(pObject) || 
                Helper::instanceof<Monster>(pObject) ||
                Helper::instanceof<BlueBeam>(pObject) ) {
                if(CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionTop(), getDrawingRegionBottom(),
                    pObject->getDrawingRegionLeft() + 5, pObject->getDrawingRegionRight() - 5, pObject->getDrawingRegionTop(), pObject->getDrawingRegionBottom() - 10)) { 
                    
                    if(Helper::instanceof<Monster>(pObject)) {
                        if((dynamic_cast<Monster*>(pObject))->getCurrentState() == Monster::DAMAGED) {
                            break;
                        }
                    }

                    m_eActionState = DAMAGED;
                    m_pDeathFreezeTimer->startTimer();
                    m_pEngine->notifyAllObjects(1, 0, 0);

                    if(Helper::instanceof<BlueBeam>(pObject)) {
                        dynamic_cast<BlueBeam*>(pObject)->die();
                    }

                    return;
                }              
            }
        }

        // ================
        // TILE INTERACTION
        // ================
        
        Hfyda2TileManager& rTM = m_pEngine->getTileManager();
        int iMapX = rTM.getMapXForScreenX(getDrawingRegionLeft() + (m_iDrawWidth / 2));
        int iMapY = rTM.getMapYForScreenY(getDrawingRegionTop());
        int iMapValue = rTM.getMapValue(iMapX, iMapY);
        // printf("%d\n",iMapValue);
        switch(iMapValue) {
            case 31:
                static_cast<RunningState*>(m_pEngine->getState())->winLevel();
                break;
                
            case 24:
                rTM.setAndRedrawMapValueAt(iMapX, iMapY, 28, m_pEngine, m_pEngine->getBackgroundSurface());
                rTM.setTileValueInSchema(iMapX, iMapY, 0);

                std::vector<std::vector<int>> vvMapSchema = rTM.getMapSchema();      
                for ( int iCol = 0 ; iCol < rTM.getTileColSize() ; iCol++ ) {
                    for ( int iRow = 0 ; iRow < rTM.getTileRowSize() ; iRow++ ) {
                        switch (vvMapSchema[iRow][iCol]) {
                            case 25: // middle
                                rTM.setAndRedrawMapValueAt(iCol, iRow, 20, m_pEngine, m_pEngine->getBackgroundSurface());
                                rTM.setTileValueInSchema(iCol, iRow, 20);
                                break;
                            case 26: // top
                                rTM.setAndRedrawMapValueAt(iCol, iRow, 21, m_pEngine, m_pEngine->getBackgroundSurface());
                                rTM.setTileValueInSchema(iCol, iRow, 21);                                
                                break;
                            case 27: // bottom
                                rTM.setAndRedrawMapValueAt(iCol, iRow, 22, m_pEngine, m_pEngine->getBackgroundSurface());
                                rTM.setTileValueInSchema(iCol, iRow, 22);
                                break;
                        }
                    }
                }
                break;

        }



        // ==========
        //  MOVEMENT
        // ==========
        if(isFeetTouchingPlatform()) {

            // check if dino is ontop of a ladder to allow climbing
            if(m_pEngine->isKeyPressed(SDLK_DOWN)) {
                if(isFeetTouchingTopLadder()) {
                    this->m_eActionState = CLIMB;        
                    this->m_pCommand = m_pInputHandler->handleInput(SDLK_DOWN);                    
                    m_pCommand->execute(*this);
                } else return;  
            }              
            
                
            if(m_pEngine->isKeyPressed(SDLK_LEFT)) {            
                if(isGoingToTouchEmptyPlatform(LEFT)) {
                    return;                
                } else {
                    if(getDrawingRegionLeft() < 0) return;
                    this->m_eDirectionState = LEFT;  
                    this->m_eActionState = WALK;
                    m_pCommand = m_pInputHandler->handleInput(SDLK_LEFT);
                    m_pCommand->execute(*this);
                } 

            }

            if(m_pEngine->isKeyPressed(SDLK_RIGHT)) {
                if(isGoingToTouchEmptyPlatform(RIGHT)){
                    return; 
                } else {
                    if(getDrawingRegionRight() > m_pEngine->getWindowWidth()) return;
                    this->m_eDirectionState = RIGHT;
                    this->m_eActionState = WALK;        
                    this->m_pCommand = m_pInputHandler->handleInput(SDLK_RIGHT);
                    m_pCommand->execute(*this);
                }
            }       
        }

        // =========
        // CLIMBING
        // =========
        if(isTouchingLadder()) {    
            if(m_pEngine->isKeyPressed(SDLK_UP)) {
                this->m_eActionState = CLIMB;        
                m_pCommand = m_pInputHandler->handleInput(SDLK_UP);
                m_pCommand->execute(*this);
            }

            if(m_pEngine->isKeyPressed(SDLK_DOWN)) {
                this->m_eActionState = CLIMB;        
                this->m_pCommand = m_pInputHandler->handleInput(SDLK_DOWN);
                m_pCommand->execute(*this);
            }
        } 

        // =========
        // SHOOTING
        // =========
        if(m_pEngine->isKeyPressed(SDLK_SPACE)) {
            if(m_eActionState == DAMAGED) return;
            
            // TODO: check for mana
            if(m_pAttackCooldown->check()) {  // 1 second attack cooldown
                if(isTouchingLadder()) return; // no attacking at ladder (even if dino is not climbing)
                this->m_eActionState = ATTACK;
                this->m_pCommand = m_pInputHandler->handleInput(SDLK_SPACE);
                m_pCommand->execute(*this);                
            } else {
                this->m_eActionState = IDLE;
            }
        }
        // cout << " isFeetTouchingTopLadder(): " << isFeetTouchingTopLadder() << endl;    
        using namespace std;
        // cout << "Dino: left: " << getDrawingRegionLeft() << " right: " << getDrawingRegionRight() << " top: " << getDrawingRegionTop() << " bottom: " << getDrawingRegionBottom() << endl;        
    }

    
    
    redrawDisplay();
    
}

void Dino::virtKeyUp(int iKeyCode) {
    if(m_eActionState != DAMAGED)
        this->m_eActionState = IDLE;
}

long Dino::virtNotify( int iSignalNumber, int x, int y ) {
    switch(iSignalNumber) {
        case 2:
            m_bFreeze = true;
            break;
        default:
            break;
    }
    return 1;
}


void Dino::setPosition(int iX, int iY) {
    this->m_iCurrentScreenX = iX;
    this->m_iCurrentScreenY = iY;
}

void Dino::moveLeft() {
    this->m_iCurrentScreenX -= m_iXVelocity;
}

void Dino::moveRight() {
    this->m_iCurrentScreenX += m_iXVelocity;
}

void Dino::climbUp() { 
    this->m_iCurrentScreenY -= m_iYVelocity;
}

void Dino::climbDown() {
    this->m_iCurrentScreenY += m_iYVelocity;
}

void Dino::attack() {
    // TODO: shoot fireball
    m_pEngine->drawableObjectsChanged();
    m_pEngine->appendObjectToArray(new Fireball(m_pEngine, m_iCurrentScreenX, m_iCurrentScreenY, m_eDirectionState));
}

bool Dino::isFeetTouchingPlatform() const {
    Hfyda2TileManager& rTM = this->m_pEngine->getTileManager();
    std::vector<std::vector<int>> vvMapSchema = rTM.getMapSchema();
        
    // using namepsace std;
    for ( int iCol = 0 ; iCol < rTM.getTileColSize() ; iCol++ ) {
        for ( int iRow = 0 ; iRow < rTM.getTileRowSize() ; iRow++ ) {
            switch (vvMapSchema[iRow][iCol]) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 10:
                case 11:
                case 12:
                    if(CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionBottom(), getDrawingRegionBottom(),
                        rTM.getLeftDrawingRegionForMapX(iCol), rTM.getRightDrawingRegionForMapX(iCol), rTM.getTopDrawingRegionForMapY(iRow), rTM.getTopDrawingRegionForMapY(iRow))) {     
                        
                        // cout << "feet touching platform: " << tileMapSchema[iRow][iCol] << " iRow: " << iRow << " iCol:" << iCol << endl;                        
                        return true;
                    }
                case 21:
                    if(CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionBottom(), getDrawingRegionBottom(),
                        rTM.getLeftDrawingRegionForMapX(iCol), rTM.getRightDrawingRegionForMapX(iCol), rTM.getTopDrawingRegionForMapY(iRow), rTM.getTopDrawingRegionForMapY(iRow))) {     
                        // printf("feet touching 21 platform\n");
                        return true; 
                }
            }
        }
    }

    // printf("not touching platform\n");
    return false;    
}

bool Dino::isFeetTouchingTopLadder() const {
    Hfyda2TileManager& rTM = this->m_pEngine->getTileManager();
    std::vector<std::vector<int>> vvMapSchema = rTM.getMapSchema();      
        
    for ( int iCol = 0 ; iCol < rTM.getTileColSize() ; iCol++ ) {
        for ( int iRow = 0 ; iRow < rTM.getTileRowSize() ; iRow++ ) {
            switch (vvMapSchema[iRow][iCol]) {
                case 21:
                    if(CollisionDetection::checkRectangles(getDrawingRegionLeft()+ 16, getDrawingRegionRight() - 16, getDrawingRegionBottom(), getDrawingRegionBottom(),
                        rTM.getLeftDrawingRegionForMapX(iCol) + 16, rTM.getRightDrawingRegionForMapX(iCol) - 16, rTM.getTopDrawingRegionForMapY(iRow), rTM.getTopDrawingRegionForMapY(iRow))) {     
                        return true;
                }
            }
        }
    }

    // printf("not touching top ladder\n");
    return false;  
}

bool Dino::isTouchingLadder() const {
    Hfyda2TileManager& rTM = m_pEngine->getTileManager();    
    std::vector<std::vector<int>> vvMapSchema = rTM.getMapSchema();      

    // using namespace std;

    // very hacky way of making sure dino can only climb if within enough pixels of the ladder
    for ( int iCol = 0 ; iCol < rTM.getTileColSize() ; iCol++ ) {
        for ( int iRow = 0 ; iRow < rTM.getTileRowSize() ; iRow++ ) {
            if(vvMapSchema[iRow][iCol] >= 20 && vvMapSchema[iRow][iCol] <= 22) {
                if(CollisionDetection::checkRectangles(getDrawingRegionLeft() + 16, getDrawingRegionRight() - 16, getDrawingRegionTop(), getDrawingRegionBottom(),
                    rTM.getLeftDrawingRegionForMapX(iCol) + 16, rTM.getRightDrawingRegionForMapX(iCol) - 16, rTM.getTopDrawingRegionForMapY(iRow) + 2, rTM.getBottomDrawingRegionForMapY(iRow)- 2)) {
                    // printf("touching ladder\n");
                    return true;
                }
            } 
        }
    }

    return false;    
}

bool Dino::isGoingToTouchEmptyPlatform(Direction eDirectionState) const {
    Hfyda2TileManager& rTM = m_pEngine->getTileManager();
    int iNextScreenX;
    eDirectionState ? iNextScreenX = getDrawingRegionRight() + 3 : iNextScreenX = getDrawingRegionLeft() - 6;
    
    switch(rTM.getMapValue(rTM.getMapXForScreenX(iNextScreenX), rTM.getMapYForScreenY(getDrawingRegionBottom()))) {
        case 0:
        case 26:
            return true;
        default:
            return false;
    }    
}

void Dino::applyGravity() {
    this->m_iCurrentScreenY += 5;
}

void Dino::attachObserver(Monster* monster) {
    m_vMonsterObservers.push_back(monster);
}

void Dino::detachObserver(Monster* pMonster) {
    auto it = std::find(m_vMonsterObservers.begin(), m_vMonsterObservers.end(), pMonster);    
    if (it != m_vMonsterObservers.end()) m_vMonsterObservers.erase(it);
}

void Dino::notifyLastXPosition() {

    for(std::vector<Monster*>::iterator it = m_vMonsterObservers.begin(); it != m_vMonsterObservers.end(); it++) {
        (*it)->updateDinoPos(m_iCurrentScreenX);
    }
}


