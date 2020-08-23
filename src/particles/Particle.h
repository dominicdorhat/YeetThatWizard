#ifndef PARTICLE_H
#define PARTICLE_H

#include "../DisplayableObject.h"
#include "../Hfyda2Engine.h"

class SimpleImage;

class Particle : public DisplayableObject {
    public:
        Particle(Hfyda2Engine* pEngine)
        :   DisplayableObject(pEngine, 100, 100, true),
            m_pEngine(pEngine) {};

        virtual ~Particle(void) {}

        virtual void virtDraw() = 0;
        virtual void virtDoUpdate(int iCurrentTime) = 0;
        virtual long virtNotify( int iSignalNumber, int x, int y ) = 0;

    protected:
        Hfyda2Engine* m_pEngine;

        SimpleImage m_oSpriteSheet;

        static const int MAX_VEL = 10;

        int m_iFrame;
        int m_iVelX, m_iVelY;

        int m_iImageScale;
        
        bool m_bDinoDamaged;
};

#endif