#include "header.h"

#include "Spawner.h"
#include "monsters/Blue.h"
#include "monsters/Red.h"

Spawner::Spawner(Hfyda2Engine* pEngine)
    :   m_pEngine(pEngine) {

        m_upSpawnCooldown = std::unique_ptr<Timer>(new Timer(5000));
}

void Spawner::init(int iMonsterTypes) {
    m_iCurrentSpawnCount = 0;
    m_iMonsterTypes = iMonsterTypes;
    
    Hfyda2TileManager& tm = m_pEngine->getTileManager();
    
    std::vector<SpawnPoints> vSpawnPoints;    
    m_vSpawnPoints.swap(vSpawnPoints);

    for(int row = 0; row < tm.getTileRowSize(); row++) {
        for(int col = 0; col < tm.getTileColSize(); col++) {

            if(tm.getMapValue(col, row) == 23) {
                SpawnPoints sSpawnPoint = { tm.getLeftDrawingRegionForMapX(col), tm.getTopDrawingRegionForMapY(row) };
                m_vSpawnPoints.push_back(sSpawnPoint);
            }
        }
    }
}

void Spawner::initialSpawn() { 
    if(m_vSpawnPoints.empty()) return;

    for(int itr = 0; itr < 2; itr++) {
        // std::cout << "spawning at: " << m_vSpawnPoints[iSpawn].XPos << m_vSpawnPoints[iSpawn].YPos << "\n";
        m_pEngine->appendObjectToArray(new Red(m_pEngine, m_vSpawnPoints[itr].XPos, m_vSpawnPoints[itr].YPos));    
        m_iCurrentSpawnCount++;
        if (m_vSpawnPoints.size() < 2) break;        
    }
    
    m_upSpawnCooldown->startTimer();
}

void Spawner::spawn() {
    if(m_vSpawnPoints.empty()) return;

    if(m_upSpawnCooldown->check()) {
        if(m_iCurrentSpawnCount <= MAX_SPAWN) {
            int iSpawn = rand() % m_vSpawnPoints.size();
            switch(m_iMonsterTypes) {
                case 100:
                    m_pEngine->appendObjectToArray(new Red(m_pEngine, m_vSpawnPoints[iSpawn].XPos, m_vSpawnPoints[iSpawn].YPos));
                    break;

                case 110:
                    rand() % 2 ? 
                            m_pEngine->appendObjectToArray(new Blue(m_pEngine, m_vSpawnPoints[iSpawn].XPos, m_vSpawnPoints[iSpawn].YPos)) : 
                            m_pEngine->appendObjectToArray(new Red(m_pEngine, m_vSpawnPoints[iSpawn].XPos, m_vSpawnPoints[iSpawn].YPos));
                    break;
                    
                default:
                    break;
            }
            m_iCurrentSpawnCount++;
        }
    }
}