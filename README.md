# YeetThatWizard
A simple 2D sidescrolling platformer made in `C++` with SDL2 Library. Your aim is to reach the green flag on a platform avoid the meteors from the wizard, while killing any monsters along the way with your fireball. The green flag may turn red, in which means that there are too many monsters in the stage and you need to kill enough to pass.

## Note
This is an assingment for my **COMP2006 C++** module in the University of Nottingham, United Kingdom to create a game with the help of a game engine provided by Jason Atkin that is written with the SDL2 library.

### Git history
As you may notice, I have only a few huge commits and you might be thinking that I have terrible `git` commit discipline. Unfortunately I lost the original `.git` folder, wiping away any of my `git` history. (Don't ask how I lost it :))

## Build Instructions
1) Install `SDL2`, `SDL2_ttf` and `SDL2_image` with your desired package manager.
```bash
mkdir build && cd build
cmake -G 'Unix Makefiles' ..
./SDL2_App
```
