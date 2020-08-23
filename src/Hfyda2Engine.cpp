#include "header.h"

#include "Hfyda2Engine.h"
#include "./states/StartState.h"

constexpr auto MUSIC_FILE = "resources/sounds/Title.wav";

Uint8* pAudioBuffer; // buffer containing audio data
Uint32 uiAudioLength; // length of below, in bytes

Uint8* pAudioBufferOri; // keeps the original audio data, so that audio_buffer can recopy it to replay
Uint32 uiAudioLengthOri; 


// =====================
// STATIC INITIALIZATION
// =====================
SDL_AudioSpec Hfyda2Engine::m_sWavSpec;
SDL_AudioDeviceID Hfyda2Engine::m_uiDev;
Uint8* Hfyda2Engine::m_pWavBuffer; 
Uint32 Hfyda2Engine::m_uiWavlength; 

Hfyda2Engine::Hfyda2Engine(void): 
    m_iHitCounter(0), 
    oTileManager(16 * 3, 16 * 3, 3, 17, 27),
    m_upState(nullptr) {

    // m_State = std::make_unique<State>(StartState()); // C++14 and above only
    m_upState = std::unique_ptr<State>(new StartState());

    // for some reason, SDL_Init() in BaseEngine::initialize() doesn't intialize for SDL_*Device() methods
    if(SDL_Init(SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDl: %s\n", SDL_GetError());
    }

    if(SDL_LoadWAV(MUSIC_FILE, &m_sWavSpec, &m_pWavBuffer, &m_uiWavlength) == nullptr) {
        fprintf(stderr, "Could not open %s: %s\n", MUSIC_FILE, SDL_GetError());
    } 

    m_sWavSpec.userdata = nullptr;
    m_sWavSpec.callback = AudioHelper::audioCallBack;

    pAudioBuffer = m_pWavBuffer;
    uiAudioLength = m_uiWavlength;

    pAudioBufferOri = m_pWavBuffer;
    uiAudioLengthOri = m_uiWavlength;


    m_uiDev = SDL_OpenAudioDevice(nullptr, 0, &m_sWavSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if(m_uiDev == 0) {
        SDL_Log("Failed to open audio: %s\n", SDL_GetError());
    }

    SDL_PauseAudioDevice(m_uiDev, 0);

}


Hfyda2Engine::~Hfyda2Engine(void) {
    if(m_uiDev > 0) {
        SDL_PauseAudioDevice(m_uiDev, 1);
        SDL_CloseAudioDevice(m_uiDev);
        SDL_FreeWAV(m_pWavBuffer);
        pAudioBuffer = nullptr;
        pAudioBufferOri = nullptr;
    } 

}

void Hfyda2Engine::virtSetupBackgroundBuffer() {
    m_upState->setupBackground(*this);    

}   

int Hfyda2Engine::virtInitialiseObjects() {

    m_upState->setupObjects(*this);    

    return 0;
}

void Hfyda2Engine::virtKeyDown( int iKeyCode ) {
    m_upState->keyPressHandler(*this, iKeyCode);
}

void Hfyda2Engine::virtMouseDown( int iButton, int iX, int iY) {
    
}

void Hfyda2Engine::virtDrawStringsOnTop() {
    m_upState->drawStringsOnTop(*this);
}

void Hfyda2Engine::virtMainLoopPreUpdate() {
    if(isPaused()) 
        SDL_PauseAudioDevice(m_uiDev, 1); 
    else
        SDL_PauseAudioDevice(m_uiDev, 0);

    m_upState->preMainLoopHook(*this);
}

void Hfyda2Engine::copyAllBackgroundBuffer() {
    m_upState->copyBackgroundBuffer(*this);
}

Hfyda2TileManager& Hfyda2Engine::getTileManager() {
    return oTileManager;
}


void AudioHelper::audioCallBack(void* userdata, Uint8* stream, int len) {

    // very hacky way of making it repeat
    if (uiAudioLength == 0) {
        pAudioBuffer = pAudioBufferOri;
        uiAudioLength = uiAudioLengthOri;
        // return;
    }
			
	len = len > uiAudioLength ? uiAudioLength : len;
	SDL_memcpy(stream, pAudioBuffer, len); // thanks Jason for this solution
    
    pAudioBuffer += len;
	uiAudioLength -= len;
    
}

