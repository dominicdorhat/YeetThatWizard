#ifndef BASESTATE_H
#define BASESTATE_H

#include <memory.h>

#include "../Hfyda2Engine.h"

class Hfyda2Engine;

class State { // State base class
    public:

        // enum CurrentScreen { MENU, NEW_GAME, LOAD_GAME, GAME, PAUSED_GAME };        

        State() {}
        virtual ~State() {} // prevent resource leak of specialized objects not calling their own destructors
        virtual void setupBackground(Hfyda2Engine& rEngine) = 0;
        virtual void setupObjects(Hfyda2Engine& rEngine) = 0;
        virtual void keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) = 0;
        virtual void drawStringsOnTop(Hfyda2Engine& rEngine) = 0;
        virtual void preMainLoopHook(Hfyda2Engine& rEngine) = 0;

        virtual void copyBackgroundBuffer(Hfyda2Engine& rEngine) = 0;

        void setState(Hfyda2Engine& rEngine, State* pState) {
            rEngine.m_upState.reset(pState);
        }

        // CurrentScreen getCurrentScreen() { return this->m_eCurrentScreen; }
        // void setCurrentScreen(CurrentScreen eCurrentScreen) { this->m_eCurrentScreen = eCurrentScreen; }

    protected:

        // CurrentScreen m_eCurrentScreen;



};

#endif