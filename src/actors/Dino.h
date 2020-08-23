#ifndef DINO_H
#define DINO_H

#include "GameActor.h"
#include "../Hfyda2Engine.h"

class Monster;
class Timer;

class Dino : public GameActor {
    
    public:
        Dino(Hfyda2Engine* pEngine);
        ~Dino();
        
        void moveLeft() override;
        void moveRight() override;
        void climbUp() override;
        void climbDown() override; 
        void attack() override;

        // observer pattern to notify X position to monsters for "AI" purposes
        void attachObserver(Monster* pMonster);
        void detachObserver(Monster* pMonster);
        void notifyLastXPosition();

        void virtDraw() override;
        void virtDoUpdate(int iCurrentTime) override;
        void virtKeyUp(int iKeyCode);
        long virtNotify( int iSignalNumber, int x, int y ) override;


    protected:
        void setPosition(int iX, int iY);

    private:

         // UP, DOWN
        enum Action { IDLE, WALK, CLIMB, ATTACK, DAMAGED };

        Direction m_eDirectionState;
        Action m_eActionState;

        bool isFeetTouchingPlatform() const;
        bool isFeetTouchingTopLadder() const;
        bool isTouchingLadder() const;
        bool isGoingToTouchEmptyPlatform(Direction eDirectionState) const;
        bool isGoingToTouchTree(Direction eDirectionState) const;
        void applyGravity();

        int m_iYVelocity;

        std::vector<Monster*> m_vMonsterObservers; // observer pattern

        std::unique_ptr<Timer> m_pDeathFreezeTimer; 
        std::unique_ptr<Timer> m_pAttackCooldown;

        std::map<int, int> m_mFrameWalkingLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 100},
            {1, 124},
            {2, 148},
            {3, 172},
            {4, 196},
            {5, 220}
        };

        std::map<int, int> m_mFrameWalkingLeftLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 461},
            {1, 437},
            {2, 413},
            {3, 389},
            {4, 365},
            {5, 341}
        };


        std::map<int, int> m_mFrameIdleLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 4},
            {1, 28},
            {2, 52},  
            {3, 76}          
        };

        std::map<int, int> m_mFrameIdleLeftLookUpTable = { // xy coordinates for sprites in the spritesheet
            {0, 557},
            {1, 533},
            {2, 509},
            {3, 485}            
        };

        std::map<int, int> m_mFrameDamageLookUpTable = {
            {0, 340},
            {1, 362},
            {2, 386}            
        };

        std::map<int, int> m_mFrameDamageLeftLookUpTable = {
            {0, 221},
            {1, 199},
            {2, 175}            
        };

        std::map<int, int> m_mFrameAttackLookUpTable = {
            {0, 266},
            {1, 292}            
        };

        std::map<int, int> m_mFrameAttackLeftLookUpTable = {
            {0, 295},
            {1, 269},
        };
};

#endif