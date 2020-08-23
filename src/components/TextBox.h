#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "../DisplayableObject.h"
#include "../Hfyda2Engine.h"

class TextBox : public DisplayableObject {

    public:
        TextBox(Hfyda2Engine& rEngine, int iXPos, int iYPos, int iWidth, int iHeight, const char* pText, int iFontSize, uint iFontColour) 
        :   DisplayableObject(iXPos, iYPos, &rEngine, iWidth, iHeight, true),
            m_rEngine(rEngine),
            m_pText(pText),
            m_iFontSize(iFontSize),
            m_iFontColour(iFontColour) {                
        }

        ~TextBox() {}
    

        void virtDraw() override {
                m_rEngine.drawForegroundRectangle(m_iCurrentScreenX - 20, m_iCurrentScreenY - 10, getDrawingRegionRight() + 20, getDrawingRegionBottom() + 10, 0x000000);
                m_rEngine.drawForegroundString(m_iCurrentScreenX, m_iCurrentScreenY, m_pText, m_iFontColour, m_rEngine.getFont("resources/fonts/TheWildBreathOfZelda-15Lv.ttf", m_iFontSize));

                    
            }

        void virtDoUpdate(int iCurrentTime) {
            if(m_bFreeze) return;
            redrawRectangle();
        }

        long virtNotify( int iSignalNumber, int x, int y ) {
            switch(iSignalNumber) {
                case 2:
                case 1:
                    m_bFreeze = true;
                    break;
                default:
                    break;
            }
            return 1;
        }


    private:
        Hfyda2Engine& m_rEngine;

        int m_iFontSize;
        unsigned int m_iFontColour;

        bool m_bFreeze;

        const char* m_pText;


};


#endif