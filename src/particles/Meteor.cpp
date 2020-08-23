#include "../header.h"

#include "Meteor.h"

Meteor::Meteor(Hfyda2Engine* pEngine, int iSpawnX, int iSpeedY)
    :   Particle(pEngine),
        m_iSpeedY(iSpeedY) { 
        m_iImageScale = 3;
        m_oSpriteSheet = pEngine->loadImage("resources/images/effects/meteor/meteor-spritesheet.png", true).resizeTo(19 * m_iImageScale, 241 * m_iImageScale);
                
        m_iDrawWidth = 19 * m_iImageScale;
        m_iDrawHeight = 34 * m_iImageScale;
        m_iCurrentScreenX = iSpawnX;
        m_bDinoDamaged = false;
    }

Meteor::~Meteor() {}

void Meteor::virtDraw() {

    int iTick = getEngine()->getModifiedTime() / 100;
	int iFrame = iTick % 5;

    
    // m_pEngine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY, getDrawingRegionRight(), getDrawingRegionBottom(), 0x000000);
    m_oSpriteSheet.renderImageWithMask(
        m_pEngine->getForegroundSurface(), 
        0, m_mFrameLookUpTable[iFrame] * m_iImageScale,
        m_iCurrentScreenX, m_iCurrentScreenY,
        getDrawWidth(), getDrawHeight(),
        0x000000);
    
}

void Meteor::virtDoUpdate(int iCurrentTime) {
    if(m_pEngine->isPaused()) return;
    if(m_bDinoDamaged) return;
    
    m_iCurrentScreenY += m_iSpeedY;

    // TODO: homing movement for last level
        
    if(m_iCurrentScreenY > m_pEngine->getWindowHeight()) {
        m_pEngine->drawableObjectsChanged();
        // m_pEngine->storeObjectInArray(m_iIndex, nullptr);
        m_pEngine->removeDisplayableObject(this);
        
        delete this;
        return;
    }
    redrawDisplay();

}

long Meteor::virtNotify( int iSignalNumber, int x, int y ) {
    switch(iSignalNumber) {
        case 1:
            m_bDinoDamaged = true;
            break;
        default:
            break;
    }
    return 1;
}