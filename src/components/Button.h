#ifndef BUTTON_H
#define BUTTON_H

// #include <functional>

#include "../header.h"
#include "../Hfyda2Engine.h"
#include "../DisplayableObject.h"
#include "../states/StartState.h"
#include "../states/State.h"

// TODO: use template instead, but its compile time, runtime dynamics like state change might not work?


// typedef void (StartState::* fp_State_Engine_t)(Hfyda2Engine&);
// typedef std::function<void(const StartState&, Hfyda2Engine&)> fp;

class Button : public DisplayableObject { 

    typedef void (*fp_Engine_t)(Hfyda2Engine&, State*);

    public:
        Button(Hfyda2Engine& rEngine, int iXPos, int iYPos, int iWidth, int iHeight, const char* pText, unsigned int uiPrimaryColor, unsigned int uiSecondaryColor, State* pState, fp_Engine_t fpOnClickHandler, int iFontSize) 
            : DisplayableObject(iXPos, iYPos, &rEngine, iWidth, iHeight, true),
                m_rEngine(rEngine),
                m_pButtonText(pText),
                m_uiPrimaryColor(uiPrimaryColor),
                m_uiSecondaryColor(uiSecondaryColor),
                m_fpOnClickHandler(fpOnClickHandler),
                m_pState(pState),
                m_iFontSize(iFontSize) {
        }

        void virtDraw() override {
            // uncomment to see size of button
            // m_rEngine.drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY, getDrawingRegionRight(), getDrawingRegionBottom(), 0x000000);

             m_rEngine.drawForegroundString(m_iCurrentScreenX, m_iCurrentScreenY, m_pButtonText, (m_bMouseHover ? m_uiPrimaryColor : m_uiSecondaryColor), m_rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", (m_bMouseHover ? m_iFontSize * 4 / 3: m_iFontSize)));

                
        }

        void virtDoUpdate(int iCurrentTime) {
            if(virtIsPositionWithinObject(m_rEngine.getCurrentMouseX(), m_rEngine.getCurrentMouseY())) 
                m_bMouseHover = true;
             else
                m_bMouseHover = false;

            redrawRectangle();
        }

        void virtMouseUp(int iButton, int iX, int iY) override {
            if(iX >= getDrawingRegionLeft() && iX <= getDrawingRegionRight() && iY >= getDrawingRegionTop() && iY <= getDrawingRegionBottom()) {
                switch(iButton) {
                    case 1: // left click
                        m_fpOnClickHandler(m_rEngine, m_pState);
                        // m_fpOnClickHandler(*m_rState, m_rEngine);
                        // (m_rState->*m_fpOnClickHandler)(m_rEngine);
                        break;
                    case 3: // right click
                        return;
                }
            } 
        }
    
    private:   
        Hfyda2Engine& m_rEngine;
        State* m_pState;
        unsigned int m_uiPrimaryColor;
        unsigned int m_uiSecondaryColor;

        int m_iFontSize;

        bool m_bMouseHover;

        const char* m_pButtonText;
        // fp_Engine_State m_fpOnClickHandler;
        fp_Engine_t m_fpOnClickHandler;
        // fp m_fpOnClickHandler;
};


#endif