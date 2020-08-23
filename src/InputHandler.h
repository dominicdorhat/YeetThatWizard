#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

class Command;

#include <memory>

class InputHandler {
    public:
        InputHandler(void);
        ~InputHandler(void);

        void initCommands();
        // void rebindCommand(int iKeyCode, );

        std::shared_ptr<Command> handleInput(int iKeyCode);        

    private:
        std::shared_ptr<Command> m_pUpArrow;    
        std::shared_ptr<Command> m_pDownArrow;
        std::shared_ptr<Command> m_pLeftArrow;
        std::shared_ptr<Command> m_pRightArrow;
        std::shared_ptr<Command> m_pSpaceBar;
};

#endif