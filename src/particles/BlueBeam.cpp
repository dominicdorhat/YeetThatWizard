#include "../header.h"

#include "BlueBeam.h"
#include "../helper/Helper.h"
#include "../CollisionDetection.h"

BlueBeam::BlueBeam(Hfyda2Engine* pEngine, int iSpawnX, int iSpawnY, Monster::Direction eDirection)
    :   Particle(pEngine),
        m_iSpeedX(6),
        m_eDirectionState(eDirection) { 

        m_iImageScale = 4;
        m_oSpriteSheet = pEngine->loadImage("resources/images/effects/blue/Blue_Beam.png", true).resizeTo(32 * m_iImageScale, 64 * m_iImageScale);        
     
        m_iDrawWidth = 10 * m_iImageScale;
        m_iDrawHeight = 1 * m_iImageScale;
        
        m_iCurrentScreenX = m_eDirectionState ?  iSpawnX + 16 : iSpawnX - m_iDrawWidth;
        m_iCurrentScreenY = iSpawnY + 12;
        
        m_bDinoDamaged = false;
}

BlueBeam::~BlueBeam() {}

void BlueBeam::virtDraw() {

    // int iTick = getEngine()->getModifiedTime() / 100;
	// int iFrame = iTick % 5;

    m_pEngine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY, getDrawingRegionRight(), getDrawingRegionBottom(), 0x000000);
    m_oSpriteSheet.renderImageWithMask(
        m_pEngine->getForegroundSurface(), 
        m_mFrameLookUpTable[0] * m_iImageScale, 16 * m_iImageScale,
        m_iCurrentScreenX, m_iCurrentScreenY,
        getDrawWidth(), getDrawHeight(),
        0x000000);


}

void BlueBeam::virtDoUpdate(int iCurrentTime) {
    if(m_pEngine->isPaused()) return;
    if(m_bDinoDamaged) return;
    
    m_eDirectionState ? m_iCurrentScreenX += m_iSpeedX : m_iCurrentScreenX -= m_iSpeedX;

    // DisplayableObject* pObject;
    // for(int itr = 0; (pObject = m_pEngine->getDisplayableObject(itr)) != NULL; itr++) {

    //     if(pObject == this) continue;
    //     if(pObject == nullptr) continue;
        
    //     // Fireball collison
    //     if(Helper::instanceof<Monster>(pObject)) {
    //         if(CollisionDetection::checkRectangles(getDrawingRegionLeft(), getDrawingRegionRight(), getDrawingRegionTop(), getDrawingRegionBottom(),
    //             pObject->getDrawingRegionLeft(), pObject->getDrawingRegionRight(), pObject->getDrawingRegionTop(), pObject->getDrawingRegionBottom())) {   
                
    //             dynamic_cast<Monster*>(pObject)->die();
    //             m_pEngine->drawableObjectsChanged();
    //             m_pEngine->removeDisplayableObject(this);
        
    //             delete this;
    //             return;
    //         }              
    //     }
    // }

        
    if(getDrawingRegionLeft() > m_pEngine->getWindowWidth() || getDrawingRegionRight() < 0) {
        m_pEngine->drawableObjectsChanged();
        m_pEngine->removeDisplayableObject(this);
        
        delete this;
        return;
    }

    redrawDisplay();

}

long BlueBeam::virtNotify( int iSignalNumber, int x, int y ) {
    switch(iSignalNumber) {
        case 1:
            m_bDinoDamaged = true;
            break;
        default:
            break;
    }
    return 1;
}

void BlueBeam::die() {
        m_pEngine->drawableObjectsChanged();
        m_pEngine->removeDisplayableObject(this);

        delete this;
        return;
}