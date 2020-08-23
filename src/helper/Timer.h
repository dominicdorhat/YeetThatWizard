#ifndef TIMER_H
#define TIMER_H

#include "../header.h"

class Timer {
    public: 
        Timer(int iInterval)
        :   m_iInterval(iInterval),
            m_iCurrentTime(0),
            m_iLastTime(0),
            m_iAccumulator(0),
            m_iTotal(0) {}

        ~Timer() {};

        // check if timer has passed the given interval
        bool check() {
            if(ready()) {
                m_iAccumulator = 0;
                return true;
            }
            return false;
        }

        void update() {
            int iDelta;
            m_iCurrentTime = SDL_GetTicks();
            iDelta = m_iCurrentTime - m_iLastTime;
            m_iTotal += iDelta;
            m_iAccumulator += iDelta;
            m_iLastTime = m_iCurrentTime;
        }

        void setInterval(int iInterval) { m_iInterval = iInterval; }

        void startTimer() { 
            m_iCurrentTime = m_iLastTime = SDL_GetTicks();
        }
        

    private:

        int ready() { return m_iAccumulator > m_iInterval; }

        int m_iCurrentTime;
        int m_iLastTime;
        int m_iAccumulator;
        int m_iTotal;
        int m_iInterval;
};

#endif