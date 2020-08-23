#ifndef NEWGAMESTATE_H
#define NEWGAMESTATE_H

#include "State.h"
#include "../CoordinateMappingFlip.h"
#include "../SimpleImage.h"

class Hfyda2Engine;

class NewGameState : public State {
    public:

        NewGameState();
        ~NewGameState();
        
        void setupBackground(Hfyda2Engine& rEngine) override;
        void setupObjects(Hfyda2Engine& rEngine) override;
        void keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) override;
        void drawStringsOnTop(Hfyda2Engine& rEngine) override;
        void preMainLoopHook(Hfyda2Engine& rEngine) override;
        void copyBackgroundBuffer(Hfyda2Engine& rEngine) override;

        std::string getPlayerName() { return m_sPlayerName; }


    private:
        
        std::string m_sPlayerName;

        CoordinateMappingFlip m_oFlip;

        int m_iOffset;

};

namespace NewGameStateHelpers
{
    void switchScreenToGame(Hfyda2Engine& rEngine, State* pState);
    void switchScreenToMenu(Hfyda2Engine& rEngine, State* pState);
    
}; 

#endif
