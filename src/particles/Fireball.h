#ifndef FIREBALL_H
#define FIREBALL_H

#include "Particle.h"
#include "../actors/GameActor.h"

class Fireball : public Particle {

    public:
        Fireball(Hfyda2Engine* pEngine, int iSpawnX, int iSpawnY, Direction eDirection);
        ~Fireball(void);

        virtual void virtDraw() override;
        virtual void virtDoUpdate(int iCurrentTime) override;
        long virtNotify( int iSignalNumber, int x, int y ) override;

        // void die();

    private:

        Direction m_eDirectionState;
        SimpleImage m_oSpriteSheetFlipped;

        const int m_iMaximumDistance;
        
        int m_iSpeedX; // speed of meteor
        // int m_iOriginalPosition;
        // int m_iCurrentPosition;
        

        std::map<int, int> m_mFrameLookUpTable {
            {0, 0},
            {1, 28},
            {2, 55},
            {3, 84},
            {4, 114}
        };

        std::map<int, int> m_mFrameLeftLookUpTable {
            {0, 141},
            {1, 113},
            {2, 86},
            {3, 57},
            {4, 27}
        };
};

#endif