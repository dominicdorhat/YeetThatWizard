#ifndef HFYDA2ENGINE_H
#define HFYDA2ENGINE_H

#pragma once

#include "BaseEngine.h"
#include "Hfyda2TileManager.h"

#include <memory.h>

class State;

class Hfyda2Engine : public BaseEngine {

    friend class State;

    public:
        Hfyda2Engine(void);
        ~Hfyda2Engine(void);

        virtual void virtSetupBackgroundBuffer() override;
        virtual int virtInitialiseObjects() override;
        virtual void virtKeyDown( int iKeyCode ) override;
        virtual void virtMouseDown( int iButton, int iX, int iY) override;
        virtual void virtDrawStringsOnTop() override;
        virtual void virtMainLoopPreUpdate();

        void copyAllBackgroundBuffer(void);

        Hfyda2TileManager& getTileManager();

        State* getState() { return m_upState.get(); }


    private:

        // AUDIO
        static SDL_AudioSpec m_sWavSpec; // audio data format of the wav source data
        static SDL_AudioDeviceID m_uiDev;
        static Uint8* m_pWavBuffer; // buffer containing audio data
        static Uint32 m_uiWavlength; // length of the audio buffer, in bytes

        Hfyda2TileManager oTileManager;

        std::unique_ptr<State> m_upState;
    
        int m_iHitCounter;

    
};

namespace AudioHelper {
    void audioCallBack(void* userdata, Uint8* stream, int len);
};



#endif