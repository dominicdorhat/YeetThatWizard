#ifndef BLUE_H
#define BLUE_H

#include "Monster.h"
#include "../MovementPosition.h"

class Blue : public Monster {
    public:
        Blue(Hfyda2Engine* pEngine, int iXPos, int iYPos);
        ~Blue();

        void virtDraw() override;
        void virtDoUpdate(int iCurrentTime) override;
        long virtNotify( int iSignalNumber, int x, int y ) override;

        void updateDinoPos(int iXPos) override;
        void die() override;        
    
    private:

        SimpleImage m_oSpriteSheetIdle;
        SimpleImage m_oSpriteSheetRun;
        SimpleImage m_oSpriteSheetAttack;
        SimpleImage m_oSpriteSheetDamaged;

        MovementPosition m_oMovement;

        bool m_bCompletedMovement;

        // enum Action { IDLE, WALK, ATTACK, DAMAGED};
        // Action m_eActionState;

        std::unique_ptr<Timer> m_upMoveCooldown;
        std::unique_ptr<Timer> m_upDeathFreezeTimer;

        void makeMove(int iCurrentTime);
        void attack();
        void setPosition(int iX, int iY);

        std::map<int, int> m_mFrameWalkingLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 10},
            {1, 42},
            {2, 74},
            {3, 106},
            {4, 138},
            {5, 170}
        };

        std::map<int, int> m_mFrameIdleLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 10},
            {1, 42},
            {2, 74},
            {3, 106}    
        };

        std::map<int, int> m_mFrameAttackLookUpTable = {
            {0, 9},
            {1, 42},
            {2, 74}
        };

        std::map<int, int> m_mFrameDamagedLookUpTable = {
            {0, 10},
            {1, 42}
        };

};

#endif