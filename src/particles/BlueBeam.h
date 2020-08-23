#ifndef BLUEBEAM_H
#define BLUEBEAM_H

#include "Particle.h"
#include "../monsters/Monster.h"

class BlueBeam : public Particle {

    public:
        BlueBeam(Hfyda2Engine* pEngine, int iSpawnX, int iSpawnY, Monster::Direction eDirection);
        ~BlueBeam(void);

        virtual void virtDraw() override;
        virtual void virtDoUpdate(int iCurrentTime) override;
        long virtNotify( int iSignalNumber, int x, int y ) override;

        void die();

    private:

        Monster::Direction m_eDirectionState;

        int m_iSpeedX; // speed of meteor

        std::map<int, int> m_mFrameLookUpTable {
            {0, 11}
        };

};

#endif