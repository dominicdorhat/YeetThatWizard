SDL_DIR = ../SDL/include


CXXFLAGS= -I$(SDL_DIR) -std=c++11

LIBS =  -D_REENTRANT -F /Library/Frameworks/ -framework SDL2 -framework SDL2_image -framework SDL2_ttf

DEPS = BaseEngine.o BouncingBallMain.o BouncingBall.o DisplayableObject.o DraggingDemo.o DrawingSurface.o FlashingDemo.o FontManager.o ImageMappingDemo.o ImageObjectDemo.o JasonsDemoA.o JasonsObjectA.o JasonsTileManager.o MazeDemoMain.o MazeDemoObject.o MazeDemoTileManager.o PlayingCardsDemo.o RawImage.o SimpleDemo.o SimpleImage.o StarfieldDemo.o TileManager.o ZoomingDemo.o JigsawDemo.o

OUTPUT = cppcw3and4


$(OUTPUT): $(DEPS)
	$(CXX) $(CXXFLAGS) mainfunction.cpp $(DEPS) $(LIBS) -o $(OUTPUT)

