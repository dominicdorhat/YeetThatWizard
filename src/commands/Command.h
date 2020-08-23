#ifndef COMMAND_H
#define COMMAND_H

// Command pattern implementation for specific player action
#include "../actors/GameActor.h"

class Command {

    public:
        Command(void) {}
        virtual ~Command(void) {}

        virtual void execute(GameActor& rActor) = 0;
    
};

class LeftCommand : public Command{
    public:
        LeftCommand(void) {}
        ~LeftCommand(void) {}

        void execute(GameActor& rActor) override {
            rActor.moveLeft();
        }
};

class RightCommand : public Command {
    public:
        RightCommand(void) {}
        ~RightCommand(void) { }

        void execute(GameActor& rActor) override {
            rActor.moveRight();
        }
};

class UpCommand : public Command {
    public:
        UpCommand(void) {}
        ~UpCommand(void) {}

        virtual void execute(GameActor& rActor) { rActor.climbUp(); }
};

class DownCommand : public Command {
    public:
        DownCommand(void) {}
        ~DownCommand(void) {}

        void execute(GameActor& rActor) override {
            rActor.climbDown();
        }
};

class SpaceBarCommand : public Command {
    public:
        SpaceBarCommand(void) {}
        ~SpaceBarCommand(void) {}

        void execute(GameActor& rActor) override {
            rActor.attack();
        }
};

#endif