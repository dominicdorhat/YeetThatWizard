#ifndef WIZARD_H
#define WIZARD_H

#include "../MovementPosition.h"
#include "Monster.h"

class Timer;

class Wizard : public Monster {
    public:
        Wizard(Hfyda2Engine* pEngine, int iWizardSpeed, int iSummonSpeed, std::vector<int> iMeteorSpeed);
        ~Wizard();
        

        void virtDraw() override;
        void virtDoUpdate(int iCurrentTime) override;
        long virtNotify( int iSignalNumber, int x, int y ) override;

        void updateDinoPos(int iXPos) override;

        void makeMove(int iCurrentTime);
        
        void die() override;
 

    protected:
        void setPosition(int iX, int iY) override;
        void attack() override;

    private:
        
        // enum Action { IDLE, WALK, ATTACK };
        // Action m_eActionState;

        SimpleImage m_oSpriteSheetFlipped;
        MovementPosition m_oMovement;

        int m_iCooldown;
        int m_iWizardSpeed;

        std::vector<int> m_iMeteorSpeed;
        
        bool m_bCompletedMovement;

        std::unique_ptr<Timer> m_pTimerCooldown;
        std::unique_ptr<Timer> m_pTimerMeteor;

        std::map<int, int> m_mFrameWalkingLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 118},
            {1, 137},
            {2, 155},
            {3, 175},
        };

        std::map<int, int> m_mFrameWalkingLeftLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 194},
            {1, 174},
            {2, 155},
            {3, 137},
        };


        std::map<int, int> m_mFrameIdleLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 3},
            {1, 22},
            {2, 41},
            {3, 59},
            {4, 79},
            {5, 98}      
        };

        std::map<int, int> m_mFrameIdleLeftLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 309},
            {1, 290},
            {2, 271},
            {3, 252},
            {4, 233},
            {5, 214}         
        };

        std::map<int, int> m_mFrameAttackLookUpTable = {
            {0, 210},
            {1, 229},
            {2, 247},
            {3, 265},
            {4, 286},
            {5, 305}
        };

        std::map<int, int> m_mFrameAttackLeftLookUpTable = {
            {0, 97},
            {1, 78},
            {2, 61},
            {3, 43},
            {4, 22},
            {5, 2}
        };

};

#endif