#ifndef STARTSTATE_H
#define STARTSTATE_H

#include "State.h"
#include "../CoordinateMappingFlip.h"

class StartState : public State { 
    public:

        StartState();
        ~StartState(void);

        void setupBackground(Hfyda2Engine& rEngine) override;
        void setupObjects(Hfyda2Engine& rEngine) override;
        void keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) override;
        void drawStringsOnTop(Hfyda2Engine& rEngine) override;
        // void switchScreenToNewGame(Hfyda2Engine& rEngine);
        void preMainLoopHook(Hfyda2Engine& rEngine) override;

        void copyBackgroundBuffer(Hfyda2Engine& rEngine) override;

    private:
        int m_iOffset;

        CoordinateMappingFlip m_oFlip;


      
};

// static functions as a separate namespace to pass them as function pointers (dirty but i dont know what else i can do)
namespace StartStateHelpers
{
    void switchScreenToNewGame(Hfyda2Engine& rEngine, State* pState);
    void switchScreenToLoadGame(Hfyda2Engine& rEngine, State* pState);
    void exitGame(Hfyda2Engine& rEngine, State* pState);  
}; 
#endif 