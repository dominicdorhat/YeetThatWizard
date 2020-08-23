#include "header.h"
#include "InputHandler.h"
#include "commands/Command.h"

InputHandler::InputHandler() {}

InputHandler::~InputHandler() {}

void InputHandler::initCommands() {
    m_pUpArrow = std::unique_ptr<UpCommand>(new UpCommand());
    m_pDownArrow = std::unique_ptr<DownCommand>(new DownCommand());
    m_pLeftArrow = std::unique_ptr<LeftCommand>(new LeftCommand());
    m_pRightArrow = std::unique_ptr<RightCommand>(new RightCommand());
    m_pSpaceBar = std::unique_ptr<SpaceBarCommand>(new SpaceBarCommand());
}

std::shared_ptr<Command> InputHandler::handleInput(int iKeyCode) {
    if(iKeyCode == SDLK_UP) return m_pUpArrow;
    if(iKeyCode == SDLK_DOWN) return m_pDownArrow;
    if(iKeyCode == SDLK_LEFT) return m_pLeftArrow;
    if(iKeyCode == SDLK_RIGHT) return m_pRightArrow;
    
    if(iKeyCode == SDLK_SPACE) return m_pSpaceBar;
    
    return nullptr;
}




