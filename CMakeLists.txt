cmake_minimum_required(VERSION 2.8)
project(G52CPP)

# SET(GCC_COVERAGE_LINK_FLAGS "-lasan") # not recommneded apparently
# SET(GCC_COVERAGE_LINK_FLAGS "-fno-omit-frame-pointer -fsanitize=address -g3")
# SET(GCC_COVERAGE_COMPILE_FLAGS "-fno-omit-frame-pointer -fsanitize=address -g3")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE RelWithDebInfo) # remove later?
# set(CMAKE_EXE_LINKER_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}")   
                                # ${GCC_COVERAGE_LINK_FLAGS}")
add_definitions(-std=c++11)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${G52CPP_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} 
                    ${SDL2_TTF_INCLUDE_DIR} 
                    ${SDL2_IMAGE_INCLUDE_DIR})                     

set(SOURCE_FILES src/mainfunction.cpp 
src/BaseEngine.cpp
src/BouncingBallMain.cpp
src/BouncingBall.cpp
src/DisplayableObject.cpp
src/DraggingDemo.cpp
src/DrawingSurface.cpp
src/FontManager.cpp
src/ImageObjectDemo.cpp
src/JasonsDemoA.cpp
src/JasonsObjectA.cpp
src/JasonsTileManager.cpp
src/MazeDemoMain.cpp
src/MazeDemoObject.cpp
src/MazeDemoTileManager.cpp
src/PlayingCardsDemo.cpp
src/RawImage.cpp
src/SimpleDemo.cpp
src/TileManager.cpp
src/ZoomingDemo.cpp
src/FlashingDemo.cpp
src/ImageMappingDemo.cpp
src/SimpleImage.cpp
src/StarfieldDemo.cpp
src/JigsawDemo.cpp
src/Hfyda2Engine.cpp
src/Hfyda2TileManager.cpp
src/states/StartState.cpp
src/states/PauseState.cpp
src/states/RunningState.cpp
src/states/NewGameState.cpp
src/states/HighScoreState.cpp
src/states/EndState.cpp
src/actors/Dino.cpp
src/InputHandler.cpp
src/particles/Meteor.cpp
src/particles/Fireball.cpp
src/particles/BlueBeam.cpp
src/monsters/Wizard.cpp
src/monsters/Blue.cpp
src/monsters/Red.cpp
src/Spawner.cpp
)

add_executable(SDL2_App ${SOURCE_FILES})
target_link_libraries(SDL2_App ${SDL2_LIBRARY} 
                                ${SDL2_TTF_LIBRARY} 
                                ${SDL2_IMAGE_LIBRARY})                                

add_custom_command(TARGET SDL2_App PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${G52CPP_SOURCE_DIR}/src/resources ${CMAKE_CURRENT_BINARY_DIR})
