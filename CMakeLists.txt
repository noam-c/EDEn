cmake_minimum_required(VERSION 2.8.5)

project( eden )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

set(HEADERS
  src/Audio/Music.h
  src/Audio/Sound.h
  src/Coroutines/Coroutine.h
  src/Coroutines/Scheduler.h
  src/Coroutines/Task.h
  src/Coroutines/TaskId.h
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
  src/Graphics/GraphicsUtil.h
  src/Graphics/OpenGLExtensions.h
  src/Graphics/ScreenTexture.h
  src/Graphics/Texture.h
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
  src/Menu/HomeMenu.h
  src/Menu/HomeViewModel.h
  src/Menu/ImageFormatter.h
  src/Menu/MenuShell.h
  src/Messaging/MessagePipe.h
  src/Messaging/Listener.h
  src/MovementDirection.h
  src/PlayerData/Character.h
  src/PlayerData/LuaCharacter.h
  src/PlayerData/CharacterRoster.h
  src/PlayerData/LuaCharacterRoster.h
  src/PlayerData/PlayerData.h
  src/PlayerData/EquipData.h
  src/PlayerData/EquipSlot.h
  src/PlayerData/Inventory.h
  src/PlayerData/LuaInventory.h
  src/PlayerData/Quest.h
  src/PlayerData/LuaQuest.h
  src/PlayerData/SaveGameItemNames.h
  src/PlayerData/Messages/RosterUpdateMessage.h
  src/ResourceLoader/Resource.h
  src/ResourceLoader/ResourceKey.h
  src/ResourceLoader/ResourceLoader.h
  src/rocket/EdenRocketBindings.h
  src/rocket/EdenRocketRenderInterface.h
  src/rocket/EdenRocketSystemInterface.h
  src/rocket/RocketSDLInputMapping.h
  src/rocket/RocketListener.h
  src/ScriptEngine/FileScript.h
  src/ScriptEngine/NPCScript.h
  src/ScriptEngine/Script.h
  src/ScriptEngine/ScriptEngine.h
  src/ScriptEngine/ScriptException.h
  src/ScriptEngine/ScriptFactory.h
  src/ScriptEngine/StringScript.h
  src/Shapes/Size.h
  src/Shapes/Point2D.h
  src/Shapes/Rectangle.h
  src/Singleton.h
  src/Sprites/Animation.h
  src/Sprites/FrameSequence.h
  src/Sprites/Sprite.h
  src/Sprites/Spritesheet.h
  src/TileEngine/Actor.h
  src/TileEngine/Actor_Orders.h 
  src/TileEngine/LuaActor.h
  src/TileEngine/DialogueController.h
  src/TileEngine/EntityGrid.h
  src/TileEngine/Layer.h
  src/TileEngine/Map.h
  src/TileEngine/MapExit.h
  src/TileEngine/NPC.h
  src/TileEngine/Pathfinder.h
  src/TileEngine/PlayerCharacter.h
  src/TileEngine/LuaPlayerCharacter.h
  src/TileEngine/Region.h
  src/TileEngine/TileEngine.h
  src/TileEngine/LuaTileEngine.h
  src/TileEngine/Tileset.h
  src/TileEngine/TileState.h
  src/TileEngine/TriggerZone.h
  src/TileEngine/Messages/ActorMoveMessage.h
  src/TileEngine/Messages/MapExitMessage.h
  src/TileEngine/Messages/MapTriggerMessage.h
  src/tinyxml/tinystr.h
  src/tinyxml/tinyxml.h
  src/Transitions/TransitionState.h
  src/Transitions/FadeState.h
  src/Transitions/SpinState.h
  src/Transitions/RandomTransitionGenerator.h
  src/Transitions/BlendState.h
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
  src/Coroutines/Coroutine.cpp
  src/Coroutines/Scheduler.cpp
  src/Coroutines/Task.cpp
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
  src/Menu/HomeMenu.cpp
  src/Menu/HomeViewModel.cpp
  src/Menu/ImageFormatter.cpp
  src/Menu/MenuShell.cpp
  src/Messaging/MessagePipe.cpp
  src/PlayerData/Character.cpp
  src/PlayerData/CharacterRoster.cpp
  src/PlayerData/LuaCharacter.cpp
  src/PlayerData/LuaCharacterRoster.cpp
  src/PlayerData/PlayerData.cpp
  src/PlayerData/Quest.cpp
  src/PlayerData/LuaQuest.cpp
  src/PlayerData/EquipData.cpp
  src/PlayerData/EquipSlot.cpp
  src/PlayerData/Inventory.cpp
  src/PlayerData/LuaInventory.cpp
  src/PlayerData/Messages/RosterUpdateMessage.cpp
  src/ResourceLoader/Resource.cpp
  src/ResourceLoader/ResourceLoader.cpp
  src/rocket/EdenRocketRenderInterface.cpp
  src/rocket/EdenRocketSystemInterface.cpp
  src/rocket/RocketSDLInputMapping.cpp
  src/Shapes/Size.cpp
  src/Shapes/Point2D.cpp
  src/Shapes/Rectangle.cpp
  src/ScriptEngine/FileScript.cpp
  src/ScriptEngine/NPCScript.cpp
  src/ScriptEngine/Script.cpp
  src/ScriptEngine/ScriptEngine.cpp
  src/ScriptEngine/ScriptFactory.cpp
  src/ScriptEngine/ScriptFunctions.cpp
  src/ScriptEngine/StringScript.cpp
  src/Sprites/Animation.cpp
  src/Sprites/Sprite.cpp
  src/Sprites/Spritesheet.cpp
  src/TileEngine/Actor.cpp
  src/TileEngine/Actor_MoveOrder.cpp
  src/TileEngine/Actor_StandOrder.cpp
  src/TileEngine/LuaActor.cpp
  src/TileEngine/DialogueController.cpp
  src/TileEngine/EntityGrid.cpp
  src/TileEngine/Layer.cpp
  src/TileEngine/Map.cpp
  src/TileEngine/MapExit.cpp
  src/TileEngine/NPC.cpp
  src/TileEngine/PlayerCharacter.cpp
  src/TileEngine/LuaPlayerCharacter.cpp
  src/TileEngine/Pathfinder.cpp
  src/TileEngine/Region.cpp
  src/TileEngine/TileEngine.cpp
  src/TileEngine/LuaTileEngine.cpp
  src/TileEngine/Tileset.cpp
  src/TileEngine/TileState.cpp
  src/TileEngine/TriggerZone.cpp
  src/TileEngine/Messages/ActorMoveMessage.cpp
  src/TileEngine/Messages/MapExitMessage.cpp
  src/TileEngine/Messages/MapTriggerMessage.cpp
  src/Transitions/TransitionState.cpp
  src/Transitions/FadeState.cpp
  src/Transitions/SpinState.cpp
  src/Transitions/RandomTransitionGenerator.cpp
  src/Transitions/BlendState.cpp
  src/main.cpp
  src/DebugUtils.cpp
  src/Exception.cpp
  src/ExecutionStack.cpp
  src/GameState.cpp
  src/Graphics/GraphicsUtil.cpp
  src/Graphics/OpenGLExtensions.cpp
  src/Graphics/ScreenTexture.cpp
  src/Graphics/Texture.cpp
)

set(SOURCE_GROUP_DELIMITER "/")

source_group("//" REGULAR_EXPRESSION src/[^/]*)
source_group(Audio REGULAR_EXPRESSION src/Audio/.*)
source_group(Coroutines REGULAR_EXPRESSION src/Coroutines/.*)
source_group(GameData REGULAR_EXPRESSION src/GameData/.*)
source_group(Graphics REGULAR_EXPRESSION src/Graphics/.*)
source_group(guichan REGULAR_EXPRESSION src/guichan/.*)
source_group(MainMenu REGULAR_EXPRESSION src/MainMenu/.*)
source_group(Menu REGULAR_EXPRESSION src/Menu/.*)
source_group(MenuControls REGULAR_EXPRESSION src/MenuControls/.*)
source_group(Messaging REGULAR_EXPRESSION src/Messaging/.*)
source_group(PlayerData REGULAR_EXPRESSION src/PlayerData/.*)
source_group("PlayerData\\Messages" REGULAR_EXPRESSION src/PlayerData/Messages/.*)
source_group(ResourceLoader REGULAR_EXPRESSION src/ResourceLoader/.*)
source_group(rocket REGULAR_EXPRESSION src/rocket/.*)
source_group(ScriptEngine REGULAR_EXPRESSION src/ScriptEngine/.*)
source_group(Shapes REGULAR_EXPRESSION src/Shapes/.*)
source_group(Sprites REGULAR_EXPRESSION src/Sprites/.*)
source_group(TileEngine REGULAR_EXPRESSION src/TileEngine/.*)
source_group("TileEngine\\Messages" REGULAR_EXPRESSION src/TileEngine/Messages/.*)
source_group(Transitions REGULAR_EXPRESSION src/Transitions/.*)
source_group(json REGULAR_EXPRESSION src/json/.*)
source_group(LuaWrapper REGULAR_EXPRESSION src/LuaWrapper/.*)
source_group(edwt REGULAR_EXPRESSION src/edwt/.*)
source_group(tinyxml REGULAR_EXPRESSION src/tinyxml/.*)

include_directories( 
  src
  src/Audio
  src/Coroutines
  src/GameData
  src/Graphics
  src/guichan
  src/MainMenu
  src/Menu
  src/MenuControls
  src/Messaging
  src/PlayerData
  src/PlayerData/Messages
  src/ResourceLoader
  src/rocket
  src/ScriptEngine
  src/Shapes
  src/Sprites
  src/TileEngine
  src/TileEngine/Messages
  src/Transitions
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
	target_link_libraries( eden SDLmain rocketCore rocketControls lua5.1 SDL_ttf SDL_image SDL_mixer SDL opengl32 glu32 )
ELSE(WIN32)
	INCLUDE(FindOpenGL)
	INCLUDE(FindSDL)
	INCLUDE(FindSDL_image)
	INCLUDE(FindSDL_ttf)
	INCLUDE(FindSDL_mixer)
	INCLUDE(FindLua51)
	INCLUDE(FindLibRocket)
	INCLUDE(FindLibRocketControls)

	set(INCL_HEADERS
	  ${LUA_INCLUDE_DIR}
	  ${ROCKET_INCLUDE_DIR}
	  ${ROCKET_CONTROLS_INCLUDE_DIR}
	  ${SDL_INCLUDE_DIR}
	  ${SDLIMAGE_INCLUDE_DIR}
	  ${SDLTTF_INCLUDE_DIR}
	  ${SDLMIXER_INCLUDE_DIR}
	  ${OPENGL_INCLUDE_DIR}
	)

	include_directories(BEFORE SYSTEM ${INCL_HEADERS})

	target_link_libraries( eden ${LIBROCKET_LIBRARY} ${LIBROCKET_CONTROLS_LIBRARY} ${LUA_LIBRARIES} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLMIXER_LIBRARY} ${SDL_LIBRARY} ${OPENGL_LIBRARIES} )
ENDIF(WIN32)

