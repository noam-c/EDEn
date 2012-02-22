cmake_minimum_required(VERSION 2.8.5)

project( eden )

set(HEADERS
  src/Audio/Music.h
  src/Audio/Sound.h
  src/Coroutines/Scheduler.h
  src/Coroutines/Task.h
  src/Coroutines/TaskId.h
  src/Coroutines/Thread.h
  src/Coroutines/Timer.h
  src/DebugUtils.h
  src/edwt/Container.h
  src/edwt/DebugConsoleWindow.h
  src/edwt/Icon.h
  src/edwt/Label.h
  src/edwt/ListBox.h
  src/edwt/ModuleSelectListener.h
  src/edwt/OpenGLTTF.h
  src/edwt/StringListModel.h
  src/edwt/Tab.h
  src/edwt/TabbedArea.h
  src/edwt/TabChangeListener.h
  src/edwt/TextAlignment.h
  src/edwt/TextBox.h
  src/edwt/TextField.h
  src/edwt/Window.h
  src/Exception.h
  src/ExecutionStack.h
  src/GameState.h
  src/GLInclude.h
  src/GraphicsUtil.h
  src/guichan/actionevent.hpp
  src/guichan/actionlistener.hpp
  src/guichan/basiccontainer.hpp
  src/guichan/cliprectangle.hpp
  src/guichan/color.hpp
  src/guichan/deathlistener.hpp
  src/guichan/defaultfont.hpp
  src/guichan/event.hpp
  src/guichan/exception.hpp
  src/guichan/focushandler.hpp
  src/guichan/focuslistener.hpp
  src/guichan/font.hpp
  src/guichan/genericinput.hpp
  src/guichan/glut.hpp
  src/guichan/graphics.hpp
  src/guichan/gui.hpp
  src/guichan/image.hpp
  src/guichan/imagefont.hpp
  src/guichan/imageloader.hpp
  src/guichan/input.hpp
  src/guichan/inputevent.hpp
  src/guichan/key.hpp
  src/guichan/keyevent.hpp
  src/guichan/keyinput.hpp
  src/guichan/keylistener.hpp
  src/guichan/listmodel.hpp
  src/guichan/mouseevent.hpp
  src/guichan/mouseinput.hpp
  src/guichan/mouselistener.hpp
  src/guichan/opengl/openglallegroimageloader.hpp
  src/guichan/opengl/openglgraphics.hpp
  src/guichan/opengl/openglimage.hpp
  src/guichan/opengl/openglsdlimageloader.hpp
  src/guichan/opengl.hpp
  src/guichan/platform.hpp
  src/guichan/rectangle.hpp
  src/guichan/sdl/sdlgraphics.hpp
  src/guichan/sdl/sdlimage.hpp
  src/guichan/sdl/sdlimageloader.hpp
  src/guichan/sdl/sdlinput.hpp
  src/guichan/sdl/sdlpixel.hpp
  src/guichan/sdl.hpp
  src/guichan/selectionevent.hpp
  src/guichan/selectionlistener.hpp
  src/guichan/widget.hpp
  src/guichan/widgetlistener.hpp
  src/guichan/widgets/adjustingcontainer.hpp
  src/guichan/widgets/button.hpp
  src/guichan/widgets/checkbox.hpp
  src/guichan/widgets/container.hpp
  src/guichan/widgets/dropdown.hpp
  src/guichan/widgets/icon.hpp
  src/guichan/widgets/imagebutton.hpp
  src/guichan/widgets/label.hpp
  src/guichan/widgets/listbox.hpp
  src/guichan/widgets/radiobutton.hpp
  src/guichan/widgets/scrollarea.hpp
  src/guichan/widgets/slider.hpp
  src/guichan/widgets/tab.hpp
  src/guichan/widgets/tabbedarea.hpp
  src/guichan/widgets/textbox.hpp
  src/guichan/widgets/textfield.hpp
  src/guichan/widgets/window.hpp
  src/guichan.hpp
  src/json/json.h
  src/json/json-forwards.h
  src/GameData/ItemData.h
  src/GameData/Item.h
  src/GameData/ItemList.h
  src/LuaWrapper/LuaWrapper.hpp
  src/MainMenu/MainMenu.h
  src/Menu/CharacterModule.h
  src/Menu/ConfirmStateListener.h
  src/Menu/ConfirmState.h
  src/Menu/DataMenu.h
  src/Menu/DataPane.h
  src/Menu/EquipMenu.h
  src/Menu/EquipPane.h
  src/Menu/HomeMenu.h
  src/Menu/HomePane.h
  src/Menu/ItemsMenu.h
  src/Menu/ItemsPane.h
  src/Menu/MenuPane.h
  src/Menu/MenuState.h
  src/Menu/StatusMenu.h
  src/Menu/StatusPane.h
  src/MenuControls/SaveGameModule.h
  src/MenuControls/MenuAction.h
  src/MenuControls/MenuShell.h
  src/MenuControls/ItemListModel.h
  src/MovementDirection.h
  src/PlayerData/Character.h
  src/PlayerData/PlayerData.h
  src/PlayerData/EquipData.h
  src/PlayerData/EquipSlot.h
  src/PlayerData/Quest.h
  src/PlayerData/SaveGameItemNames.h
  src/Point2D.h
  src/Rectangle.h
  src/ResourceLoader/Resource.h
  src/ResourceLoader/ResourceKey.h
  src/ResourceLoader/ResourceLoader.h
  src/ScriptEngine/FileScript.h
  src/ScriptEngine/NPCScript.h
  src/ScriptEngine/Script.h
  src/ScriptEngine/ScriptEngine.h
  src/ScriptEngine/ScriptException.h
  src/ScriptEngine/ScriptFactory.h
  src/ScriptEngine/StringScript.h
  src/Singleton.h
  src/Sprites/Animation.h
  src/Sprites/FrameSequence.h
  src/Sprites/Sprite.h
  src/Sprites/SpriteFrame.h
  src/Sprites/Spritesheet.h
  src/TileEngine/DialogueController.h
  src/TileEngine/Map.h
  src/TileEngine/NPC.h
  src/TileEngine/LuaNPC.h
  src/TileEngine/NPC_Orders.h
  src/TileEngine/Obstacle.h
  src/TileEngine/Pathfinder.h
  src/TileEngine/PlayerCharacter.h
  src/TileEngine/LuaPlayerCharacter.h
  src/TileEngine/Region.h
  src/TileEngine/TileEngine.h
  src/TileEngine/LuaTileEngine.h
  src/TileEngine/Tileset.h
  src/TileEngine/XMap.h
  src/TileEngine/XRegion.h
  src/tinyxml/tinystr.h
  src/tinyxml/tinyxml.h
  src/TODOLIST.h
)

set(SOURCES 
  src/guichan/actionevent.cpp
  src/guichan/basiccontainer.cpp
  src/guichan/cliprectangle.cpp
  src/guichan/color.cpp
  src/guichan/defaultfont.cpp
  src/guichan/event.cpp
  src/guichan/exception.cpp
  src/guichan/focushandler.cpp
  src/guichan/font.cpp
  src/guichan/genericinput.cpp
  src/guichan/graphics.cpp
  src/guichan/gui.cpp
  src/guichan/guichan.cpp
  src/guichan/image.cpp
  src/guichan/imagefont.cpp
  src/guichan/inputevent.cpp
  src/guichan/key.cpp
  src/guichan/keyevent.cpp
  src/guichan/keyinput.cpp
  src/guichan/mouseevent.cpp
  src/guichan/mouseinput.cpp
  src/guichan/opengl/opengl.cpp
  src/guichan/opengl/openglgraphics.cpp
  src/guichan/opengl/openglimage.cpp
  src/guichan/rectangle.cpp
  src/guichan/sdl/sdl.cpp
  src/guichan/sdl/sdlgraphics.cpp
  src/guichan/sdl/sdlimage.cpp
  src/guichan/sdl/sdlimageloader.cpp
  src/guichan/sdl/sdlinput.cpp
  src/guichan/selectionevent.cpp
  src/guichan/widget.cpp
  src/guichan/widgets/adjustingcontainer.cpp
  src/guichan/widgets/button.cpp
  src/guichan/widgets/checkbox.cpp
  src/guichan/widgets/container.cpp
  src/guichan/widgets/dropdown.cpp
  src/guichan/widgets/icon.cpp
  src/guichan/widgets/imagebutton.cpp
  src/guichan/widgets/label.cpp
  src/guichan/widgets/listbox.cpp
  src/guichan/widgets/radiobutton.cpp
  src/guichan/widgets/scrollarea.cpp
  src/guichan/widgets/slider.cpp
  src/guichan/widgets/tab.cpp
  src/guichan/widgets/tabbedarea.cpp
  src/guichan/widgets/textbox.cpp
  src/guichan/widgets/textfield.cpp
  src/guichan/widgets/window.cpp
  src/tinyxml/tinystr.cpp
  src/tinyxml/tinyxml.cpp
  src/tinyxml/tinyxmlerror.cpp
  src/tinyxml/tinyxmlparser.cpp
  src/json/jsoncpp.cpp
  src/Audio/Music.cpp
  src/Audio/Sound.cpp
  src/Coroutines/Scheduler.cpp
  src/Coroutines/Task.cpp
  src/Coroutines/Thread.cpp
  src/Coroutines/Timer.cpp
  src/edwt/Container.cpp
  src/edwt/DebugConsoleWindow.cpp
  src/edwt/Icon.cpp
  src/edwt/Label.cpp
  src/edwt/ListBox.cpp
  src/edwt/OpenGLTTF.cpp
  src/edwt/StringListModel.cpp
  src/edwt/Tab.cpp
  src/edwt/TabbedArea.cpp
  src/edwt/TextBox.cpp
  src/edwt/TextField.cpp
  src/edwt/Window.cpp
  src/GameData/ItemData.cpp
  src/GameData/Item.cpp
  src/MainMenu/MainMenu.cpp
  src/MainMenu/MainMenuActions.cpp
  src/Menu/CharacterModule.cpp
  src/Menu/ConfirmState.cpp
  src/Menu/DataMenu.cpp
  src/Menu/DataPane.cpp
  src/Menu/EquipMenu.cpp
  src/Menu/EquipPane.cpp
  src/Menu/HomeMenu.cpp
  src/Menu/HomePane.cpp
  src/Menu/ItemsMenu.cpp
  src/Menu/ItemsPane.cpp
  src/Menu/MenuPane.cpp
  src/Menu/MenuState.cpp
  src/Menu/StatusMenu.cpp
  src/Menu/StatusPane.cpp
  src/MenuControls/SaveGameModule.cpp
  src/MenuControls/MenuShell.cpp
  src/MenuControls/ItemListModel.cpp
  src/PlayerData/Character.cpp
  src/PlayerData/PlayerData.cpp
  src/PlayerData/Quest.cpp
  src/PlayerData/EquipData.cpp
  src/PlayerData/EquipSlot.cpp
  src/ResourceLoader/Resource.cpp
  src/ResourceLoader/ResourceLoader.cpp
  src/ScriptEngine/FileScript.cpp
  src/ScriptEngine/NPCScript.cpp
  src/ScriptEngine/Script.cpp
  src/ScriptEngine/ScriptEngine.cpp
  src/ScriptEngine/ScriptFactory.cpp
  src/ScriptEngine/ScriptFunctions.cpp
  src/ScriptEngine/StringScript.cpp
  src/Sprites/Animation.cpp
  src/Sprites/Sprite.cpp
  src/Sprites/SpriteFrame.cpp
  src/Sprites/Spritesheet.cpp
  src/TileEngine/DialogueController.cpp
  src/TileEngine/Map.cpp
  src/TileEngine/NPC.cpp
  src/TileEngine/LuaNPC.cpp
  src/TileEngine/NPC_MoveOrder.cpp
  src/TileEngine/NPC_StandOrder.cpp
  src/TileEngine/Obstacle.cpp
  src/TileEngine/PlayerCharacter.cpp
  src/TileEngine/LuaPlayerCharacter.cpp
  src/TileEngine/Pathfinder.cpp
  src/TileEngine/Region.cpp
  src/TileEngine/TileEngine.cpp
  src/TileEngine/LuaTileEngine.cpp
  src/TileEngine/Tileset.cpp
  src/TileEngine/XMap.cpp
  src/TileEngine/XRegion.cpp
  src/main.cpp
  src/DebugUtils.cpp
  src/Exception.cpp
  src/ExecutionStack.cpp
  src/GameState.cpp
  src/GraphicsUtil.cpp
  src/Point2D.cpp
  src/Rectangle.cpp
)

SET(SOURCE_GROUP_DELIMITER "/")

source_group("//" REGULAR_EXPRESSION src/[^/]*)
source_group(Audio REGULAR_EXPRESSION src/Audio/.*)
source_group(Coroutines REGULAR_EXPRESSION src/Coroutines/.*)
source_group(GameData REGULAR_EXPRESSION src/GameData/.*)
source_group(guichan REGULAR_EXPRESSION src/guichan/.*)
source_group(MainMenu REGULAR_EXPRESSION src/MainMenu/.*)
source_group(Menu REGULAR_EXPRESSION src/Menu/.*)
source_group(MenuControls REGULAR_EXPRESSION src/MenuControls/.*)
source_group(PlayerData REGULAR_EXPRESSION src/PlayerData/.*)
source_group(ResourceLoader REGULAR_EXPRESSION src/ResourceLoader/.*)
source_group(ScriptEngine REGULAR_EXPRESSION src/ScriptEngine/.*)
source_group(Sprites REGULAR_EXPRESSION src/Sprites/.*)
source_group(TileEngine REGULAR_EXPRESSION src/TileEngine/.*)
source_group(json REGULAR_EXPRESSION src/json/.*)
source_group(LuaWrapper REGULAR_EXPRESSION src/LuaWrapper/.*)
source_group(edwt REGULAR_EXPRESSION src/edwt/.*)
source_group(tinyxml REGULAR_EXPRESSION src/tinyxml/.*)

include_directories( 
  src
  src/Audio
  src/Coroutines
  src/GameData
  src/guichan
  src/MainMenu
  src/Menu
  src/MenuControls
  src/PlayerData
  src/ResourceLoader
  src/ScriptEngine
  src/Sprites
  src/TileEngine
  src/json
  src/LuaWrapper
  src/edwt
  src/tinyxml
)

add_definitions( 
  -DDEBUG_MODE
  -D_CONSOLE
)

add_executable( eden ${SOURCES} ${HEADERS} )

IF(WIN32)
	target_link_libraries( eden SDLmain lua5.1 SDL_ttf SDL_image SDL_mixer SDL opengl32 glu32 )
ELSE(WIN32)
	INCLUDE(FindOpenGL)
	INCLUDE(FindSDL)
	INCLUDE(FindSDL_image)
	INCLUDE(FindSDL_ttf)
	INCLUDE(FindSDL_mixer)
	INCLUDE(FindLua51)

	set(INCL_HEADERS
	  ${LUA_INCLUDE_DIR}
	  ${SDL_INCLUDE_DIR}
	  ${SDLIMAGE_INCLUDE_DIR}
	  ${SDLTTF_INCLUDE_DIR}
	  ${SDLMIXER_INCLUDE_DIR}
	  ${OPENGL_INCLUDE_DIR}
	)

	include_directories(BEFORE SYSTEM ${INCL_HEADERS})

	target_link_libraries( eden ${LUA_LIBRARIES} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLMIXER_LIBRARY} ${SDL_LIBRARY} ${OPENGL_LIBRARIES} )
ENDIF(WIN32)

