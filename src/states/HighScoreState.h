#ifndef HIGHSCORESTATE_H
#define HIGHSCORESTATE_H

#include "State.h"
#include "../CoordinateMappingFlip.h"

class Hfyda2Engine;

class HighScoreState : public State {
    public:

        HighScoreState(Hfyda2Engine& rEngine);
        ~HighScoreState();
        
        void setupBackground(Hfyda2Engine& rEngine) override;
        void setupObjects(Hfyda2Engine& rEngine) override;
        void keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) override;
        void drawStringsOnTop(Hfyda2Engine& rEngine) override;
        void preMainLoopHook(Hfyda2Engine& rEngine);
        void copyBackgroundBuffer(Hfyda2Engine& rEngine);

        typedef struct {
            std::string name;
            int highestLevel;
            int livesLeft;
        } Score;

    private:

        std::vector<Score> m_vScoreList;
        
        int m_iOffset;

        void loadHighScore(std::string sPath);

        CoordinateMappingFlip m_oFlip;

};

namespace HighScoreStateHelpers {
    void switchScreenToMenu(Hfyda2Engine& rEngine, State* pState);
    bool sort(HighScoreState::Score const& lhs, HighScoreState::Score const& rhs);

};



#endif
