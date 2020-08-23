#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "State.h"
#include "RunningState.h"

class Hfyda2Engine;
class RunningState;

class PauseState : public State { 

    public:

        PauseState(Hfyda2Engine& rEngine, RunningState::LevelData sLevelData, RunningState::PlayerData sPlayerData);

        ~PauseState(void);

        void setupBackground(Hfyda2Engine& rEngine) override;
        void setupObjects(Hfyda2Engine& rEngine) override;
        void keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) override;
        void drawStringsOnTop(Hfyda2Engine& rEngine) override;
        void preMainLoopHook(Hfyda2Engine& rEngine) {}
        void copyBackgroundBuffer(Hfyda2Engine& rEngine) { rEngine.getForegroundSurface()->copyEntireSurface(rEngine.getBackgroundSurface()); };
    
    private:

        RunningState::LevelData m_levelData;
        RunningState::PlayerData m_playerData;


};

#endif