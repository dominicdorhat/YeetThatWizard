#ifndef GAMEACTOR_H
#define GAMEACTOR_H

#include "../DisplayableObject.h"
#include "../Hfyda2Engine.h"
#include "../InputHandler.h"

#include <memory>

class Command;
class SimpleImage;

enum Direction { LEFT, RIGHT};
extern Direction m_eDirectionState;

class GameActor : public DisplayableObject {
    public: 
        GameActor(Hfyda2Engine* pEngine) 
            :   DisplayableObject(pEngine, 100, 100, true),   
                m_pEngine(pEngine),
                m_pCommand(nullptr),
                m_iXVelocity(0) {} // DisplayableObject(pEngine, 100, 100, true)
        virtual ~GameActor(void) {}
        
        virtual void moveLeft() = 0;
        virtual void moveRight() = 0;
        virtual void climbUp() = 0;
        virtual void climbDown() = 0;
        virtual void attack() = 0;

        virtual void virtDraw() = 0;
        virtual void virtDoUpdate(int iCurrentTime) = 0;
        virtual long virtNotify( int iSignalNumber, int x, int y ) = 0;
        // virtual void virtKeyDown(int iKeyCode);
        virtual void virtKeyUp(int iKeyCode) = 0;

    protected:
        
        Hfyda2Engine* m_pEngine;

        // enum Direction { LEFT, RIGHT};

        SimpleImage m_oSpriteSheet;
        SimpleImage m_oSpriteSheetFlipped; 
        std::shared_ptr<Command> m_pCommand;
        std::unique_ptr<InputHandler> m_pInputHandler;

        int m_iImageScale;
        int m_iXVelocity;

        bool m_bFreeze;

        void setPosition(int iX, int iY) = 0;

        
};

#endif