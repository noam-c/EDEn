# Project: GamePrototype
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++
CC   = gcc
RES  = 
OBJ  = obj/DebugUtils.o obj/Exception.o obj/ExecutionStack.o obj/GameState.o obj/GraphicsUtil.o obj/main.o obj/Resource.o obj/ResourceLoader.o obj/MainMenu.o obj/MainMenuActions.o obj/OpenGLTTF.o obj/StringListModel.o obj/TextBox.o obj/TitleScreenListBox.o obj/DialogueController.o obj/Map.o obj/NPC.o obj/Region.o obj/TileEngine.o obj/Tileset.o obj/ScriptFunctions.o obj/ScriptEngine.o obj/Label.o obj/Container.o obj/Script.o obj/Scheduler.o obj/Thread.o obj/Timer.o obj/Task.o obj/Music.o obj/Sound.o obj/FileScript.o obj/StringScript.o obj/ScriptFactory.o obj/NPCScript.o obj/Spritesheet.o obj/Frame.o obj/Sprite.o obj/Animation.o $(RES)
LINKOBJ  = obj/DebugUtils.o obj/Exception.o obj/ExecutionStack.o obj/GameState.o obj/GraphicsUtil.o obj/main.o obj/Resource.o obj/ResourceLoader.o obj/MainMenu.o obj/MainMenuActions.o obj/OpenGLTTF.o obj/StringListModel.o obj/TextBox.o obj/TitleScreenListBox.o obj/DialogueController.o obj/Map.o obj/NPC.o obj/Region.o obj/TileEngine.o obj/Tileset.o obj/ScriptFunctions.o obj/ScriptEngine.o obj/Label.o obj/Container.o obj/Script.o obj/Scheduler.o obj/Thread.o obj/Timer.o obj/Task.o obj/Music.o obj/Sound.o obj/FileScript.o obj/StringScript.o obj/ScriptFactory.o obj/NPCScript.o obj/Spritesheet.o obj/Frame.o obj/Sprite.o obj/Animation.o $(RES)
LIBS =  -lSDLmain -llua5.1 -lguichan -lguichan_sdl -lguichan_opengl -lSDL_ttf -lSDL_image -lSDL_mixer -lSDL -lGL -lGLU -pg  -g3 
INCS =  -I"include"  -I"src"  -I"src/Audio"  -I"src/Coroutines"  -I"src/edwt"  -I"src/MainMenu"  -I"src/ResourceLoader"  -I"src/ScriptEngine"  -I"src/Sprites"  -I"src/TileEngine"  -I"include"  -I"include/SDL" 
CXXINCS =  -I"lib/gcc/mingw32/3.4.2/include"  -I"include/c++/3.4.2/backward"  -I"include/c++/3.4.2/mingw32"  -I"include/c++/3.4.2"  -I"include"  -I"src"  -I"src/Audio"  -I"src/Coroutines"  -I"src/edwt"  -I"src/MainMenu"  -I"src/ResourceLoader"  -I"src/ScriptEngine"  -I"src/Sprites"  -I"src/TileEngine"  -I"include"  -I"include/SDL" 
BIN  = ED
CXXFLAGS = $(CXXINCS) -DGUICHAN_DLL_IMPORT=1   -fexceptions -pg -g3
CFLAGS = $(INCS) -Dmain=SDL_main -DGUICHAN_DLL_IMPORT=1   -fexceptions -pg -g3
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before ED all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "ED" $(LIBS)

obj/DebugUtils.o: src/DebugUtils.cpp
	$(CPP) -c src/DebugUtils.cpp -o obj/DebugUtils.o $(CXXFLAGS)

obj/Exception.o: src/Exception.cpp
	$(CPP) -c src/Exception.cpp -o obj/Exception.o $(CXXFLAGS)

obj/ExecutionStack.o: src/ExecutionStack.cpp
	$(CPP) -c src/ExecutionStack.cpp -o obj/ExecutionStack.o $(CXXFLAGS)

obj/GameState.o: src/GameState.cpp
	$(CPP) -c src/GameState.cpp -o obj/GameState.o $(CXXFLAGS)

obj/GraphicsUtil.o: src/GraphicsUtil.cpp
	$(CPP) -c src/GraphicsUtil.cpp -o obj/GraphicsUtil.o $(CXXFLAGS)

obj/main.o: src/main.cpp
	$(CPP) -c src/main.cpp -o obj/main.o $(CXXFLAGS)

obj/Resource.o: src/ResourceLoader/Resource.cpp
	$(CPP) -c src/ResourceLoader/Resource.cpp -o obj/Resource.o $(CXXFLAGS)

obj/ResourceLoader.o: src/ResourceLoader/ResourceLoader.cpp
	$(CPP) -c src/ResourceLoader/ResourceLoader.cpp -o obj/ResourceLoader.o $(CXXFLAGS)

obj/MainMenu.o: src/MainMenu/MainMenu.cpp
	$(CPP) -c src/MainMenu/MainMenu.cpp -o obj/MainMenu.o $(CXXFLAGS)

obj/MainMenuActions.o: src/MainMenu/MainMenuActions.cpp
	$(CPP) -c src/MainMenu/MainMenuActions.cpp -o obj/MainMenuActions.o $(CXXFLAGS)

obj/OpenGLTTF.o: src/edwt/OpenGLTTF.cpp
	$(CPP) -c src/edwt/OpenGLTTF.cpp -o obj/OpenGLTTF.o $(CXXFLAGS)

obj/StringListModel.o: src/edwt/StringListModel.cpp
	$(CPP) -c src/edwt/StringListModel.cpp -o obj/StringListModel.o $(CXXFLAGS)

obj/TextBox.o: src/edwt/TextBox.cpp
	$(CPP) -c src/edwt/TextBox.cpp -o obj/TextBox.o $(CXXFLAGS)

obj/TitleScreenListBox.o: src/edwt/TitleScreenListBox.cpp
	$(CPP) -c src/edwt/TitleScreenListBox.cpp -o obj/TitleScreenListBox.o $(CXXFLAGS)

obj/DialogueController.o: src/TileEngine/DialogueController.cpp
	$(CPP) -c src/TileEngine/DialogueController.cpp -o obj/DialogueController.o $(CXXFLAGS)

obj/Map.o: src/TileEngine/Map.cpp
	$(CPP) -c src/TileEngine/Map.cpp -o obj/Map.o $(CXXFLAGS)

obj/NPC.o: src/TileEngine/NPC.cpp
	$(CPP) -c src/TileEngine/NPC.cpp -o obj/NPC.o $(CXXFLAGS)

obj/Region.o: src/TileEngine/Region.cpp
	$(CPP) -c src/TileEngine/Region.cpp -o obj/Region.o $(CXXFLAGS)

obj/TileEngine.o: src/TileEngine/TileEngine.cpp
	$(CPP) -c src/TileEngine/TileEngine.cpp -o obj/TileEngine.o $(CXXFLAGS)

obj/Tileset.o: src/TileEngine/Tileset.cpp
	$(CPP) -c src/TileEngine/Tileset.cpp -o obj/Tileset.o $(CXXFLAGS)

obj/ScriptFunctions.o: src/ScriptEngine/ScriptFunctions.cpp
	$(CPP) -c src/ScriptEngine/ScriptFunctions.cpp -o obj/ScriptFunctions.o $(CXXFLAGS)

obj/ScriptEngine.o: src/ScriptEngine/ScriptEngine.cpp
	$(CPP) -c src/ScriptEngine/ScriptEngine.cpp -o obj/ScriptEngine.o $(CXXFLAGS)

obj/Label.o: src/edwt/Label.cpp
	$(CPP) -c src/edwt/Label.cpp -o obj/Label.o $(CXXFLAGS)

obj/Container.o: src/edwt/Container.cpp
	$(CPP) -c src/edwt/Container.cpp -o obj/Container.o $(CXXFLAGS)

obj/Script.o: src/ScriptEngine/Script.cpp
	$(CPP) -c src/ScriptEngine/Script.cpp -o obj/Script.o $(CXXFLAGS)

obj/Scheduler.o: src/Coroutines/Scheduler.cpp
	$(CPP) -c src/Coroutines/Scheduler.cpp -o obj/Scheduler.o $(CXXFLAGS)

obj/Thread.o: src/Coroutines/Thread.cpp
	$(CPP) -c src/Coroutines/Thread.cpp -o obj/Thread.o $(CXXFLAGS)

obj/Timer.o: src/Coroutines/Timer.cpp
	$(CPP) -c src/Coroutines/Timer.cpp -o obj/Timer.o $(CXXFLAGS)

obj/Task.o: src/Coroutines/Task.cpp
	$(CPP) -c src/Coroutines/Task.cpp -o obj/Task.o $(CXXFLAGS)

obj/Music.o: src/Audio/Music.cpp
	$(CPP) -c src/Audio/Music.cpp -o obj/Music.o $(CXXFLAGS)

obj/Sound.o: src/Audio/Sound.cpp
	$(CPP) -c src/Audio/Sound.cpp -o obj/Sound.o $(CXXFLAGS)

obj/FileScript.o: src/ScriptEngine/FileScript.cpp
	$(CPP) -c src/ScriptEngine/FileScript.cpp -o obj/FileScript.o $(CXXFLAGS)

obj/StringScript.o: src/ScriptEngine/StringScript.cpp
	$(CPP) -c src/ScriptEngine/StringScript.cpp -o obj/StringScript.o $(CXXFLAGS)

obj/ScriptFactory.o: src/ScriptEngine/ScriptFactory.cpp
	$(CPP) -c src/ScriptEngine/ScriptFactory.cpp -o obj/ScriptFactory.o $(CXXFLAGS)

obj/NPCScript.o: src/ScriptEngine/NPCScript.cpp
	$(CPP) -c src/ScriptEngine/NPCScript.cpp -o obj/NPCScript.o $(CXXFLAGS)

obj/Spritesheet.o: src/Sprites/Spritesheet.cpp
	$(CPP) -c src/Sprites/Spritesheet.cpp -o obj/Spritesheet.o $(CXXFLAGS)

obj/Frame.o: src/Sprites/Frame.cpp
	$(CPP) -c src/Sprites/Frame.cpp -o obj/Frame.o $(CXXFLAGS)

obj/Sprite.o: src/Sprites/Sprite.cpp
	$(CPP) -c src/Sprites/Sprite.cpp -o obj/Sprite.o $(CXXFLAGS)

obj/Animation.o: src/Sprites/Animation.cpp
	$(CPP) -c src/Sprites/Animation.cpp -o obj/Animation.o $(CXXFLAGS)