#ifndef MONSTER_H
#define MONSTER_H

#include "../DisplayableObject.h"
#include "../Hfyda2Engine.h"

class Timer;

class Monster : public DisplayableObject {
    public:
        Monster(Hfyda2Engine* pEngine)
            :   DisplayableObject(pEngine, 100, 100, true),
                m_pEngine(pEngine) {}

        virtual ~Monster() {}

        virtual void virtDraw() = 0;
        virtual void virtDoUpdate(int iCurrentTime) = 0;
        virtual long virtNotify( int iSignalNumber, int x, int y ) = 0;

        virtual void updateDinoPos(int iXPos) = 0;
        virtual void die() {}
        
        enum Direction { LEFT, RIGHT };
        enum Action { IDLE, WALK, ATTACK, DAMAGED};

        Action getCurrentState() { return m_eActionState; }
        
        bool m_bTimeToDie;
        

    protected:

        virtual void attack() = 0;
        virtual void setPosition(int iX, int iY) = 0;

        Hfyda2Engine* m_pEngine;

        Action m_eActionState;
        Direction m_eDirectionState;

        int m_iLastDinoPosX;
        int m_iImageScale;
        int m_iXVelocity;

        bool m_bFreeze;        

        SimpleImage m_oSpriteSheet;
};

#endif