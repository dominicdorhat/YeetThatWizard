#ifndef ENDSTATE_H
#define ENDSTATE_H

#include "State.h"

class Hfyda2Engine;
class SimpleImage;

class EndState : public State {
    public: 
        // win or lose, highest level
        EndState(Hfyda2Engine& rEngine, int iHighestLevel, int iLivesLeft, bool bWin, std::string sPlayerName);
        ~EndState();

        void setupBackground(Hfyda2Engine& pEngine);
        void setupObjects(Hfyda2Engine& pEngine);
        void keyPressHandler(Hfyda2Engine& pEngine, int iKeyCode);
        // void copyBackgroundBuffer(Hfyda2Engine& rEngine);
        void copyBackgroundBuffer(Hfyda2Engine& rEngine) { rEngine.getForegroundSurface()->copyEntireSurface(rEngine.getBackgroundSurface()); };

        void preMainLoopHook(Hfyda2Engine& rEngine);
        void drawStringsOnTop(Hfyda2Engine& rEngine);


    private:

        void updatePlayerData(std::string sPath, std::string sPlayerName, int iHighestLevel, int iLivesLeft);
        
        SimpleImage m_oLivesSprite;
        std::string m_sPlayerName;

        int m_iHighestLevel;
        int m_iLivesLeft;
        int m_iOffset;        

        int m_iImageScale;

        bool m_bWin;
};

namespace EndStateHelpers
{
    void switchScreenToMenu(Hfyda2Engine& rEngine, State* pState);
    void quit(Hfyda2Engine& rEngine, State* pState);
    
}; 
#endif