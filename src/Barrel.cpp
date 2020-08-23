#include "header.h"

#include "Barrel.h"
#include "ImageManager.h"

using namespace std;

Barrel::Barrel(BaseEngine* pEngine, int xPos, int yPos) 
    : ImageObject(xPos, yPos, pEngine, "resources/images/barrel-roll-1.png", false),
        speedX(10),
        jumpHeight(10),
        imgWidth(50),
        imgHeight(50),
        directionState(RIGHT)
{
    barrelImage = pEngine->loadImage("resources/images/barrel-roll-1.png", true).resizeTo(imgWidth, imgHeight);
    setSize(barrelImage.getWidth(), barrelImage.getWidth());

    setVisible(true);
}

Barrel::~Barrel(void) {}

void Barrel::virtDraw() {   

    barrelImage.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0, 
    m_iCurrentScreenX + m_iStartDrawPosX, 
    m_iCurrentScreenY + m_iStartDrawPosY, 
    m_iDrawWidth, 
    m_iDrawHeight,
    0x005b7f);

}


void Barrel::virtDoUpdate(int iCurrentTime)  {
    if(m_pEngine->isPaused()) {
        return;
    }

    int iTick = getEngine()->getModifiedTime() / 200;
	int iFrame = (iTick % 4) + 1; 
    barrelImage = getEngine()->loadImage("resources/images/barrel-roll-" + std::to_string(iFrame) + ".png", true).resizeTo(imgWidth, imgHeight);

    // TODO: change direction when touching tile
    if(getXCentre() >= getEngine()->getWindowWidth()) {
            directionState = LEFT;
        } else if (m_iCurrentScreenX <= 10 ) {
            directionState = RIGHT;
        }

    if(horizontalMovement.hasMovementFinished(iCurrentTime)) {

        if(directionState == RIGHT ) {
            // cout <<"setting up Right" << endl;
            horizontalMovement.setup(m_iCurrentScreenX, m_iCurrentScreenY, 
                getEngine()->getWindowWidth(), 
                m_iCurrentScreenY + 100, 
                iCurrentTime,
                iCurrentTime + 2000);

        } else if(directionState == LEFT ) {
            // cout <<"setting up Left "<< iCurrentTime << " " <<  iCurrentTime + 2500 << endl;
            horizontalMovement.setup(m_iCurrentScreenX, m_iCurrentScreenY, 
                0, 
                m_iCurrentScreenY + 100, 
                iCurrentTime,
                iCurrentTime + 2000);
        }
    }

    if(!horizontalMovement.hasMovementFinished(iCurrentTime)) {  
        horizontalMovement.calculate(iCurrentTime);
        m_iCurrentScreenX = horizontalMovement.getX();
        m_iCurrentScreenY = horizontalMovement.getY();
    }    

    if (m_iCurrentScreenY > getEngine()->getWindowHeight())
        resetPosition();

    redrawDisplay();
}



void Barrel::setUpJumpMovement(int iCurrentTime) {
    horizontalMovement.setup(m_iCurrentScreenX, m_iCurrentScreenY, 
                m_iCurrentScreenX, m_iCurrentScreenY - 50, 
                // m_pEngine->getModifiedTime(), m_pEngine->getModifiedTime() + 100);
                iCurrentTime, iCurrentTime + 100);
}

void Barrel::changeDirection() {
    switch (directionState){
    case LEFT:
        directionState = RIGHT;
        break;
    case RIGHT:
        directionState = LEFT;
        break;
    
    default:
        break;
    }
}

void Barrel::resetPosition() {
    m_iCurrentScreenX = 0;
    m_iCurrentScreenY = 300;    
}
