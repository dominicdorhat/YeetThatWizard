#ifndef METEOR_H
#define METEOR_H

#include "Particle.h"

class Meteor : public Particle {
    public:
        Meteor(Hfyda2Engine* pEngine, int iSpawnX, int iSpeedY);
        ~Meteor(void);

        virtual void virtDraw() override;
        virtual void virtDoUpdate(int iCurrentTime) override;
        long virtNotify( int iSignalNumber, int x, int y ) override;

    private:

        int m_iSpeedY; // speed of meteor

        std::map<int, int> m_mFrameLookUpTable {
            {0, 3},
            {1, 46},
            {2, 98},
            {3, 146},
            {4, 201}
        };
};

#endif