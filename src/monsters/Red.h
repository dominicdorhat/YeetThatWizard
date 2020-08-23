#ifndef RED_H
#define RED_H

#include "Monster.h"
#include "../MovementPosition.h"

class Red : public Monster {
    public:
        Red(Hfyda2Engine* pEngine, int iXPos, int iYPos);
        ~Red();

        void virtDraw() override;
        void virtDoUpdate(int iCurrentTime) override;
        long virtNotify( int iSignalNumber, int x, int y ) override;

        void updateDinoPos(int iXPos) override;
        void die() override;        
    
    private:

        SimpleImage m_oSpriteSheetIdle;
        SimpleImage m_oSpriteSheetRun;
        SimpleImage m_oSpriteSheetDamaged;

        MovementPosition m_oMovement;

        bool m_bCompletedMovement;

        std::unique_ptr<Timer> m_upMoveCooldown;
        std::unique_ptr<Timer> m_upDeathFreezeTimer;

        void attack() {}
        void makeMove(int iCurrentTime);
        void setPosition(int iX, int iY);

        std::map<int, int> m_mFrameWalkingLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 7},
            {1, 39},
            {2, 72},
            {3, 104},
            {4, 136},
            {5, 168}
        };

        std::map<int, int> m_mFrameIdleLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 9},
            {1, 41},
            {2, 73},
            {3, 105}    
        };

        std::map<int, int> m_mFrameDamagedLookUpTable = {
            {0, 8},
            {1, 40}
        };

};

#endif