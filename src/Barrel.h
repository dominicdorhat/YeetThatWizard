#ifndef BARREL_H
#define BARREL_H

#pragma once

#include "BaseEngine.h"
#include "SimpleImage.h"
#include "ImageObject.h"
#include "MovementPosition.h"

class Barrel : public ImageObject {

    public:
        Barrel(BaseEngine* pEngine, int xPos, int yPos);
        ~Barrel(void);

        virtual void virtDraw() override;
        virtual void virtDoUpdate(int iCurrentTime) override;

    private:
        enum Direction { LEFT, RIGHT, UP, DOWN };
        
        int speedX, jumpHeight, imgHeight, imgWidth;

        MovementPosition horizontalMovement;  
        // MovementPosition verticalMovement;      
        
        Direction directionState;
        SimpleImage barrelImage;

        void setUpJumpMovement(int iCurrentTime);
        void changeDirection();
        void resetPosition();

};

#endif