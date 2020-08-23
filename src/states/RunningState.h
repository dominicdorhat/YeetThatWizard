#ifndef RUNNINGSTATE_H
#define RUNNINGSTATE_H

#include "State.h"

class Spawner;
class Timer;

class RunningState : public State { 

    friend class PauseState;

    private:
        typedef struct  {
            int monsterTypes;
            int meteorCooldown;
            std::vector<int> meteorSpeed;
            int wizardSpeed;
            int minMonsters;
            bool lastLevel;

        } LevelData;

        typedef struct {
            std::string sPlayerName;
            int iDinoLives;
            int iCurrentLevel;
            bool bLastLevel;

        } PlayerData;

    public:

        RunningState(Hfyda2Engine* pEngine, std::string sPlayerName);
        RunningState(Hfyda2Engine* pEngine, RunningState::LevelData sLevelData, RunningState::PlayerData sPlayerData);
        
        ~RunningState(void);

        void setupBackground(Hfyda2Engine& pEngine) override;
        void setupObjects(Hfyda2Engine& pEngine) override;
        void keyPressHandler(Hfyda2Engine& rEngine, int iKeyCode) override;
        void drawStringsOnTop(Hfyda2Engine& rEngine) override;
        void preMainLoopHook(Hfyda2Engine& rEngine) override;
        void copyBackgroundBuffer(Hfyda2Engine& rEngine) { rEngine.getForegroundSurface()->copyEntireSurface(rEngine.getBackgroundSurface()); };

        void winLevel() { m_bLevelWin = true; }
        void respawnAll() { m_bDinoDamaged = true; }

        // objects that use this will not extend the lifetime of the pointer, so returning the raw pointer in this case is fine
        Spawner* getSpawner() { return m_pSpawner.get(); }

    private:
    
        bool m_bLevelWin;
        bool m_bTimerStarted;
        bool m_bDinoDamaged;

        std::unique_ptr<Timer> m_upWinLevelFreezeTimer; 

        std::shared_ptr<Spawner> m_pSpawner;    

        SimpleImage m_oDinoSpriteSheet;

        LevelData m_levelData;
        PlayerData m_playerData;

        void loadLevelData(std::string sPath);
        
        const std::vector<std::string> m_vsLevels = { "Level1", "Level2", "Level3" };

};

#endif 