#include "../header.h"

#include "Fireball.h"

#include "../helper/Helper.h"
#include "../CollisionDetection.h"
#include "../monsters/Monster.h"
#include "../particles/BlueBeam.h"

Fireball::Fireball(Hfyda2Engine* pEngine, int iSpawnX, int iSpawnY, Direction eDirection)
    :   Particle(pEngine),
        m_iSpeedX(8),
        m_iMaximumDistance(500),
        m_eDirectionState(eDirection) { 

        m_iImageScale = 2;
        m_oSpriteSheet = pEngine->loadImage("resources/images/effects/fireball/fireball-spritesheet.png", true).resizeTo(160 * m_iImageScale, 19 * m_iImageScale);
        m_oSpriteSheetFlipped = pEngine->loadImage("resources/images/effects/fireball/fireball-left-spritesheet.png", true).resizeTo(160 * m_iImageScale, 19 * m_iImageScale);
     
        m_iDrawWidth = 19 * m_iImageScale;
        m_iDrawHeight = 18 * m_iImageScale;
        
        m_iCurrentScreenX = m_eDirectionState ?  iSpawnX + 16 : iSpawnX;
        m_iCurrentScreenY = iSpawnY;
        
        m_bDinoDamaged = false;
    }

Fireball::~Fireball() {}

void Fireball::virtDraw() {

    int iTick = getEngine()->getModifiedTime() / 100;
	int iFrame = iTick % 5;

    
    // m_pEngine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY, getDrawingRegionRight(), getDrawingRegionBottom(), 0x000000);
    switch(m_eDirectionState) {
        case LEFT:
            m_oSpriteSheetFlipped.renderImageWithMask(
                m_pEngine->getForegroundSurface(), 
                m_mFrameLeftLookUpTable[iFrame] * m_iImageScale, 0,
                m_iCurrentScreenX, m_iCurrentScreenY,
                getDrawWidth(), getDrawHeight(),
                0x000000);
            break;
        case RIGHT:
            m_oSpriteSheet.renderImageWithMask(
                m_pEngine->getForegroundSurface(), 
                m_mFrameLookUpTable[iFrame] * m_iImageScale, 0,
                m_iCurrentScreenX, m_iCurrentScreenY,
                getDrawWidth(), getDrawHeight(),
                0x000000);
            break;
    }

}

void Fireball::virtDoUpdate(int iCurrentTime) {
    if(m_pEngine->isPaused()) return;
    if(m_bDinoDamaged) return;

    m_eDirectionState ? m_iCurrentScreenX += m_iSpeedX : m_iCurrentScreenX -= m_iSpeedX;

    DisplayableObject* pObject;
    for(int itr = 0; (pObject = m_pEngine->getDisplayableObject(itr)) != NULL; itr++) {

        if(pObject == this) continue;
        if(pObject == nullptr) continue;
        
        // Fireball collison
        if(Helper::instanceof<Monster>(pObject) || Helper::instanceof<BlueBeam>(pObject)) {
            if(CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionTop(), getDrawingRegionBottom(),
                pObject->getDrawingRegionLeft() + 8, pObject->getDrawingRegionRight() - 8, pObject->getDrawingRegionTop(), pObject->getDrawingRegionBottom())) {   
                                
                if(Helper::instanceof<BlueBeam>(pObject)) {
                    dynamic_cast<BlueBeam*>(pObject)->die();

                } else {
                    Monster* monster = dynamic_cast<Monster*>(pObject);
                    if(monster->getCurrentState() == Monster::DAMAGED) { // let fireball pass through dying monsters                        
                        break;
                    } else {
                        monster->die();
                    }

                }

                m_pEngine->drawableObjectsChanged();
                m_pEngine->removeDisplayableObject(this);
        
                delete this;
                return;
            }              
        }
    }

        
    if(getDrawingRegionLeft() > m_pEngine->getWindowWidth() || getDrawingRegionRight() < 0) {
        m_pEngine->drawableObjectsChanged();
        m_pEngine->removeDisplayableObject(this);
        
        delete this;
        return;
    }

    redrawDisplay();

}

long Fireball::virtNotify( int iSignalNumber, int x, int y ) {
    switch(iSignalNumber) {
        case 1:
            m_bDinoDamaged = true;
            break;
        default:
            break;
    }
    return 1;
}
