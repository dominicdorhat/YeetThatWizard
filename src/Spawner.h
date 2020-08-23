#ifndef SPAWNER_H
#define SPAWNER_H

#include "Hfyda2Engine.h"
#include "helper/Timer.h"

#include <memory>

class Spawner {
    public:
        Spawner(Hfyda2Engine* pEngine);
        ~Spawner() { m_upSpawnCooldown.reset(); }

        // static Spawner& getInstance(Hfyda2Engine* pEngine) {
        //     static Spawner oInstance(pEngine);
        //     return oInstance;
        // }

    private:
        // Spawner(Hfyda2Engine* pEngine);

        Hfyda2Engine* m_pEngine;

        struct SpawnPoints {
            int XPos;
            int YPos;
        };
        
        std::vector<SpawnPoints> m_vSpawnPoints;    
        
        int m_iCurrentSpawnCount;
        int m_iMonsterTypes;

        const int MAX_SPAWN = 6;

    public:

        // Spawner(Spawner const&) = delete;
        // void operator=(Spawner const&) = delete;
        
        std::unique_ptr<Timer> m_upSpawnCooldown;

        void init(int iMonsterTypes);
        void initialSpawn();
        void spawn();
        void updateSpawnCount() { m_iCurrentSpawnCount--; }
        int getSpawnCount() { return m_iCurrentSpawnCount; }
};

#endif